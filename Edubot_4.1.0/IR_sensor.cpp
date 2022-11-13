#include"Edu_bot_esp32.h"

void IR_sensor_init() {
  pinMode(IR_SENSOR_1_D, INPUT);
  pinMode(IR_SENSOR_2_D, INPUT);
  pinMode(IR_SENSOR_3_D, INPUT);
  Serial.println("IR Sensor Initialize.");
}

int Read_IR_Sensor_1() {
  int value = digitalRead(IR_SENSOR_1_D);
  return value;
}

int Read_IR_Sensor_2() {
  int value = digitalRead(IR_SENSOR_2_D);
  return value;
}

int Read_IR_Sensor_3() {
  int value = digitalRead(IR_SENSOR_3_D);
  return value;
}

void Follow_black_line() {
  while (1) {
    Serial.println("Black LFR");
    int sensor1 = digitalRead(IR_SENSOR_1_D);
    int sensor2 = digitalRead(IR_SENSOR_2_D);
    int sensor3 = digitalRead(IR_SENSOR_3_D);

    if ((sensor1 == 0) && (sensor2 == 1) && (sensor3 == 0)) {
      Robot_Move_Forward_Speed(110);
    }
    else if ((sensor1 == 1) && (sensor2 == 0) && (sensor3 == 0)) {
      Robot_Move_Left_Speed(90);
    }
    else if ((sensor1 == 0) && (sensor2 == 0) && (sensor3 == 1)) {
      Robot_Move_Right_Speed(90);
    }
    else if ((sensor1 == 1) && (sensor2 == 1) && (sensor3 == 1)) {
      Robot_Move_Stop();
    }
  }
}

void Follow_white_line() {
  while (1) {
    Serial.println("White LFR");
    int sensor1 = digitalRead(IR_SENSOR_1_D);
    int sensor2 = digitalRead(IR_SENSOR_2_D);
    int sensor3 = digitalRead(IR_SENSOR_3_D);

    if ((sensor1 == 1) && (sensor2 == 0) && (sensor3 == 1)) {
      Robot_Move_Forward_Speed(110);
    }
    else if ((sensor1 == 0) && (sensor2 == 1) && (sensor3 == 1)) {
      Robot_Move_Left_Speed(90);
    }
    else if ((sensor1 == 1) && (sensor2 == 1) && (sensor3 == 0)) {
      Robot_Move_Right_Speed(90);
    }
    else if ((sensor1 == 0) && (sensor2 == 0) && (sensor3 == 0)) {
      Robot_Move_Stop();
    }
  }
}
