#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int timeout = 120;  // seconds to run for
int i = 0;
int statusCode;

const char* apiEndpoint = "http://api-v2.showaapp.com/api/v2/sensor-module-attached/add-sensor-data?macAddress=AC:67:B2:09:02:F4";
#define URL_fw_Version "https://raw.githubusercontent.com/anttroboticsltd/firmware-update/main/SHAWA_FW/fw_Version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/anttroboticsltd/firmware-update/main/SHOWA_FW/AC_67_B2_09_02_F4_Module4.ino.bin"  //state url of your firmware image

//Firmware Current Version
String FirmwareVer = {
  "2.1"
};

String st;
String content;
String esid;
String epass = "";
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 500;  // interval after every data send



#define v1 39
#define v2 36
#define v3 34
#define v4 35
#define v5 32
#define v6 33
#define t1 25
#define t2 26
#define t3 27
#define t4 23
#define t5 16
#define t6 17

#define ONE_WIRE_BUS1 t1

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire1(ONE_WIRE_BUS1);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensor1(&oneWire1);

#define ONE_WIRE_BUS2 t2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire2(ONE_WIRE_BUS2);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensor2(&oneWire2);

#define ONE_WIRE_BUS3 t3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire3(ONE_WIRE_BUS3);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensor3(&oneWire3);

#define ONE_WIRE_BUS4 t4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire4(ONE_WIRE_BUS4);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensor4(&oneWire4);

#define ONE_WIRE_BUS5 t5

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire5(ONE_WIRE_BUS5);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensor5(&oneWire5);

#define ONE_WIRE_BUS6 t6

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire6(ONE_WIRE_BUS6);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensor6(&oneWire6);



int vib1;
int vib2;
int vib3;
int vib4;
int vib5;
int vib6;
float temp1;
float temp2;
float temp3;
float temp4;
float temp5;
float temp6;


const char * rootCACertificate = 
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
  "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
  "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
  "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
  "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
  "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
  "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
  "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
  "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
  "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
  "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
  "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
  "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
  "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
  "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
  "-----END CERTIFICATE-----";


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
  pinMode(vib1,INPUT_PULLDOWN);
  pinMode(vib2,INPUT_PULLDOWN);
  pinMode(vib3,INPUT_PULLDOWN);
  pinMode(vib4,INPUT_PULLDOWN);
  pinMode(vib5,INPUT_PULLDOWN);
  pinMode(vib6,INPUT_PULLDOWN);
  pinMode(temp1,INPUT);
  pinMode(temp2,INPUT);
  pinMode(temp3,INPUT);
  pinMode(temp4,INPUT);
  pinMode(temp5,INPUT);
  pinMode(temp6,INPUT);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(100);
  sensor1.begin();
  sensor2.begin();
  sensor3.begin();
  sensor4.begin();
  sensor5.begin();
  sensor6.begin();
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
  Serial.println("connected to :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}

int vibra_Data(){
  int vibrate1 = analogRead(v1);
  vib1 = map(vibrate1, 0, 4096, 0 , 100);
    int vibrate2 = analogRead(v2);
  vib2 = map(vibrate2, 0, 4096, 0 , 100);
    int vibrate3 = analogRead(v3);
  vib3 = map(vibrate3, 0, 4096, 0 , 100);
    int vibrate4 = analogRead(v4);
  vib4 = map(vibrate4, 0, 4096, 0 , 100);
    int vibrate5 = analogRead(v5);
  vib5 = map(vibrate5, 0, 4096, 0 , 100);
    int vibrate6 = analogRead(v6);
  vib6 = map(vibrate6, 0, 4096, 0 , 100);

  return vib1,vib2,vib3,vib4,vib5,vib6;
}

float temp_Data(){
  
  sensor1.requestTemperatures(); 
  
  temp1 = sensor1.getTempCByIndex(0); 
  
  sensor2.requestTemperatures(); 
  
  temp2 = sensor2.getTempCByIndex(0); 
  
    sensor3.requestTemperatures(); 
  
  temp3 = sensor3.getTempCByIndex(0); 

    sensor4.requestTemperatures(); 
  
  temp4 = sensor4.getTempCByIndex(0); 

    sensor5.requestTemperatures(); 
  
  temp5 = sensor5.getTempCByIndex(0); 

    sensor6.requestTemperatures(); 
  
  temp6 = sensor6.getTempCByIndex(0); 


  return temp1,temp2,temp3,temp4,temp5,temp6;

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
    StaticJsonDocument<512> jsonDoc;
    JsonObject sensorData = jsonDoc.createNestedObject("sensorData");
    vibra_Data();
    temp_Data();
    // Add data for vibration sensors
    JsonArray vibrationArray = sensorData.createNestedArray("vibration");
    vibrationArray.add(vib1); // Example pin A0, adjust based on actual pin
    vibrationArray.add(vib2);
    vibrationArray.add(vib3);
    vibrationArray.add(vib4);
    vibrationArray.add(vib5);
    vibrationArray.add(vib6);
    
    // Add data for temperature sensors
    JsonArray temperatureArray = sensorData.createNestedArray("temperature");
    temperatureArray.add(temp1); // Example pin A6, adjust based on actual pin
    temperatureArray.add(temp2);
    temperatureArray.add(temp3);
    temperatureArray.add(temp4);
    temperatureArray.add(temp5);
    temperatureArray.add(temp6);

    // Convert JSON document to string
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);

    HTTPClient http;
    http.begin(apiEndpoint);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.PATCH(jsonString);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error code: " + String(httpResponseCode));
    }
    http.end();
  
  } else {
    Serial.println("WiFi not connected.");
  }

  delay(6000);  // Send data every 1 minute
}
