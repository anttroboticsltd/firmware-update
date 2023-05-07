// ************** Showa Sensor + App  ************

// All Library function

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "HttpsOTAUpdate.h"
#include "cert.h"
//Variables Decalration
HTTPClient http;
// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

int timeout = 120; // seconds to run for
int i = 0;
int statusCode;
const char* ssid = "ANTT SOFT_ROOM";
const char* password = "AnttRoboticsLtd123";
const char* serverName = "http://www.showabackend-env-1.eba-kai5b5bn.ap-northeast-1.elasticbeanstalk.com/admin/iot/update-sensor-data";
static const char *url = "https://raw.githubusercontent.com/anttroboticsltd/firmware-update/main/SHAWA_TEST/SHAWA_TEST.bin"; //state url of your firmware image

String st;
String content;
String esid;
String epass = "";


// sensor pin number

#define VIBRATION_SENSOR_PIN_1 A0

#define TEMPERATURE_SENSOR_PIN_1 34


// A structure that stores sensor data

struct SensorData {

  double temperature;

  String mac;

  String uid;

  String id;

  int vibration;
};



static HttpsOTAStatus_t otastatus;

void HttpEvent(HttpEvent_t *event)
{
  switch (event->event_id) {
    case HTTP_EVENT_ERROR:
      Serial.println("Http Event Error");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      Serial.println("Http Event On Connected");
      break;
    case HTTP_EVENT_HEADER_SENT:
      Serial.println("Http Event Header Sent");
      break;
    case HTTP_EVENT_ON_HEADER:
      Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      break;
    case HTTP_EVENT_ON_FINISH:
      Serial.println("Http Event On Finish");
      break;
    case HTTP_EVENT_DISCONNECTED:
      Serial.println("Http Event Disconnected");
      
      break;
  }
}
void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
 WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    HttpsOTA.onHttpEvent(HttpEvent);
}


void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    if (Serial.available()>0){
    char x = Serial.read();
    while ( x == 'U'){
      
  Serial.println("Starting OTA");
  HttpsOTA.begin(url, server_certificate);

  Serial.println("Please Wait it takes some time ...");
  delay(1000);
  break;
    }
  otastatus = HttpsOTA.status();
  
  if (otastatus == HTTPS_OTA_SUCCESS) {
    Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
    
  } else if (otastatus == HTTPS_OTA_FAIL) {
    Serial.println("Firmware Upgrade Fail");
  }
    
  }
    Serial.println();
    Serial.println("Connection status positive");
    delay(500);

    // sensor data read

    SensorData sensorData;

    sensorData.temperature = analogRead(TEMPERATURE_SENSOR_PIN_1);

    sensorData.id = "01";

    sensorData.uid = "test01";

    sensorData.mac = WiFi.macAddress();
    Serial.print("ESP mac :");
    Serial.println(sensorData.mac);

    sensorData.vibration = analogRead(VIBRATION_SENSOR_PIN_1);

    // json encode to encode sensor data into JSON




    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Content-Type", "application/json");

    char buffer[256];
    DynamicJsonDocument doc(256);
    

    doc["macId"] = "08:3a:f2:72:f2:00";
   // doc["uid"] = sensorData.uid;
   // doc["id"] = sensorData.id;
    doc["temperature1"] = sensorData.temperature;
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



    // wait 30 seconds

    delay(30000);


    return;
  } else {
    Serial.println("Connection Status Negative. Press 0");
    // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW) {
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

}


