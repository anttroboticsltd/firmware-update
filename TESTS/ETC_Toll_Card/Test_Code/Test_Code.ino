/*
 Blink led on PIN0
 by Mischianti Renzo <http://www.mischianti.org>

 https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
*/

#include "Arduino.h"
#include "PCF8574.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 1

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 4
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
Adafruit_SH1106 display(21, 22);
// Set i2c address
PCF8574 pcf8574(0x38);

void setup()
{
	Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Clear the buffer.
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Hello Peps!");
  display.display();
  delay(2000);

  // miniature bitmap display
  display.clearDisplay();
  // Set pinMode to OUTPUT
	pcf8574.pinMode(P0, OUTPUT);
  pcf8574.pinMode(P1, OUTPUT);
  pcf8574.pinMode(P2, OUTPUT);
  pcf8574.pinMode(P3, OUTPUT);
  pcf8574.pinMode(P4, OUTPUT);
  pcf8574.pinMode(P5, OUTPUT);
  pcf8574.pinMode(P6, OUTPUT);
  pcf8574.pinMode(P7, OUTPUT);
	Serial.print("Init pcf8574...");
	if (pcf8574.begin()){
		Serial.println("OK");
      // Turn the LED on, then pause
  leds[0] = CRGB::Green;
  FastLED.show();
	}else{
		Serial.println("KO");
      // Turn the LED on, then pause
  leds[0] = CRGB::Red;
  FastLED.show();
	}
  pcf8574.digitalWrite(P0, HIGH);
	delay(100);
	pcf8574.digitalWrite(P0, LOW);
	delay(200);
  	pcf8574.digitalWrite(P1, HIGH);
	delay(100);
	pcf8574.digitalWrite(P1, LOW);
	delay(200);
  	pcf8574.digitalWrite(P2, HIGH);
	delay(100);
	pcf8574.digitalWrite(P2, LOW);
	delay(200);
  	pcf8574.digitalWrite(P3, HIGH);
	delay(100);
	pcf8574.digitalWrite(P3, LOW);
	delay(200);
  	pcf8574.digitalWrite(P4, HIGH);
	delay(100);
	pcf8574.digitalWrite(P4, LOW);
	delay(200);
  	pcf8574.digitalWrite(P5, HIGH);
	delay(100);
	pcf8574.digitalWrite(P5, LOW);
	delay(200);
  	pcf8574.digitalWrite(P6, HIGH);
	delay(100);
	pcf8574.digitalWrite(P6, LOW);
	delay(200);
  	pcf8574.digitalWrite(P7, HIGH);
	delay(100);
	pcf8574.digitalWrite(P7, LOW);
	delay(200);
  pcf8574.digitalWrite(P0, HIGH);
	delay(100);
	pcf8574.digitalWrite(P0, LOW);
	delay(200);
  	pcf8574.digitalWrite(P1, HIGH);
	delay(100);
	pcf8574.digitalWrite(P1, LOW);
	delay(200);
  	pcf8574.digitalWrite(P2, HIGH);
	delay(100);
	pcf8574.digitalWrite(P2, LOW);
	delay(200);
  	pcf8574.digitalWrite(P3, HIGH);
	delay(100);
	pcf8574.digitalWrite(P3, LOW);
	delay(200);
  	pcf8574.digitalWrite(P4, HIGH);
	delay(100);
	pcf8574.digitalWrite(P4, LOW);
	delay(200);
  	pcf8574.digitalWrite(P5, HIGH);
	delay(100);
	pcf8574.digitalWrite(P5, LOW);
	delay(200);
  	pcf8574.digitalWrite(P6, HIGH);
	delay(100);
	pcf8574.digitalWrite(P6, LOW);
	delay(200);
  	pcf8574.digitalWrite(P7, HIGH);
	delay(100);
	pcf8574.digitalWrite(P7, LOW);
	delay(200);

}

void loop()
{
	
}
