#include <WiFi.h>
#include <HTTPClient.h>
//*******************for OTA***********
#include <HTTPUpdate.h> // take care the update queries and handling the file that is getting payload and then rebooting the new firmware
#include <WiFiClientSecure.h>
#include "cert.h" //Github is secure website so we require the certificate provided into that thing. It remains same for all the GitHub Account
//*******************END for OTA***********
#include <ArduinoJson.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SH1106 display(OLED_SDA, OLED_SCL);

int timeForTrigerSensorData=600000;//******************************----------------------------------------

const char* ssid       = "ANTT Robotics TP";
const char* password   = "digi@anti@123";

const char* ActionGet_serverName        = "https://iotsnacksbox.io/api/actions?snacksboxtoken=19bbfd0ba4cac58dd638f09a5572e26b1a028ba4647507ddc101c9f6deea81e3" ;//19bbfd0ba4cac58dd638f09a5572e26b1a028ba4647507ddc101c9f6deea81e3
const char* ActionPost_serverName       = "https://iotsnacksbox.io/api/action?snacksboxtoken=19bbfd0ba4cac58dd638f09a5572e26b1a028ba4647507ddc101c9f6deea81e3" ;
const char* TemperaturePost_serverName  = "https://iotsnacksbox.io/api/trigger/Temperature?snacksboxtoken=19bbfd0ba4cac58dd638f09a5572e26b1a028ba4647507ddc101c9f6deea81e3" ;
const char* SmokePost_serverName        = "https://iotsnacksbox.io/api/trigger/Smoke?snacksboxtoken=19bbfd0ba4cac58dd638f09a5572e26b1a028ba4647507ddc101c9f6deea81e3" ;


//*******************for OTA***********
int status = WL_IDLE_STATUS;// cheecking out the ideal condition to handle the disconnection of wifi
int incomingByte; /* TO recive a command from terminal it should workout. Like When I press a button, based
                  on the command if it is maching the string then it will check on the update and
                  if update is available then it will update the firmware. */
String FirmwareVer = {"1.0"};   //It will cheeking with github file if the version is not same then it will update

#define URL_fw_Version "https://raw.githubusercontent.com/faizur-rahman/Home-monitoring-and-controlling-System-with-OTA/main/bin-version.txt"  //Github link of text file to check the version
#define URL_fw_Bin "https://github.com/faizur-rahman/Home-monitoring-and-controlling-System-with-OTA/blob/main/fw.bin"  // Arduino program .bin file link
//*******************END for OTA***********

String payload ;

#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int ManualMode_value;
int ManualMode_state;

//Temp pin No 25
int Led = 4;//4
int Fan = 2;//2
int Buzzer = 15;//tx2
int relay4=13;

int Smoke_Sensor = 35;


int LDR_Sensor = 34;//unused

int Led_state;
int Buzzer_state;
int Fan_state;

int V_Led_value=0;
int V_Buzzer_value=0;
int V_Fan_value=0;

int Temperature=0;
int Smoke=0;

unsigned long previousTime=0;// for calculateing time
unsigned long currentTime=0; // for calculateing 


void setup() {
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  Serial.print("Active Firmware Version:");
  Serial.println(FirmwareVer);
  
  dht.begin();

  pinMode(Led, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  pinMode(Smoke_Sensor, INPUT);
  pinMode(LDR_Sensor, INPUT);

  digitalWrite(Led, HIGH);
  digitalWrite(Buzzer, HIGH);
  digitalWrite(Fan, HIGH);
  digitalWrite(relay4, HIGH);

}


// ***************************************Server Switch*****************************
void Json_Parsing() {
  // Parsing the JSON using https://arduinojson.org/v6/assistant/
  Serial.print("Payload/Json from Server:");
  Serial.println(payload);

  DynamicJsonDocument doc(1536);
  deserializeJson(doc, payload);

  JsonObject root_0 = doc[0];
  String root_0_name = root_0["name"]; // "Relay1"
  ManualMode_value = root_0["value"]; // "0"
  Serial.print("Component Name: ");
  Serial.print(root_0_name);
  Serial.print(" : ");
  Serial.println(ManualMode_value);

  JsonObject root_1 = doc[1];
  String root_1_name = root_1["name"]; // "Relay2"
  V_Led_value = root_1["value"]; // "0"
  Serial.print("Component Name: ");
  Serial.print(root_1_name);
  Serial.print(" : ");
  Serial.println(V_Led_value);
  
  JsonObject root_2 = doc[2];
  String root_2_name = root_2["name"]; // "Relay3"
  V_Fan_value = root_2["value"]; // "0"
  Serial.print("Component Name: ");
  Serial.print(root_2_name);
  Serial.print(" : ");
  Serial.println(V_Fan_value);


}


// ***************************************Function creating For sensor*****************************
int Temperature_Read() {
  Temperature = dht.readTemperature(); //random(0,35);
  Serial.print("Room Temperature: ");
  Serial.print(Temperature);
  Serial.println("°C");
  return Temperature;
}


int Smoke_Read() {
  Smoke = analogRead(Smoke_Sensor) / 45;
  Serial.print("Smoke: ");
  Serial.print(Smoke);
  Serial.println("%");
  return Smoke;
}



// **************************************1*Logic of Controling "Device" From <-- "Server" *****************************
void Control_Device_From_Server() {
  HTTPClient http;
  // Your Domain name with URL path or IP address with path
  http.begin(ActionGet_serverName);
  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.println(payload);
    Json_Parsing();



    //*******************************Manual Mode On*******************************
    if ((ManualMode_value == 0)) {
      Serial.println("---------------------------------");
      Serial.println("ManualMode OFF");
      Serial.println("---------------------------------");
    }
     else if ((ManualMode_value == 1)) {

      ManualMode_state = 1;
      Serial.println("---------------------------------");
      Serial.println("ManualMode ON");
      Serial.println("---------------------------------");
      
      //----------Led*******************************

      if (V_Led_value == 1) {
        digitalWrite(Led, LOW);
        Led_state = 1;
        Serial.println("Led ON");
        
      }
      else if (V_Led_value == 0) {
        digitalWrite(Led, HIGH);
        Led_state = 0;
        Serial.println("Led OFF");
        
        
      }


        //----------Fan********************************
      if (V_Fan_value == 1) {
        digitalWrite(Fan, LOW);
        Fan_state = 1;
        Serial.println("Fan ON");
        
       
      }
      else if (V_Fan_value == 0) {
        digitalWrite(Fan, HIGH);
        Fan_state = 0;
        Serial.println("Fan OFF");
        
      }

    }
    
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

// **************************************2*Logic of Controling "Server" from <-- "Device"*****************************

void Control_Server_From_Device() {
  HTTPClient http;
  http.begin(ActionPost_serverName);

  char buffer[256];
  DynamicJsonDocument doc(256);

  JsonArray actions = doc.createNestedArray("actions");
  JsonObject actions_0 = actions.createNestedObject();
  actions_0["name"] = "ManualMode";
  actions_0["value"] = ManualMode_state;

  JsonObject actions_1 = actions.createNestedObject();
  actions_1["name"] = "Led";
  actions_1["value"] = Led_state;
  
JsonObject actions_2 = actions.createNestedObject();
  actions_2["name"] = "Fan";
  actions_2["value"] = Fan_state;

  serializeJson(doc, buffer);
  Serial.println(String(buffer));

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");
  String httpRequestData = buffer;
  int httpResponseCode = http.POST(httpRequestData);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
}

  // **************************************3*Logic of Sending Sensor Data to the Server[{Trigger}] *****************************
void Sensors_Data_triger_toThe_Server() {
  
  HTTPClient http;
  http.begin(TemperaturePost_serverName);
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  char buffer[32];
  DynamicJsonDocument doc0(32);
  JsonObject data = doc0.createNestedObject("data");
  Temperature_Read();
  data["Temperature"] = Temperature;
  serializeJson(doc0, buffer);
  Serial.print(buffer);

   String httpRequestData = buffer;
   int httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();

//+++++++++++++++++++++++++++++++++++++++SmokePost_serverName++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  http.begin(SmokePost_serverName);
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  buffer[32];
  DynamicJsonDocument doc2(32);
  data = doc2.createNestedObject("data");
  Smoke_Read();
  data["Smoke"] = Smoke;
  serializeJson(doc2, buffer);
  Serial.print(buffer);

  httpRequestData = buffer;
  httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
//++++++++++++++++++++++++++++++++++++++++++++++++end+++++++++++++++++++++++++++++++++++++++++++++++++++
}
//---------------------------------------------OTA----------------------------------------------------------------------------------------------------
void reconnect() {
    int i = 0;
    // Loop until we're reconnected
    status = WiFi.status();
    if (status != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            if (i == 10) {
                ESP.restart();
            }
            i++;
        }
        Serial.println("Connected to AP");
    }
}

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
    String FirmwareURL = "";
    FirmwareURL += URL_fw_Version; // Requesting the firmware URL
    FirmwareURL += "?";
    FirmwareURL += String(rand());
    Serial.println(FirmwareURL);
    WiFiClientSecure * client = new WiFiClientSecure;

    if (client) {
        client -> setCACert(rootCACertificate);
        HTTPClient https;

        if (https.begin(FirmwareURL)) {
            Serial.print("[HTTPS] GET...\n");
            // start connection and send HTTP header
            delay(100);
            httpCode = https.GET();
            delay(100);
            if (httpCode == HTTP_CODE_OK) // if version received
            {
                payload = https.getString(); // save received version
            } else {
                Serial.print("Error Occured During Version Check: ");
                Serial.println(httpCode);
            }
            https.end();
        }
        delete client;
    }

    if (httpCode == HTTP_CODE_OK) // if version received
    {
        payload.trim();
        if (payload.equals(FirmwareVer)) {
            Serial.printf("\nDevice  IS Already on Latest Firmware Version:%s\n", FirmwareVer);
            return 0;
        } else {
            Serial.println(payload);
            Serial.println("New Firmware Detected");
            return 1;
        }
    }
    return 0;
}
//-------------------------------------END---OTA----------------------------------------------------------------------------------------------------


void loop() {
 
 currentTime= millis();
 
  if (Serial.available() > 0) {
        incomingByte = Serial.read();
        if (incomingByte == 'U') { // if I press 'U' it will checkout firmware version
            Serial.println("Firmware Update In Progress..");
            if (FirmwareVersionCheck()) {// If the version is not same then it will return 1 and execute update
                firmwareUpdate();
            }
        }
    }
    
 Serial.print(" Active Firmware Version:");
 Serial.println(FirmwareVer);

 //---------------------------------------------Loop1--------------------
 if(currentTime - previousTime <timeForTrigerSensorData){
      if (WiFi.status() == WL_CONNECTED) {
        Control_Device_From_Server();
        Control_Server_From_Device();
       // digitalWrite(relay4, LOW);
       
        
    
        
      }
      else {
        Serial.println("WiFi Disconnected");
      }
  
      //**************************OLED Display********************
      Temperature_Read();
      Smoke_Read();
      if (Smoke >= 18) {
        digitalWrite(Buzzer, LOW);
      }
      else {
        digitalWrite(Buzzer, HIGH);
      }
      display.clearDisplay();
      display.setRotation(2);
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.print("   Temp   ");
      display.print("   ");
      display.print(Temperature);
      display.print("'C ");
      display.print("     Smoke  ");
  
      display.print("    ");
      display.print(Smoke);
      display.print("% ");
      display.display();

 }

 //---------------------------------------------Loop2--------------------
 else if(currentTime - previousTime >timeForTrigerSensorData){
   if (WiFi.status() == WL_CONNECTED) {
    Sensors_Data_triger_toThe_Server();
    Serial.println("+++++++++++++++++++++++++++++++++++");
    Serial.println("Trigger Mode On");
    Serial.println("+++++++++++++++++++++++++++++++++++");
    delay(5000);
    previousTime=currentTime;
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  
  
 }
}
