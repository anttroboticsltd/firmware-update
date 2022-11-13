#include"Edu_bot_esp32.h"

CRGB leds[NUM_LEDS];

void Led_init() {
  FastLED.addLeds<WS2812, LED, GRB>(leds, NUM_LEDS);
  /*
    GREEN: (0, 120, 0)
    RED:   (255, 0, 0)
    BLUE:  (0, 0, 255)
  */
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
  delay(400);
  leds[0] = CRGB(0, 255, 0);
  FastLED.show();
  delay(400);
  leds[0] = CRGB(0, 0, 255);
  FastLED.show();
  delay(400);
  leds[0] = CRGB(0, 0, 0);
  FastLED.show();
  Serial.println("Led Initialize.");
}

void Led_On() {
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
}

void Led_Off() {
  leds[0] = CRGB(0, 0, 0);
  FastLED.show();
}

void Led_On_Delay(int Delay) {
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
  delay(1000 * Delay);
}

void Led_Off_Delay(int Delay) {
  leds[0] = CRGB(0, 0, 0);
  FastLED.show();
  delay(1000 * Delay);
}

void RGB_Led_On(int R, int G, int B) {
  leds[0] = CRGB(R, G, B);
  FastLED.show();
}

void RGB_Led_On_Delay(int R, int G, int B, int Delay) {
  leds[0] = CRGB(R, G, B);
  FastLED.show();
  delay(1000 * Delay);
}

void Led_On_Set_Brightness(int Brightness) {
  leds[0] = CRGB(Brightness, 0, 0);
  FastLED.show();
}

void Led_On_Set_BrightnesS_Delay(int Brightness, int Delay) {
  leds[0] = CRGB(Brightness, 0, 0);
  FastLED.show();
  delay(1000 * Delay);
}
