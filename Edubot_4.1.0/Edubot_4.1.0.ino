#include"Edu_bot_esp32.h"
#include "BluetoothSerial.h"
#include <LiquidCrystal_I2C.h>
#include "cert.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Firmware Current Version
String FirmwareVer = {
  "4.1"
};

/*
  if else:
  "IF  DIS OBSFFF 11 255 BBB 31 105 ELS        RRR 33 255 LLL 12 255 LLL 12 255 END        "
  "FFF 11 255 IF  DIS OBSFFF 11 255 BBB 31 105 ELS        RRR 33 255 LLL 12 255 LLL 12 255 END        "
  "FFF 11 255 IF  DIS OBSFFF 11 255 BBB 31 105 ELS        RRR 33 255 LLL 12 255 LLL 12 255 END        FFF 11 255 "

  for:
  "FOR 2      AAA 12 255 FFF 11 255 AAA 12 255 EFOR       "
  "FOR 2      AAA 12 255 FFF 11 255 AAA 12 255 EFOR       FFF 11 255 "
  "AAA 12 255 FOR 2      AAA 12 255 FFF 11 255 AAA 12 255 EFOR       FFF 11 255 "

  while:
  "WHL        AAA 12 255 FOR 2      BBB 12 255 FFF 11 255 CCC 12 255 EFOR       WND"
*/


/*setting comparison for condition:
  if robot detect is day         :LIG <= DAY
  if robot detect is night       :LIG >= NIG
  if robot get obstacle          :DIS <= OBS
  if robot got temperature high  :TEM >= HIG
  if robot got temperature low   :TEM <= LOW
  if robot detect gas            :GAS >= DGA
  setting x value for condition:
  if robot detect is day         :DAY
  if robot detect is night       :NIG
  if robot get obstacle          :OBS
  if robot got temperature high  :HIG
  if robot got temperature low   :LOW
  if robot detect gas            :DGA
*/
//setting x value for condition. these var will read value from sensor
//int distance    = 10; // DIS
//int light       = 20; // LIG
//int temperature = 28; // TEM
//int gas         = 30; // GAS


//for getting y value for condition.
int day_value          = 0;   //DAY
int night_value        = 100; //NIG
int obstacle_value     = 15;  //OBS
int hight_temp_value   = 34;  //HIG
int low_temp_value     = 24;  //LOW
int gas_value          = 800; //DGA

int Mode               = 1;   //********Mode Covertion**********//
int Mode_count         = 1;   //********Mode Covertion**********//

long int JoyStick_Data;       // reading data for joystick from bluetooth
String raw_cmd;               // for storing main string scratch data that was coming from serial.
String LCD_data;              //display data;
int stop_cmd = 0;


//Internal Reset setting.
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};
Button button_boot = {
  0,
  0,
  false
};
void IRAM_ATTR isr() {
  button_boot.numberKeyPresses += 1;
  button_boot.pressed = true;
}

void connect_wifi();
void firmwareUpdate(void);
int FirmwareVersionCheck(void);
void bot_signature();
void slicing_execution(String total_block, int block_size);
void fun_operation(String cmd, int delay_m, int rsltn);

int if_processing_unit(String raw_cmd, int len);
int for_processing_unit(String raw_cmd, int len);

String bot_v = "03";

void setup() {
  Serial.begin(BAUDRATE);
  SerialBT.begin("Edubot 4.1.0" + bot_v);
  Serial.println(F("Edubot 4.1.0 started, now you can pair it with bluetooth!"));

  lcd.init();
  lcd.backlight();
  bot_signature();
  Motor_init();
  Led_init();
  Buzzer_init();
  Temperature_sensor_init();
  Ultrasonic_sensor_init();
  IR_sensor_init();
  USB_sensor_init();

  pinMode(button_boot.PIN, INPUT);
  attachInterrupt(button_boot.PIN, isr, RISING);
}



void loop() {
  //************************************************* Joystick Mode*************************************************************//
  if (Mode == 1) {
    if (Mode_count == 1) {
      Serial.println("****************Joystick Mode begin****************");
      Mode_count = 0;
    }
    while (SerialBT.available() > 0) {
      JoyStick_Data = SerialBT.parseInt();
      Serial.print("JoyStick Raw Data: ");
      Serial.println(JoyStick_Data);

      if (JoyStick_Data == 9999) {
        //Move to the Scratch Mode
        Serial.println("************Joystick Mode Break*************");
        Mode = 0;
        break;
      }
      else {
        //Implementation of Joystick part
        Serial.print("JoyStick exe data: ");
        Serial.println(JoyStick_Data);

        if ( JoyStick_Data >= 1000 &&  JoyStick_Data <= 1255) {
          int value = JoyStick_Data - 1000;
          Robot_Move_Forward_Speed(value);
        }
        else if ( JoyStick_Data >= 1300 &&  JoyStick_Data <= 1555) {
          int value = JoyStick_Data - 1300;
          Robot_Move_Backward_Speed(value);
        }
        else if ( JoyStick_Data >= 2000 &&  JoyStick_Data <= 2255) {
          int value = JoyStick_Data - 2000;
          Robot_Move_Left_Speed(value);
        }
        else if ( JoyStick_Data >= 2300 &&  JoyStick_Data <= 2555) {
          int value = JoyStick_Data - 2300;
          Robot_Move_Right_Speed(value);
        }
        else if (JoyStick_Data == 5000) {
          Led_On();
        }
        else if (JoyStick_Data == 5001) {
          Led_Off();
        }
        else if (JoyStick_Data == 80020) {
          Buzzer_On();
        }
        else if (JoyStick_Data == 80021) {
          Buzzer_Off();
        }
        else if (JoyStick_Data == 70010) {
          while (true) {
            //            Serial.print("Smoke Sensor");
            //            Serial.print(A_Read_USB_2());
            //            Serial.println("%");
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("Smoke Sensor");
            lcd.setCursor(6, 1);
            lcd.print(A_Read_USB_P_2());
            lcd.setCursor(9, 1);
            lcd.print("%");

            JoyStick_Data = SerialBT.parseInt();
            if (JoyStick_Data == 70011) {
              bot_signature();
              break;
            }
            else {
              continue;
            }
          }
        }
        else if (JoyStick_Data == 80201) {
          while (true) {
            //            Serial.print("Temperature: ");
            //            Serial.print(Read_Temperature_C());
            //            Serial.println("°C");
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("Temperature");
            lcd.setCursor(4, 1);
            lcd.print(Read_Temperature_C());
            lcd.setCursor(9, 1);
            lcd.print((char)223);
            lcd.setCursor(10, 1);
            lcd.print("C");

            JoyStick_Data = SerialBT.parseInt();
            if (JoyStick_Data == 80211) {
              bot_signature();
              break;
            }
            else {
              continue;
            }
          }
        }
        else if (JoyStick_Data == 40080) {
          while (true) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Distance: ");
            lcd.setCursor(10, 0);
            lcd.print(Read_Distance_CM());
            lcd.setCursor(14, 0);
            lcd.print("CM");
            //            Serial.print("Distance: ");
            //            Serial.print(Read_Distance_CM());
            //            Serial.println("cm");

            JoyStick_Data = SerialBT.parseInt();
            lcd.clear();
            if (JoyStick_Data == 40081) {
              bot_signature();
              break;
            }
            else {
              continue;
            }
          }
        }
        else {
          Robot_Move_Stop();
        }
      }
    }
  }

  //************************************************* Scratch Mode*************************************************************//
  else if (Mode == 0) {
    if (Mode_count == 0) {
      Serial.println("****************Scratch mode begin****************");
      Mode_count = 1;
    }
    while (SerialBT.available() > 0) {
      raw_cmd = SerialBT.readString();//reading string scratch data from bluetooth
      raw_cmd.trim();
      Serial.print("Scratch Raw Data: ");
      Serial.println(raw_cmd);

      if (raw_cmd == "AAAA") {
        //Move to the joystick Mode
        Serial.println("****************Scratch Mode Break***************");
        Mode = 1;
        break;
      }

      else {
        int cmd_length = raw_cmd.length();
        Serial.print("Data Length:  ");
        Serial.println(cmd_length);

        for (int i = 0; i <= cmd_length; i = i + 11) {
          int pri_prm = 0 + i;   // for detecting string 1st index
          int les_prm = 11 + i;  // for detecting string last index

          Serial.println("********main cmd extaxting*********");
          String process_cmd = raw_cmd.substring(pri_prm, les_prm); //for slicing string for making every single command in sub-module
          Serial.println(process_cmd);

          String CMD;                         // main conditional cmd
          CMD = process_cmd.substring(0, 4);  //Spilting main CMD from string for condition
          CMD.trim();




          /*************While Loop Data Processing Start*****************/
          if (CMD == "WHL") {
            //WND - Stop cmd.
            Serial.println("*********while loop prcessing start here************");
            String while_raw_cmd = raw_cmd.substring(pri_prm, cmd_length);
            String while_exe_cmd = while_raw_cmd.substring(11, cmd_length);
            int while_exe_cmd_size = while_exe_cmd.length();
            int w_i;
            Serial.println("#################################");
            Serial.print("while_raw_cmd :");
            Serial.println(while_raw_cmd);
            Serial.print("while_exe_cmd :");
            Serial.println(while_exe_cmd);

            while (1) {
              for (w_i = 0; w_i < while_exe_cmd_size - 11; w_i = w_i + 11) {
                pri_prm = w_i; // for detecting string 1st index
                les_prm = 11 + w_i;// for detecting string last index
                process_cmd = while_exe_cmd.substring(pri_prm, les_prm);
                Serial.println(process_cmd);
                CMD = process_cmd.substring(0, 4);  //Spilt main CMD from string for condition
                CMD.trim();

                //processing will be stop here
                if ((CMD == "SSS") || (CMD == "\0")) {
                  stop_cmd = 1;
                  break;
                }

                if (CMD == "IF") {
                  int last_state_block_b_p = pri_prm; // store last state which position you send to the if extraction system
                  String unprocessed_raw_if_block = while_exe_cmd.substring(pri_prm, cmd_length); // slice only if
                  int unprocessed_raw_if_block_size = unprocessed_raw_if_block.length();
                  Serial.print("unprocessed_raw_if_block :");
                  Serial.println(unprocessed_raw_if_block);
                  int last_state_block_a_p = if_processing_unit(unprocessed_raw_if_block, unprocessed_raw_if_block_size); //read last state of if block
                  w_i = last_state_block_b_p + last_state_block_a_p; // update i for looping.
                }/*************If Else Condition Data Processing End*****************/

                /*************For Loop Data Processing Strat*****************/
                else if (CMD == "FOR") {
                  int last_state_block_b_p = pri_prm; // store last state which position you send to the for extraction system
                  String unprocessed_raw_for_block = while_exe_cmd.substring(pri_prm, cmd_length); // slice only for
                  int unprocessed_raw_for_block_size = unprocessed_raw_for_block.length();
                  Serial.print("unprocessed_raw_if_block :");
                  Serial.println(unprocessed_raw_for_block);
                  int last_state_block_a_p = for_processing_unit(unprocessed_raw_for_block, unprocessed_raw_for_block_size); //read last state of for block
                  w_i = last_state_block_b_p + last_state_block_a_p; // update i for looping.
                } /*************For Loop Data Processing End*****************/


                /*************LCD Display Print Strat*****************/
                else if (CMD == "PNA") {
                  LCD_data = raw_cmd.substring(pri_prm + 3, pri_prm + 36);
                  Serial.print("LCD Data: ");
                  Serial.println(LCD_data);
                  fun_operation(CMD, 0, 0);
                  w_i = w_i + 33;
                } /*************LCD Display Print End*****************/

                /**************single block execution**************/
                else {
                  Serial.println("**************single block execution**************");
                  slicing_execution(process_cmd, 11);
                  Serial.println("**************single block execution**************");
                }

              }//for loop: while loop data processing end.




              /*whole program will be stop here*/
              if (stop_cmd == 1) {
                stop_cmd = 0;
                i = w_i + 11;
                Serial.println("*************Program Stop**************");
                break;
              }
              String exit_cmd = SerialBT.readString(); // reading new data or program will be stop
              if (exit_cmd == "\0") {
                continue;
              }
              else if ((exit_cmd != "\0") || (exit_cmd == "SSS")) {
                Led_Off();
                Buzzer_Off();
                Robot_Move_Stop();
                bot_signature();
                i = w_i + 11;
                Serial.println("*************Program Stop**************");
                break;
              }
            }
            Serial.println("*********while loop prcessing stop here************");
          }/*************While loop Data Processing End*****************/

          /*************If Else Condition Data Processing Strat*****************/
          else if (CMD == "IF") {
            int last_state_block_b_p = pri_prm; // store last state which position you send to the if extraction system
            String unprocessed_raw_if_block = raw_cmd.substring(pri_prm, cmd_length); // slice only if
            int unprocessed_raw_if_block_size = unprocessed_raw_if_block.length();
            Serial.print("unprocessed_raw_if_block :");
            Serial.println(unprocessed_raw_if_block);
            int last_state_block_a_p = if_processing_unit(unprocessed_raw_if_block, unprocessed_raw_if_block_size); //read last state of if block
            i = last_state_block_b_p + last_state_block_a_p; // update i for looping.
          }/*************If Else Condition Data Processing End*****************/

          /*************For Loop Data Processing Strat*****************/
          else if (CMD == "FOR") {
            int last_state_block_b_p = pri_prm; // store last state which position you send to the for extraction system
            String unprocessed_raw_for_block = raw_cmd.substring(pri_prm, cmd_length); // slice only for
            int unprocessed_raw_for_block_size = unprocessed_raw_for_block.length();
            Serial.print("unprocessed_raw_if_block :");
            Serial.println(unprocessed_raw_for_block);
            int last_state_block_a_p = for_processing_unit(unprocessed_raw_for_block, unprocessed_raw_for_block_size); //read last state of for block
            i = last_state_block_b_p + last_state_block_a_p; // update i for looping.
          } /*************For Loop Data Processing End*****************/


          /*************LCD Display Print Strat*****************/
          else if (CMD == "PNA") {
            LCD_data = raw_cmd.substring(pri_prm + 3, pri_prm + 36);
            Serial.print("LCD Data: ");
            Serial.println(LCD_data);
            fun_operation(CMD, 0, 0);
            i = i + 33;
          } /*************LCD Display Print End*****************/

          else if (CMD == "CFW") {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Current Firmware");
            lcd.setCursor(5, 1);
            lcd.print("V ");
            lcd.setCursor(7, 1);
            lcd.print(FirmwareVer);
          }

          /*************Firmware Update section start*****************/
          else if (CMD == "FWU") {
            SerialBT.disconnect();
            delay(300);
            SerialBT.end();


            Serial.print("Active firmware version:");
            Serial.println(FirmwareVer);
            connect_wifi();

            if (WiFi.status() == WL_CONNECTED) {
              Serial.println("wifi connected");
              if (FirmwareVersionCheck()) {
                firmwareUpdate();
              }
              else {
                Serial.print("Active firmware version:");
                Serial.println(FirmwareVer);
                ESP.restart();
              }
            }
            else {
              connect_wifi();
            }
          }/*************Firmware Update section end*****************/

          /**************single block execution**************/
          else {
            Serial.println("**************single block execution**************");
            slicing_execution(process_cmd, 11);
            Serial.println("**************single block execution**************");
          }


        }//for loop : whole data processing.
      }//scratch else
    }//bluetooth read
  }//else if : mode
}// void loop.


int if_processing_unit(String raw_cmd, int len) {
  Serial.println("**************Extraction start of if else condition**************");
  String conditional_x_detect;  //for checking and assigning conditional_y_value
  String conditional_y_detect;  // for checking condition y parameter for that is it for <= or >= and assigning conditional_y_value
  int conditional_x_value;      // for storing sensor value
  int conditional_y_value;      // for storing user value
  int if_end_prm;
  int else_end_prm;

  for (int if_i = 0; if_i <= len; if_i = if_i + 11) {
    int a_prm = if_i;
    String check_end_if = raw_cmd.substring(a_prm, a_prm + 3);

    if (check_end_if == "ELS") {
      if_end_prm = a_prm - 1;
    }
    if (check_end_if == "END") {
      else_end_prm = a_prm - 1;
      break;
    }
  }

  String if_raw_cmd = raw_cmd.substring(0, 11);               // if conditional whole block spilt
  String if_exe_raw_cmd = raw_cmd.substring(11, if_end_prm);        // if conditional execution data spilt.
  int if_exe_raw_cmd_size = if_exe_raw_cmd.length();
  String else_exe_raw_cmd = raw_cmd.substring(if_end_prm + 11, else_end_prm); // else conditional & elecution whole data spilt.
  int else_exe_raw_cmd_size = else_exe_raw_cmd.length();

  String x_cmd = if_raw_cmd.substring(4, 7);  // spilting conditional x for making if condition
  String y_cmd = if_raw_cmd.substring(8, 11); // spilting conditional y for making if condition

  conditional_x_detect = x_cmd;
  conditional_y_detect = y_cmd;

  // setting x value for condition uding x_cmd, that was coming from app & stroing value from sensor read function.
  if (conditional_x_detect == "LIG") {
    conditional_x_value = A_Read_USB_2();
  }
  else if (conditional_x_detect == "DIS") {
    conditional_x_value = Read_Distance_CM();
  }
  else if (conditional_x_detect == "TEM") {
    conditional_x_value = Read_Temperature_C();
  }
  else if (conditional_x_detect == "GAS") {
    conditional_x_value = A_Read_USB_1();
  }


  if ((conditional_y_detect == "DAY") || (conditional_y_detect == "OBS") || (conditional_y_detect == "LOW")) {
    /***************setting conditional y value for condition from predefine value*******************/
    if (conditional_y_detect == "DAY") {
      conditional_y_value = day_value;
    }
    else if (conditional_y_detect == "OBS") {
      conditional_y_value = obstacle_value;
    }
    else if (conditional_y_detect == "LOW") {
      conditional_y_value = low_temp_value;
    }

    /**************if <= condition implementation***************/
    if (conditional_x_value <= conditional_y_value) {
      slicing_execution(if_exe_raw_cmd, if_exe_raw_cmd_size);
    }
    else {
      slicing_execution(else_exe_raw_cmd, else_exe_raw_cmd_size);
    }
  }

  else if ((conditional_y_detect == "NIG") || (conditional_y_detect == "HIG") || (conditional_y_detect == "DGA")) {
    /***************setting conditional y value for condition from predefine value*******************/
    if (conditional_y_detect == "NIG") {
      conditional_y_value = night_value;
    }
    else if (conditional_y_detect == "HIG") {
      conditional_y_value = hight_temp_value;
    }
    else if (conditional_y_detect == "DAG") {
      conditional_y_value = gas_value;
    }

    /**************if > condition implementation***************/
    if (conditional_x_value > conditional_y_value) {
      slicing_execution(if_exe_raw_cmd, if_exe_raw_cmd_size);
    }
    else {
      slicing_execution(else_exe_raw_cmd, else_exe_raw_cmd_size);
    }
  }
  Serial.println("**************Extraction end of if else condition**************");
  return else_end_prm + 1; // update i value for update for loop data extraction.

}



int for_processing_unit(String raw_cmd, int len ) {
  int for_end_prm;
  int for_i;

  Serial.println("*****************For data Extracting*****************");
  for (for_i = 0; for_i <= len; for_i = for_i + 11) { // for checking where for loop is end.
    String check_end_for = raw_cmd.substring(for_i, for_i + 4);
    if (check_end_for == "EFOR") {
      for_end_prm = for_i - 1;
      break;
    }
  }

  String for_raw_data = raw_cmd.substring(0, 11);
  String for_exe_raw_cmd = raw_cmd.substring(11, for_end_prm);
  int for_exe_raw_cmd_size = for_exe_raw_cmd.length();
  String for_time_s = raw_cmd.substring(4 , 6);
  int for_looping_time = for_time_s.toInt();

  Serial.println("#################################");
  Serial.print("for_raw_data :");
  Serial.println(for_raw_data);
  Serial.print("for_exe_raw_cmd :");
  Serial.println(for_exe_raw_cmd);
  Serial.print("for_looping_time :");
  Serial.println(for_looping_time);
  Serial.println("#################################");

  //repeat untill for_looping_time is not end.
  for (int f_i = 1; f_i <= for_looping_time; f_i++) {
    slicing_execution(for_exe_raw_cmd, for_exe_raw_cmd_size);
  }
  Serial.println("*****************For data Extraction done*****************");
  return for_end_prm + 1; //update for loop data extraction.

}

void bot_signature() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("EduBlock.co");
  lcd.setCursor(1, 1);
  lcd.print("Edubot V");
  lcd.setCursor(8, 1);
  lcd.print(FirmwareVer);
  lcd.setCursor(12, 1);
  lcd.print(bot_v);

}

//execution of block.
void fun_operation(String cmd, int delay_m, int rsltn) {
  if (cmd == "LN") {
    Led_On();
  }
  if (cmd == "LF") {
    Led_Off();
  }
  if (cmd == "LNB") {
    Led_On_Set_Brightness(rsltn);
  }
  if (cmd == "LND") {
    Led_On_Delay(delay_m);
  }
  if (cmd == "LFD") {
    Led_Off_Delay(delay_m);
  }
  if (cmd == "LNBD") {
    Led_On_Set_BrightnesS_Delay(rsltn, delay_m);
  }
  if (cmd == "RLN") {
    RGB_Led_On(255, 0, 0);
  }
  if (cmd == "BLN") {
    RGB_Led_On(0, 0, 255);
  }
  if (cmd == "GLN") {
    RGB_Led_On(0, 120, 0);
  }
  if (cmd == "RND") {
    RGB_Led_On_Delay(255, 0, 0, delay_m);
  }
  if (cmd == "BND") {
    RGB_Led_On_Delay(0, 0, 255, delay_m);
  }
  if (cmd == "GND") {
    RGB_Led_On_Delay(0, 120, 0, delay_m);
  }
  if (cmd == "MN") {
    Motor1_On();
  }
  if (cmd == "MF") {
    Motor1_Off();
  }
  if (cmd == "MND") {
    Motor1_On_Delay(delay_m);
  }
  if (cmd == "MFD") {
    Motor1_Off_Delay(delay_m);
  }
  if (cmd == "RMF") {
    Robot_Move_Forward_Speed(rsltn);
  }
  if (cmd == "RMB") {
    Robot_Move_Backward_Speed(rsltn);
  }
  if (cmd == "RMR") {
    Robot_Move_Right_Speed(rsltn);
  }
  if (cmd == "RML") {
    Robot_Move_Left_Speed(rsltn);
  }
  if (cmd == "RDF") {
    Robot_Move_Forward_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RDB") {
    Robot_Move_Backward_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RDR") {
    Robot_Move_Right_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RDL") {
    Robot_Move_Left_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RSF") {
    Robot_Move_Forward_Speed(rsltn);
  }
  if (cmd == "RSB") {
    Robot_Move_Backward_Speed(rsltn);
  }
  if (cmd == "RSR") {
    Robot_Move_Right_Speed(rsltn);
  }
  if (cmd == "RSL") {
    Robot_Move_Left_Speed(rsltn);
  }
  if (cmd == "RSDF") {
    Robot_Move_Forward_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RSDB") {
    Robot_Move_Backward_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RSDR") {
    Robot_Move_Right_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RSDL") {
    Robot_Move_Left_Delay_Speed(delay_m, rsltn);
  }
  if (cmd == "RSS") {
    Robot_Move_Stop();
  }
  if (cmd == "RS") {
    Robot_Move_Stop_Delay(delay_m);
  }
  if (cmd == "RTR") {
    Robot_Move_Right_Delay_Speed(delay_m, 130);
  }
  if (cmd == "RTL") {
    Robot_Move_Left_Delay_Speed(delay_m, 130);
  }
  if (cmd == "THN") {
    Buzzer_On();
  }
  if (cmd == "THF") {
    Buzzer_Off();
  }
  if (cmd == "HND") {
    Buzzer_On_Delay(delay_m);
  }
  if (cmd == "HFD") {
    Buzzer_Off_Delay(delay_m);
  }
  if (cmd == "HNV") {
    Buzzer_On_Set_Vol(rsltn);
  }
  if (cmd == "HNVD") {
    Buzzer_On_Set_Vol_Delay(rsltn, delay_m);
  }
  if (cmd == "GAS") {
    Play_Super_Mario();
  }
  if (cmd == "GAU") {
    Play_Underworld();
  }
  if (cmd == "GAJ") {
    Play_Jingle_Bells();
  }
  if (cmd == "GAW") {
    Play_Wish_Christmas();
  }
  if (cmd == "GAC") {
    Play_Santa_Claus();
  }
  if (cmd == "RD1") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("USB Sensor_1");
    lcd.setCursor(3, 1);
    lcd.print("D Value : ");
    lcd.setCursor(13, 1);
    if (A_Read_USB_P_1() > 10) {
      lcd.print("1");
    }
    else {
      lcd.print("0");
    }
  }
  if (cmd == "RD2") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("USB Sensor_2");
    lcd.setCursor(3, 1);
    lcd.print("D Value : ");
    lcd.setCursor(13, 1);
    if (A_Read_USB_P_2() <= 1) {
      lcd.print("1");
    }
    else {
      lcd.print("0");
    }
  }
  if (cmd == "RA1") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("USB Sensor_1");
    lcd.setCursor(1, 1);
    lcd.print("A Value : ");
    lcd.setCursor(11, 1);
    lcd.print(A_Read_USB_1());
  }
  if (cmd == "RA2") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("USB Sensor_2");
    lcd.setCursor(1, 1);
    lcd.print("A Value : ");
    lcd.setCursor(11, 1);
    lcd.print(A_Read_USB_2());
  }
  if (cmd == "PNA") {
    lcd.clear();
    String row_1 = LCD_data.substring(1, 17);
    String row_2 = LCD_data.substring(17, 33);
    Serial.print("Row 1: ");
    Serial.println(row_1);
    Serial.print("Row 2: ");
    Serial.println(row_2);
    lcd.setCursor(0, 0);
    lcd.print(row_1);
    lcd.setCursor(0, 1);
    lcd.print(row_2);
  }
  if (cmd == "RTC") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Temperature");
    lcd.setCursor(4, 1);
    lcd.print(Read_Temperature_C());
    lcd.setCursor(9, 1);
    lcd.print((char)223);
    lcd.setCursor(10, 1);
    lcd.print("C");
  }
  if (cmd == "RTF") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Temperature");
    lcd.setCursor(4, 1);
    lcd.print(Read_Temperature_F());
    lcd.setCursor(9, 1);
    lcd.print((char)223);
    lcd.setCursor(10, 1);
    lcd.print("F");
  }
  if (cmd == "RGP") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Smoke Value");
    int value = map(A_Read_USB_1(), 0, 4095, 0, 100);
    lcd.setCursor(6, 1);
    lcd.print(value);
    lcd.setCursor(9, 1);
    lcd.print("%");
  }
  if (cmd == "RLP") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Light Value");
    int value = map(A_Read_USB_2(), 0, 4095, 0, 100);
    lcd.setCursor(6, 1);
    lcd.print(value);
    lcd.setCursor(9, 1);
    lcd.print("%");
  }
  if (cmd == "IR1") {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("IR Sensor_1: ");
    lcd.setCursor(14, 0);
    lcd.print(Read_IR_Sensor_1());
  }
  if (cmd == "IR2") {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("IR Sensor_2: ");
    lcd.setCursor(14, 0);
    lcd.print(Read_IR_Sensor_2());
  }
  if (cmd == "IR3") {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("IR Sensor_3: ");
    lcd.setCursor(14, 0);
    lcd.print(Read_IR_Sensor_3());
  }
  if (cmd == "CLR") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   S3  S2  S1   ");
    lcd.setCursor(3, 1);
    lcd.print(Read_IR_Sensor_3());
    lcd.setCursor(7, 1);
    lcd.print(Read_IR_Sensor_2());
    lcd.setCursor(11, 1);
    lcd.print(Read_IR_Sensor_1());

  }
  if (cmd == "FBL") {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Following");
    lcd.setCursor(3, 1);
    lcd.print("Black Line");
    Follow_black_line();
  }
  if (cmd == "FWL") {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Following");
    lcd.setCursor(3, 1);
    lcd.print("White Line");
    Follow_white_line();
  }
  if (cmd == "SMC") {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Distance");
    lcd.setCursor(5, 1);
    lcd.print(Read_Distance_CM());
    lcd.setCursor(8, 1);
    lcd.print("cm");
  }
  if (cmd == "SMI") {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Distance");
    lcd.setCursor(3, 1);
    lcd.print(Read_Distance_INCH());
    lcd.setCursor(9, 1);
    lcd.print("inch");
  }
}

//slicing every scratch block and executing here
void slicing_execution(String total_block, int block_size) {
  for (int x = 0; x < block_size; x = x + 11) {
    int x_s = x;
    int x_e = 11 + x;
    String single_exe_block = total_block.substring(x_s, x_e);
    String exe_cmd = single_exe_block.substring(0, 4);//Spilting main CMD from string for condition
    exe_cmd.trim();
    String st_tm = single_exe_block.substring(4, 7);      // Spilting time from string
    int in_tm = st_tm.toInt();                       // converting int time from string time
    String st_rslutn = single_exe_block.substring(7, 10); // spilting speed/brightness from int time
    int in_rslutn = st_rslutn.toInt();               // converting int time from string time

    Serial.print("exe cmd: ");
    Serial.println(exe_cmd);
    Serial.print("Time :");
    Serial.println(in_tm);
    Serial.print("Speed or Brightness :");
    Serial.println(in_rslutn);
    fun_operation(exe_cmd, in_tm, in_rslutn); // implementation of single block.
  }
}

void connect_wifi() {
  Serial.println("Waiting for WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecting");
  }
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("WiFi Connected");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(750);
}

//firmware update section
void firmwareUpdate(void) {
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);
  t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}
int FirmwareVersionCheck(void) {
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_fw_Version;
  fwurl += "?";
  fwurl += String(rand());
  //Serial.println(fwurl);
  WiFiClientSecure * client = new WiFiClientSecure;

  if (client) {
    client -> setCACert(rootCACertificate);
    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;
    if (https.begin( * client, fwurl)) { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) { // if version received
        payload = https.getString(); // save received version
      }
      else {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Latest firmware");
        lcd.setCursor(1, 1);
        lcd.print("download error");
        delay(1000);
      }
      https.end();
    }
    delete client;
  }

  if (httpCode == HTTP_CODE_OK) { // if version received
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Device already");
      lcd.setCursor(2, 1);
      lcd.print("on latest FW");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Current Firmware");
      lcd.setCursor(5, 1);
      lcd.print("V ");
      lcd.setCursor(7, 1);
      lcd.print("1.1");
      delay(1500);

      return 0;
    }
    else {
      Serial.println(payload);
      Serial.println("New firmware detected");
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("New firmware");
      lcd.setCursor(2, 1);
      lcd.print("detected V ");
      lcd.setCursor(11, 1);
      lcd.print("1.1");
      delay(750);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("downloading...");
      return 1;
    }
  }
  return 0;
}
