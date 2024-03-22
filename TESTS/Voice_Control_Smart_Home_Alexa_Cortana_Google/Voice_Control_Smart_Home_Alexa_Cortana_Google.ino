#include <PCF8574.h>

// Include necessary libraries
#include <WiFi.h>
#include <Espalexa.h>
 
// =================== CONSTANTS ===================
// Define GPIO pins for relays and switches
#define RelayPin1 P0
#define RelayPin2 P1
#define RelayPin3 P2
#define RelayPin4 P3
 
#define SwitchPin1 5
#define SwitchPin2 17
#define SwitchPin3 13
#define SwitchPin4 16
PCF8574 pcf(0x38);
const char* ssid = "ANTT SOFT_ROOM";
const char* password = "AnttRoboticsLtd123";
 
String Device_1_Name = "Relay 1

";
String Device_2_Name = "Relay 2";
String Device_3_Name = "Relay 3";
String Device_4_Name = "Relay 4";
 
// =================== VARIABLES ===================
int toggleState_1 = 0;
int toggleState_2 = 0;
int toggleState_3 = 0;
int toggleState_4 = 0;
boolean wifiConnected = false;
 
// Create an instance of the Espalexa class
Espalexa espalexa;
 
// =================== FUNCTION DECLARATIONS ===================
boolean connectWifi();
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void relayOnOff(int relay);
void addDevices();
 
// =================== SETUP ===================
void setup()
{
  Serial.begin(115200); // Start serial communication
  
  // Initialize pins for relay and switches
  pcf.pinMode(RelayPin1, OUTPUT);
  pcf.pinMode(RelayPin2, OUTPUT);
  pcf.pinMode(RelayPin3, OUTPUT);
  pcf.pinMode(RelayPin4, OUTPUT);
  
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pcf.begin();
  // Ensure all relays are turned off initially
  pcf.digitalWrite(RelayPin1, LOW);
  pcf.digitalWrite(RelayPin2, LOW);
  pcf.digitalWrite(RelayPin3, LOW);
  pcf.digitalWrite(RelayPin4, LOW);
 
  // Try connecting to WiFi
  wifiConnected = connectWifi();
  if (wifiConnected)
  {
    addDevices();
  }
  else
  {
    Serial.println("Cannot connect to WiFi. So in Manual Mode");
    delay(1000);
  }
}
 
// =================== MAIN LOOP ===================
void loop()
{
  // Handle manual switches regardless of WiFi status
  if (digitalRead(SwitchPin1) == LOW)
  {
    delay(200);
    relayOnOff(1);
  }
  else if (digitalRead(SwitchPin2) == LOW)
  {
    delay(200);
    relayOnOff(2);
  }
  else if (digitalRead(SwitchPin3) == LOW)
  {
    delay(200);
    relayOnOff(3);
  }
  else if (digitalRead(SwitchPin4) == LOW)
  {
    delay(200);
    relayOnOff(4);
  }
 
  // Handle WiFi connections
  if (wifiConnected)
  {
    espalexa.loop();
    delay(1);
  }
  else
  {
    wifiConnected = connectWifi(); // Try to reconnect if not connected
    if (wifiConnected)
    {
      addDevices();
    }
  }
}
 
// =================== FUNCTION IMPLEMENTATIONS ===================
boolean connectWifi()
{
  int attempts = 0;
  boolean state = true;
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED && attempts < 20)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }
 
  // Handle successful connection
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    state = false;
    Serial.println("Connection failed.");
  }
 
  return state;
}
 
 
// Functions for each relay to handle toggle operations
void relayOnOff(int relay)
{
  // Based on the relay number, toggle its state
  switch (relay)
  {
    case 1:
      if (toggleState_1 == 0)
      {
        pcf.digitalWrite(RelayPin1, HIGH); // turn on relay 1
        toggleState_1 = 1;
        Serial.println("Device1 ON");
      }
      else
      {
        pcf.digitalWrite(RelayPin1, LOW); // turn off relay 1
        toggleState_1 = 0;
        Serial.println("Device1 OFF");
      }
      delay(100);
      break;
 
    case 2:
      if (toggleState_2 == 0)
      {
        pcf.digitalWrite(RelayPin2, HIGH); // turn on relay 2
        toggleState_2 = 1;
        Serial.println("Device2 ON");
      }
      else
      {
        pcf.digitalWrite(RelayPin2, LOW); // turn off relay 2
        toggleState_2 = 0;
        Serial.println("Device2 OFF");
      }
      delay(100);
      break;
 
    case 3:
      if (toggleState_3 == 0)
      {
        pcf.digitalWrite(RelayPin3, HIGH); // turn on relay 3
        toggleState_3 = 1;
        Serial.println("Device3 ON");
      } else {
        pcf.digitalWrite(RelayPin3, LOW); // turn off relay 3
        toggleState_3 = 0;
        Serial.println("Device3 OFF");
      }
      delay(100);
      break;
 
    case 4:
      if (toggleState_4 == 0)
      {
        pcf.digitalWrite(RelayPin4, HIGH); // turn on relay 4
        toggleState_4 = 1;
        Serial.println("Device4 ON");
      }
      else
      {
        pcf.digitalWrite(RelayPin4, LOW); // turn off relay 4
        toggleState_4 = 0;
        Serial.println("Device4 OFF");
      }
      delay(100);
      break;
    default : break;
  }
}
 
 
// Functions to handle Alexa brightness changes for each device
void firstLightChanged(uint8_t brightness)
{
  // If brightness is set to 255, turn on the relay, otherwise turn it off
  if (brightness == 255)
  {
    pcf.digitalWrite(RelayPin1, HIGH);
    Serial.println("Device1 ON");
    toggleState_1 = 1;
  }
  else
  {
    pcf.digitalWrite(RelayPin1, LOW);
    Serial.println("Device1 OFF");
    toggleState_1 = 0;
  }
}
 
void secondLightChanged(uint8_t brightness)
{
  // If brightness is set to 255, turn on the relay, otherwise turn it off
  if (brightness == 255)
  {
    pcf.digitalWrite(RelayPin2, HIGH);
    Serial.println("Device2 ON");
    toggleState_2 = 1;
  }
  else
  {
    pcf.digitalWrite(RelayPin2, LOW);
    Serial.println("Device2 OFF");
    toggleState_2 = 0;
  }
}
 
void thirdLightChanged(uint8_t brightness)
{
  // If brightness is set to 255, turn on the relay, otherwise turn it off
  if (brightness == 255)
  {
    pcf.digitalWrite(RelayPin3, HIGH);
    Serial.println("Device3 ON");
    toggleState_3 = 1;
  }
  else
  {
    pcf.digitalWrite(RelayPin3, LOW);
    Serial.println("Device3 OFF");
    toggleState_3 = 0;
  }
}
 
void fourthLightChanged(uint8_t brightness)
{
  // If brightness is set to 255, turn on the relay, otherwise turn it off
  if (brightness == 255)
  {
    pcf.digitalWrite(RelayPin4, HIGH);
    Serial.println("Device4 ON");
    toggleState_4 = 1;
  }
  else
  {
    pcf.digitalWrite(RelayPin4, LOW);
    Serial.println("Device4 OFF");
    toggleState_4 = 0;
  }
}
 
 
// Function to add the devices for Alexa control
void addDevices()
{
  // Define the devices and their respective callback functions
  // and start the Espalexa service
  espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off
  espalexa.addDevice(Device_2_Name, secondLightChanged);
  espalexa.addDevice(Device_3_Name, thirdLightChanged);
  espalexa.addDevice(Device_4_Name, fourthLightChanged);
 
  espalexa.begin();
}