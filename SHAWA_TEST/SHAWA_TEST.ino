// ************** Showa Sensor + App  ************

// All Library function

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "cert.h"
#include "FreqCountESP.h"
#include <Arduino.h>
//Variables Decalration


int timeout = 120;  // seconds to run for
int i = 0;
int statusCode;
const char* ssid = "ANTT SOFT_ROOM";
const char* password = "AnttRoboticsLtd123";
const char* serverName = "http://www.showabackend-env-1.eba-kai5b5bn.ap-northeast-1.elasticbeanstalk.com/admin/iot/update-sensor-data";
#define URL_fw_Version "https://raw.githubusercontent.com/anttroboticsltd/firmware-update/main/SHAWA_TEST/fw_Version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/anttroboticsltd/firmware-update/main/SHAWA_TEST/SHAWA_TEST.bin"  //state url of your firmware image

//Firmware Current Version
String FirmwareVer = {
  "1.1"
};

String st;
String content;
String esid;
String epass = "";
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 5000;  // interval after every data send


// sensor pin number

int inputPin = 36;
int timerMs = 1000;


#define ntc_pin 34  // Pin,to which the voltage divider is connected

#define vd_power_pin 17  // 3.3V for the voltage divider

#define nominal_resistance 10000  //Nominal resistance at 25⁰C


#define nominal_temeprature 25  // temperature for nominal resistance (almost always 25⁰ C)


#define samplingrate 5  // Number of samples


#define beta 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.


#define Rref 10000  //Value of  resistor used for the voltage divider


int samples = 0;  //array to store the samples


// A structure that stores sensor data

struct SensorData {

  double temperature;

  String mac;

  String uid;

  String id;

  int vibration;
};


void Send_Data_To_Server() {
  // sensor data read

  SensorData sensorData;


  uint8_t i;


  float average;
  float temperature;

  samples = 0;

  digitalWrite(vd_power_pin, HIGH);

  for (i = 0; i < samplingrate; i++) {


    samples += analogRead(ntc_pin);


    delay(10);
  }

  digitalWrite(vd_power_pin, LOW);

  average = 0;


  average = samples / samplingrate;
  // Calculate NTC resistance


  average = 4096 / average - 1;


  average = Rref / average;
  temperature = average / nominal_resistance;  // (R/Ro)


  temperature = log(temperature);  // ln(R/Ro)


  temperature /= beta;  // 1/B * ln(R/Ro)


  temperature += 1.0 / (nominal_temeprature + 273.15);  // + (1/To)


  temperature = 1.0 / temperature;  // Invert


  temperature -= 273.15;  // convert absolute temp to C

  sensorData.temperature = temperature;

  sensorData.id = "01";

  sensorData.uid = "test01";

  sensorData.mac = WiFi.macAddress();
  Serial.print("ESP mac :");
  Serial.println(sensorData.mac);
  uint32_t frequency = (FreqCountESP.read()/100);
  sensorData.vibration = frequency;

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
  doc["temperature2"] = 123;
  doc["temperature3"] = 123;
  doc["temperature4"] = 123;
  doc["temperature5"] = 123;
  doc["temperature6"] = 123;
  doc["vibration1"] = sensorData.vibration;
  doc["vibration2"] = 123;
  doc["vibration3"] = 123;
  doc["vibration4"] = 123;
  doc["vibration5"] = 123;
  doc["vibration6"] = 123;
  serializeJson(doc, buffer);
  Serial.print(buffer);

  String httpRequestData = buffer;
  int httpResponseCode = http.POST(httpRequestData);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
}

void connect_wifi() {
  Serial.println("Waiting for WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
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
  WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once:
  Serial.begin(115200);
  FreqCountESP.begin(inputPin, timerMs);
  Serial.println("\n Starting");
  pinMode(vd_power_pin, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  if (Serial.available()) {
    char x = Serial.read();
    if (x == 'U') {
      Serial.print("Active firmware version:");
      Serial.println(FirmwareVer);
      connect_wifi();

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("wifi connected");
        if (FirmwareVersionCheck()) {
          firmwareUpdate();
        } else {
          Serial.print("Active firmware version:");
          Serial.println(FirmwareVer);
          ESP.restart();
        }
      } else {
        connect_wifi();
      }
    } /*************Firmware Update section end*****************/
    else if (x == 'W') {
        WiFiManager wm;

        //reset settings - for testing
        //wm.resetSettings();

        // set configportal timeout
        wm.setConfigPortalTimeout(timeout);

        if (!wm.startConfigPortal("SHOWA_COM")) {
          Serial.println("failed to connect and hit timeout");
          delay(3000);
          //reset and try again, or maybe put it to deep sleep
          ESP.restart();
          delay(5000);
        }

        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
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
    Serial.println("Connection Status Negative. Creating Hotspot...");
    WiFiManager wm;

        //reset settings - for testing
        //wm.resetSettings();

        // set configportal timeout
        wm.setConfigPortalTimeout(timeout);

        if (!wm.startConfigPortal("SHOWA_COM")) {
          Serial.println("failed to connect and hit timeout");
          delay(3000);
          //reset and try again, or maybe put it to deep sleep
          ESP.restart();
          delay(5000);
        }

        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
  }
}