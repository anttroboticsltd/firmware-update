// ************** Showa Sensor + App  ************

// All Library function
#include <FS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "cert.h"
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//Variables Decalration


int timeout = 120;  // seconds to run for
int i = 0;
int statusCode;
const char* serverName = "http://www.showabackend-env-1.eba-kai5b5bn.ap-northeast-1.elasticbeanstalk.com/admin/iot/update-sensor-data";
#define URL_fw_Version "https://raw.githubusercontent.com/anttroboticsltd/firmware-update/main/SHAWA_TEST/fw_Version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/anttroboticsltd/firmware-update/main/SHAWA_TEST/SHAWA_TEST.bin"  //state url of your firmware image

//Firmware Current Version
String FirmwareVer = {
  "1.2"
};

String st;
String content;
String esid;
String epass = "";
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 500;  // interval after every data send


// sensor pin number
// Data wire is conntec to the Arduino digital pin 4
int vibPin = 39;
#define ONE_WIRE_BUS 13

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);



// A structure that stores sensor data

struct SensorData {

  float temperature;

  String mac;

  String uid;

  String id;

  int vibration;
};


void Send_Data_To_Server() {
  // sensor data read
   // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  float temp = sensors.getTempCByIndex(0); 
  SensorData sensorData;
  sensorData.temperature = temp;

  sensorData.id = "01";

  sensorData.uid = "test01";

  sensorData.mac = WiFi.macAddress();
  Serial.print("ESP mac :");
  Serial.println(sensorData.mac);
  int vibrate = analogRead(vibPin);
  int vibra = map(vibrate, 0, 4096, 0 , 100);
  sensorData.vibration = vibra;

  // json encode to encode sensor data into JSON



  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  char buffer[256];
  DynamicJsonDocument doc(256);


  doc["macId"] = sensorData.mac;
  // doc["uid"] = sensorData.uid;
  // doc["id"] = sensorData.id;
  doc["temperature1"] = serialized(String(sensorData.temperature,2));
  doc["temperature2"] = 0;
  doc["temperature3"] = 0;
  doc["temperature4"] = 0;
  doc["temperature5"] = 0;
  doc["temperature6"] = 0;
  doc["vibration1"] = sensorData.vibration;
  doc["vibration2"] = 0;
  doc["vibration3"] = 0;
  doc["vibration4"] = 0;
  doc["vibration5"] = 0;
  doc["vibration6"] = 0;
  serializeJson(doc, buffer);
  Serial.print(buffer);

  String httpRequestData = buffer;
  int httpResponseCode = http.POST(httpRequestData);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
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
  WiFiClientSecure* client = new WiFiClientSecure;

  if (client) {
    client->setCACert(rootCACertificate);
    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;
    if (https.begin(*client, fwurl)) {  // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = https.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) {  // if version received
        payload = https.getString();   // save received version
      } else {
        Serial.print("error in downloading version file:");
        Serial.println(httpCode);
      }
      https.end();
    }
    delete client;
  }

  if (httpCode == HTTP_CODE_OK) {  // if version received
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.printf("\nDevice already on latest firmware version:%s\n", FirmwareVer);


      return 0;
    } else {
      Serial.println(payload);
      Serial.println("New firmware detected");

      return 1;
    }
  }
  return 0;
}

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  sensors.begin();
  Serial.println("\n Starting");
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);

  //reset settings - for testing
  //wifiManager.resetSettings();


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("SHOWA_COM")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}




void loop() {

  if (Serial.available()) {
    char x = Serial.read();
    if (x == 'U') {
      Serial.print("Active firmware version:");
      Serial.println(FirmwareVer);
      

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("wifi connected");
        if (FirmwareVersionCheck()) {
          firmwareUpdate();
        } else {
          Serial.print("Active firmware version:");
          Serial.println(FirmwareVer);
          ESP.restart();
        }
      } 
    } /*************Firmware Update section end*****************/
    else if (x == 'R'){
      WiFiManager wifiManager;
      wifiManager.resetSettings();
      Serial.println("WiFi Reseted. Configure again.");
      delay(500);
      ESP.restart();
    }
  }


  if (WiFi.status() == WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Send_Data_To_Server();
      previousMillis = currentMillis;
    }


  }

  else {
    Serial.println("Connection Status Negative. Restarting..");
    delay(1000);
    ESP.restart();
  }
}