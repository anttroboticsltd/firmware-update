#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <PZEM004Tv30.h>

#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

#define PZEM_SERIAL Serial2
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);


const char* ssid = "ANTT SOFT_ROOM";
const char* password = "AnttRoboticsLtd123";

const char* Voltage_Server    = "https://iotsnacksbox.io/api/trigger/Voltage?snacksboxtoken=3de8c4be03ae4c7c01bac8b95ab2d27d9aa66837decbc1e5cab5cadfa877b463" ;
const char* Current_Server    = "https://iotsnacksbox.io/api/trigger/Current?snacksboxtoken=3de8c4be03ae4c7c01bac8b95ab2d27d9aa66837decbc1e5cab5cadfa877b463" ;
const char* Power_Server      = "https://iotsnacksbox.io/api/trigger/Power?snacksboxtoken=3de8c4be03ae4c7c01bac8b95ab2d27d9aa66837decbc1e5cab5cadfa877b463" ;
const char* Energy_Server     = "https://iotsnacksbox.io/api/trigger/Energy?snacksboxtoken=3de8c4be03ae4c7c01bac8b95ab2d27d9aa66837decbc1e5cab5cadfa877b463" ;
const char* Frequency_Server  = "https://iotsnacksbox.io/api/trigger/Frequency?snacksboxtoken=3de8c4be03ae4c7c01bac8b95ab2d27d9aa66837decbc1e5cab5cadfa877b463" ;
const char* PF_Server         = "https://iotsnacksbox.io/api/trigger/PFF?snacksboxtoken=3de8c4be03ae4c7c01bac8b95ab2d27d9aa66837decbc1e5cab5cadfa877b463" ;

float volt;
float curr;
float pwr;
float enrg;
float freq;
float pf;
char voltage[10];
char current[10];
char power[10];
char energy[10];
char frequency[10];
char pff[10];

void setup() {
  Serial.begin(115200);
  

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // Print the custom address of the PZEM
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);


}

float Energy_Data(){

  
    volt = pzem.voltage();
    curr = pzem.current();
    pwr = pzem.power();
    enrg = pzem.energy();
    freq = pzem.frequency();
    pf = pzem.pf();
    sprintf(voltage, "%.3f", volt);
    sprintf(current, "%.3f", curr);
    sprintf(power, "%.3f", pwr);
    sprintf(energy, "%.3f", enrg);
    sprintf(frequency, "%.3f", freq);
    sprintf(pff, "%.3f", pf);
    // // Check if the data is valid
    // if(isnan(voltage)){
    //     voltage = 0.000;
    // } else if (isnan(current)) {
    //     current = 0.000;
    // } else if (isnan(power)) {
    //     power = 0.000;
    // } else if (isnan(energy)) {
    //     energy = 0.000;
    // } else if (isnan(frequency)) {
    //     frequency = 0.000;
    // } else if (isnan(pf)) {
    //     pf = 0.000;
    // } 
    return voltage,current,power,energy,frequency,pf;
}

void Control_Server_From_Device() {
  HTTPClient http;

  Energy_Data();

  http.begin(Voltage_Server);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  char buffer[32];
  DynamicJsonDocument doc0(32);
  JsonObject data = doc0.createNestedObject("data");
  data["Voltage"] = voltage;
  serializeJson(doc0, buffer);
  Serial.print(buffer);

  String httpRequestData = buffer;
  int httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();



  http.begin(Current_Server);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  buffer[32];
  DynamicJsonDocument doc1(32);
  data = doc1.createNestedObject("data");
  data["Current"] = current;
  serializeJson(doc1, buffer);
  Serial.print(buffer);

  httpRequestData = buffer;
  httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();

  http.begin(Power_Server);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  buffer[32];
  DynamicJsonDocument doc2(32);
  data = doc2.createNestedObject("data");
  data["Power"] = power;
  serializeJson(doc2, buffer);
  Serial.print(buffer);

  httpRequestData = buffer;
  httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();



  http.begin(Energy_Server);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  buffer[32];
  DynamicJsonDocument doc3(32);
  data = doc3.createNestedObject("data");
  data["Energy"] = energy;
  serializeJson(doc3, buffer);
  Serial.print(buffer);

  httpRequestData = buffer;
  httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();

  http.begin(Frequency_Server);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  buffer[32];
  DynamicJsonDocument doc4(32);
  data = doc4.createNestedObject("data");
  data["Frequency"] = frequency;
  serializeJson(doc4, buffer);
  Serial.print(buffer);

  httpRequestData = buffer;
  httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();

   http.begin(PF_Server);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Content-Type", "application/json");

  buffer[32];
  DynamicJsonDocument doc5(32);
  data = doc5.createNestedObject("data");
  data["PFF"] = pff;
  serializeJson(doc5, buffer);
  Serial.print(buffer);

  httpRequestData = buffer;
  httpResponseCode = http.POST(httpRequestData);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();






}



void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    
    Control_Server_From_Device();
    delay(60000);
  }
  else {
    Serial.println("WiFi Disconnected");
  }


}
