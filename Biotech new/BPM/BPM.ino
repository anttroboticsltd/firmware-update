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
unsigned long previousTime = 0; // for calculateing time

const char* ssid       = "ANTT SOFT_ROOM";
const char* password   = "AnttRoboticsLtd123";

const char* BPM_serverName   = "https://iotsnacksbox.io/api/trigger/BPM?snacksboxtoken=74d5f0f35dac8586f5b3b6d882c49f19c8ab84fc5c3a0968c27b1f8b6f6b47d6" ;

#include "MAX30100_PulseOximeter.h"
PulseOximeter pox;
int BPM_Value = 0;
int BPM_Max_Value = 0;

//***************************************4. BPM_Value_Post***************************************
void BPM_Value_Post() {

  HTTPClient http;
  http.begin(BPM_serverName);
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  char buffer[62];
  DynamicJsonDocument doc0(62);
  JsonObject data = doc0.createNestedObject("data");

  data["BPM"] = BPM_Max_Value;
  serializeJson(doc0, buffer);
  Serial.print(buffer);

  String httpRequestData = buffer;
  int  httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
  delay(50);
}

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
  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
    //pox.setOnBeatDetectedCallback(onBeatDetected);
  }

}
void loop() {

  pox.update();
  if (WiFi.status() == WL_CONNECTED) {
    pox.update();
    pox.getHeartRate();

    if (pox.getHeartRate() >= BPM_Max_Value) {
      BPM_Max_Value = pox.getHeartRate();
    }
    Serial.print("Max Heart rate:");
    Serial.println(BPM_Max_Value);

    display.clearDisplay();
    display.setRotation(0);
    display.setTextSize(2);

    display.setCursor(0, 0);
    display.print("BPM");
    display.print("=");
    int bpm_value= pox.getHeartRate();
    display.println(bpm_value);
    display.println("");
    display.print("MaxBPM");
    display.print("=");
    display.print(BPM_Max_Value);
    display.display();
    
    if ( millis() - previousTime > 20000) {
      BPM_Value_Post();
      previousTime = millis();
      BPM_Max_Value = 0;
      pox.begin();
      
    }
    

  }
  else {
    Serial.println("WiFi Disconnected");
  }



}
