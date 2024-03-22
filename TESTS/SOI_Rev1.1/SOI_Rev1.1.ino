#include <FastLED.h>
#include "Arduino.h"
#include "PCF8574.h"

// Set i2c address
PCF8574 pcf8574(0x38);
// How many leds in your strip?
#define NUM_LEDS 1

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 2
#define buzzer 4
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    Serial.begin(115200);
	  delay(2000);
	  pcf8574.pinMode(P0, OUTPUT);
    pcf8574.pinMode(P1, OUTPUT);
    pinMode(buzzer,OUTPUT);
    
    Serial.print("Init pcf8574...");
	  if (pcf8574.begin()){
		Serial.println("OK");
	  }else{
		Serial.println("KO");
	  }
    pcf8574.digitalWrite(P0, HIGH);
	  delay(1000);
	  pcf8574.digitalWrite(P0, LOW);
	  delay(2000);
    pcf8574.digitalWrite(P1, HIGH);
	  delay(1000);
	  pcf8574.digitalWrite(P1, LOW);
	  delay(2000);
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(2000);
    

    // Turn the LED on, then pause
  leds[0] = CRGB::Green;
  FastLED.show();
    
}

void loop() { 
  
  
}
