// All codes are copyrighted these lines shouldn't be removed
// Codes by Shivam - https://github.com/ShivamJoker/Easy-Home-Automation-with-ESP8266
// Copyright (c) 2018 Shivam

#include <Arduino.h>
#include "PCF8574.h"
// Add wifi library
#include <WiFi.h>
// #include <WiFi.h> // for esp32
#include <EEPROM.h> // include eeprom for storage
// #include <ESPmDNS.h> // added for local mdns (homeauto.local)

uint8_t n = 0;

unsigned long check_wifi = 30000;
const char* ssid     = "ANTT SOFT_ROOM";
const char* password = "AnttRoboticsLtd123";
PCF8574 pcf(0x38);
// Define relay pins for switching things
int LED1 = P0,
    LED2 = P1,
    fan = P2,
    // your device = pin number,
    socket = P3;

WiFiServer server(80);
// Start Wifi Server on port 80

void setup()
{
    Serial.begin(115200); // start serial connection
    Serial.println("Welcome To Easy Home Automation");
    // Print Welcome message
    EEPROM.begin(64);
    // Start eeprom connection
    WiFi.enableSTA(true);
    delay(50);

    // Set digital pins to output;
    pcf.pinMode(LED1, OUTPUT);
    pcf.pinMode(LED2, OUTPUT);
    pcf.pinMode(fan, OUTPUT);
    pcf.pinMode(socket, OUTPUT);
    // pinMode(your device, OUTPUT);
    Serial.print("Init pcf8574...");
	  if (pcf.begin()){
		Serial.println("OK");
	  }else{
		Serial.println("KO");
	  }

    // Get data from eeprom and set pins to high/low

    pcf.digitalWrite(LED1,EEPROM.read(0));
    pcf.digitalWrite(LED2,EEPROM.read(1));
    pcf.digitalWrite(fan,EEPROM.read(2));
    pcf.digitalWrite(socket,EEPROM.read(3));
    // digitalWrite(device,EEPROM.read(adress));

    // We start by connecting to a WiFi network
    WiFi.begin(ssid, password);

    delay(3500);
    // wait for 3.5 sec

    // If wifi is connected then show message with device ip

   if (WiFi.status() == WL_CONNECTED){
     Serial.println("");
     Serial.print("WiFi connected to - ");
     Serial.print(ssid);
     Serial.println("");
     Serial.print("IP address: ");
     Serial.print(WiFi.localIP()); // Local ip of esp32
   }
   // if (!MDNS.begin("homeauto")) {
   //      Serial.println("Error setting up MDNS responder!");
   //  }
   

   server.begin();


}

int value = 0;

void loop(){

// if wifi is down, try reconnecting every 30 seconds
if ((WiFi.status() != WL_CONNECTED) && (millis() > check_wifi)) {
  Serial.println();
  Serial.println("Wifi Not Connected");
  Serial.print("Reconnecting to ");
  Serial.print(ssid);
  WiFi.reconnect();
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Local ip of esp32
  check_wifi = millis() + 30000;
}

 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        // Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            // Our client HTML code starts here which is in compressed format with no double semicolons
            client.print("<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <meta http-equiv='X-UA-Compatible' content='ie=edge, chrome=1'> <meta name='mobile-web-app-capable' content='yes'> <meta name='theme-color' content='#2f3640'> <meta name='apple-mobile-web-app-status-bar-style' content='black-translucent'> <link href='https://fonts.googleapis.com/css?family=Rubik' rel='stylesheet'> <link rel='icon' href='http://creativeshi.com/favicon.ico' type='image/icon'> <link rel='icon' sizes='256x256' href='http://creativeshi.com/idea.png'> <title>Easy Home Automation - ANTT Robotics Ltd</title></head><style>*{-webkit-touch-callout: none; -webkit-text-size-adjust: none; -webkit-tap-highlight-color: rgba(0, 0, 0, 0); -webkit-user-select: none; user-select: none; box-sizing: border-box; font-family: 'Rubik', sans-serif}html, body{width: 100vw; height: 100vh; margin: 0; padding: 0}body{display: flex; align-content: center; align-items: center; justify-content: center; flex-direction: column; background: #2f3640}.tggl-btn{transition: all 0.3s ease-in; background-color: #ff6b6b; display: inline-block; position: relative; width: 4em; height: 2em; border-radius: 1em; cursor: pointer; padding: 2px;}.tggl-btn:after{content: ''; display: block; position: absolute; background-color: #fff; width: calc(50% - 4px); height: calc(100% - 4px); border-radius: 50%; left: 2px; transition: left 0.2s ease, background-color 0.3s ease;}.tggl-btn.tggl-on:after{left: calc(50% + 2px);}.tggl-btn.tggl-on{background: #1dd1a1;}.box{background: #ffffff14; padding: 20px 30px; border-radius: 18px; box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19)}.switch-wrap{display: flex; align-items: center; justify-content: space-between; width: 300px; padding: 8px}span{font-size: 38px; color: #fff; padding-right: 15px}</style><body> <div class='box'> <div class='switch-wrap'> <span>Side LED</span> <label class='tggl-btn ying'></label></div><div class='switch-wrap'> <span>Front LED</span> <label class='tggl-btn ying'></label></div><div class='switch-wrap'> <span>Fan</span> <label class='tggl-btn ying'></label></div><div class='switch-wrap'> <span>Socket</span> <label class='tggl-btn ying'></label></div><div class='switch-wrap'> <span>All Devices</span> <label class='tggl-btn ying'></label></div></div><script>const allbtn=document.querySelectorAll('.tggl-btn'), allSpans=document.querySelectorAll('span'); allbtn.forEach((el, index)=> el.addEventListener('click', ()=>{if (!el.classList.contains('tggl-on')){fetch('/' + allSpans[index].innerText + '/on');}else{fetch('/' + allSpans[index].innerText + '/off');}el.classList.toggle('tggl-on');})); </script></body></html>");
          
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
          // Serial.println(currentLine);

        }
        // Check to see if the client request was "get device on":
        currentLine.toLowerCase(); // convert requests to lowercase
        if (currentLine.endsWith("get /side%20led/on")) {
          // %20 is for space
          pcf.digitalWrite(LED1, HIGH);
          // if we will make led1 pin to low relay will turn on
          EEPROM.write(0,0);
          // 0 is the eeprom address and one 0 means low
        }
        if (currentLine.endsWith("get /front%20led/on")) {
          pcf.digitalWrite(LED2, HIGH);
          EEPROM.write(1,0);
        }
        if (currentLine.endsWith("get /fan/on")) {
          pcf.digitalWrite(fan, HIGH);
          EEPROM.write(2,0);
        }
        if (currentLine.endsWith("get /socket/on")) {
          pcf.digitalWrite(socket, HIGH);
          EEPROM.write(3,0);
        }
//         if (currentLine.endsWith("get /tv/on")) {
//           digitalWrite(socket, HIGH);
//           EEPROM.write(4,0);
//         }
        // duplicate if statement if you have other Devices
        // if (currentLine.endsWith("get /YourDeviceName/on or off")) {
        //   digitalWrite(your device, LOW or Hight or 0 or 1);
        //   EEPROM.write(adress,0 or 1);
        // }

        if (currentLine.endsWith("get /all%20devices/on")) {
          pcf.digitalWrite(LED1, HIGH);
          pcf.digitalWrite(LED2, HIGH);
          pcf.digitalWrite(fan, HIGH);
          pcf.digitalWrite(socket, HIGH);
          EEPROM.write(0,0);
          EEPROM.write(1,0);
          EEPROM.write(2,0);
          EEPROM.write(3,0);
          // make all devices on
        }


        // check for off request
        if (currentLine.endsWith("get /side%20led/off")) {
          pcf.digitalWrite(LED1, LOW);
          EEPROM.write(0,1);
        }
        if (currentLine.endsWith("get /front%20led/off")) {
          pcf.digitalWrite(LED2, LOW);
          EEPROM.write(1,1);
        }
        if (currentLine.endsWith("get /fan/off")) {
          pcf.digitalWrite(fan, LOW);
          EEPROM.write(2,1);
        }
        if (currentLine.endsWith("get /socket/off")) {
          pcf.digitalWrite(socket, LOW);
          EEPROM.write(3,1);
        }
        // if (currentLine.endsWith("get /tv/off")) {
        //   pcf.digitalWrite(socket, HIGH);
        //   EEPROM.write(4,1);
        // }
        if (currentLine.endsWith("get /all%20devices/off")) {
          pcf.digitalWrite(LED1, LOW);
          pcf.digitalWrite(LED2, LOW);
          pcf.digitalWrite(fan, LOW);
          pcf.digitalWrite(socket, LOW);
          // digitalWrite(tv, LOW);
          EEPROM.write(0,1);
          EEPROM.write(1,1);
          EEPROM.write(2,1);
          EEPROM.write(3,1);
          //EEPROM.write(4,1);
          // make all devices off
        }

      }
    }
    // close the connection:
    EEPROM.commit();
    client.stop();
    Serial.println("Client Disconnected.");
  }
}