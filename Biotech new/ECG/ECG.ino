#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SH1106 display(OLED_SDA, OLED_SCL);


const char* ssid       = "ANTT SOFT_ROOM";
const char* password   = "AnttRoboticsLtd123";

const char* ECG_serverName  = "https://iotsnacksbox.io/api/trigger/ECG?snacksboxtoken=74d5f0f35dac8586f5b3b6d882c49f19c8ab84fc5c3a0968c27b1f8b6f6b47d6" ;

#define ECG_Analog_Pin 33 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define ECG_LO_P_Pin 16 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define ECG_LO_N_Pin 17 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//#define shutdown_pin 13 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<SDN
int ECG_Value = 0;

void setup() {

  WiFi.begin(ssid, password);
  Serial.begin(115200);


  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setRotation(0);
    display.setTextSize(1);
    display.setCursor(0, 28);
    display.print("Connecting to Wifi..");
    display.display();

  }

  display.clearDisplay();
  display.setRotation(0);
  display.setTextSize(1);
  display.setCursor(34, 28);
  display.print("Connected");
  display.display();

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(ECG_LO_P_Pin, INPUT); // Setup for leads off detection LO +

  pinMode(ECG_LO_N_Pin, INPUT); // Setup for leads off detection LO -
}


// ***************************************Function creating For sensor*****************************
void ECG() {
  if ((digitalRead(16) == 1) || (digitalRead(17) == 1)) { //check if leads are removed
    Serial.println("leads off!");
  }
  else {
    ECG_Value = analogRead(33);
  }
  delay(10);
}


// **************************************2*Logic of Controling "Server" from <-- "Device"*****************************

  //++++++++++++++++++++++++++++++++++++++ECG_serverName+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  void ECG_Rate_Post() {

    HTTPClient http;
    http.begin(ECG_serverName);
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Content-Type", "application/json");

    char buffer[62];
    DynamicJsonDocument doc1(62);
    JsonObject data = doc1.createNestedObject("data");

    data["ECG_Rate"] = ECG_Value;
    serializeJson(doc1, buffer);
    Serial.print(buffer);

    String httpRequestData = buffer;
    int httpResponseCode = http.POST(httpRequestData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();

  }


  void loop() {

    if (WiFi.status() == WL_CONNECTED) {
      ECG();
      display.clearDisplay();
      display.setRotation(0);
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("   ECG ");
      display.println("Testing....");
      display.display();
      ECG_Rate_Post();
    }
    else {
      Serial.println("WiFi Disconnected");
    }

  }
