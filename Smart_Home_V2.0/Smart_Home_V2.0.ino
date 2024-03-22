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

int timeForTrigerSensorData=600000;//******************************----------------------------------------

const char* ssid       = "ANTT Robotics TP";
const char* password   = "digi@anti@123";

const char* ActionGet_serverName        = "https://iotsnacksbox.io/api/actions?snacksboxtoken=fb02004eeae750fa02105d953431eb9abdbf38aa1683d33fe68b00d368086b88" ;
const char* ActionPost_serverName       = "https://iotsnacksbox.io/api/action?snacksboxtoken=fb02004eeae750fa02105d953431eb9abdbf38aa1683d33fe68b00d368086b88" ;
const char* TemperaturePost_serverName  = "https://iotsnacksbox.io/api/trigger/Temperature?snacksboxtoken=fb02004eeae750fa02105d953431eb9abdbf38aa1683d33fe68b00d368086b88" ;//19bbfd0ba4cac58dd638f09a5572e26b1a028ba4647507ddc101c9f6deea81e3
const char* SmokePost_serverName        = "https://iotsnacksbox.io/api/trigger/Smoke?snacksboxtoken=19bbfd0ba4cac58dd638f09a5572e26b1a028ba4647507ddc101c9f6deea81e3" ;


String payload ;

#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int ManualMode_value;
int ManualMode_state;

//Temp pin No 25
int Led = 26;//4
int Fan = 27;//2
int Smoke_Sensor = 35;

int Buzzer = 15;//tx2
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
  
  WiFi.begin(ssid, password);
  Serial.begin(115200);
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

  dht.begin();

  pinMode(Led, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(Smoke_Sensor, INPUT);
  pinMode(LDR_Sensor, INPUT);

  digitalWrite(Led, HIGH);
  digitalWrite(Buzzer, HIGH);
  digitalWrite(Fan, HIGH);

}






// **************************************1* {GET Request data needs to control device from Server Switch}*********************************************************************************************************
void Control_Device_From_Server() {
  HTTPClient http;
  // Your Domain name with URL path or IP address with path
  http.begin(ActionGet_serverName);
  // Send HTTP GET request
  int httpResponseCode = http.GET();// JSON DATA from the Server

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.println(payload);

    Json_Parsing();

    //----------Manual Mode On************************Server Switch 0
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
      
      //----------Led*******************************  Server Switch 1

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

       //----------Fan********************************Server Switch 2
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

// ***************************************Convert the JSON into the Arduino analyzable data that we will get from server   *****************************
void Json_Parsing() {
  // Parsing the JSON using https://arduinojson.org/v6/assistant/
  Serial.print("Payload/Json from Server:");
  Serial.print("JSON from derber by HTTP GET req");
  Serial.println(payload);

  DynamicJsonDocument doc(1536); //DynamicJsonDocument is a JsonDocument that allocates its memory pool in the heap. So for the Json paylod ESP32 will allocate 1536 Bytes in Memory(Heap)
  deserializeJson(doc, payload); //When we create a 1536Bytes JSON Document after that deserializeJson parses(analyze the JSON paylod into its parts) a JSON input and puts the result( copy then past it in the doc) in a JsonDocument.

  JsonObject root_0 = doc[0];
  String root_0_name = root_0["name"]; // "Relay1"
  ManualMode_value = root_0["value"]; // "0"
  
  Serial.print("Component Name: ");
  Serial.print(root_0_name);
  Serial.print(" : ");
  Serial.println(ManualMode_value);
  //..................................
  JsonObject root_1 = doc[1];
  String root_1_name = root_1["name"]; // "Relay2"
  V_Led_value = root_1["value"]; // "0"
  
  Serial.print("Component Name: ");
  Serial.print(root_1_name);
  Serial.print(" : ");
  Serial.println(V_Led_value);
  //..................................  
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
  Temperature =random(1,40); //dht.readTemperature();
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

// **************************************2*Logic of Controling "Server" from <-- "Device"{POST Request Send the Devive Load Status(O/F) to the Server }*****************************

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


void loop() {
 
 currentTime= millis();
 //---------------------------------------------Loop1--------------------
 if(currentTime - previousTime <timeForTrigerSensorData){
      if (WiFi.status() == WL_CONNECTED) {
        Control_Device_From_Server();
        Control_Server_From_Device();
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
