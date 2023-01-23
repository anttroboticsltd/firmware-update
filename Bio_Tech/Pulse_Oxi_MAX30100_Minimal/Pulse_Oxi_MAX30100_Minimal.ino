#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SH1106 display(OLED_SDA, OLED_SCL);

#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     500
PulseOximeter pox;
uint32_t tsLastReport = 0;
int BPM_Value = 0;
int BPM_Max_Value = 0;
int Oxygen_in_Blood = 0;


void BPM() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    Serial.print("BPM:");
    BPM_Value = pox.getHeartRate();
    Serial.println(BPM_Value  );

    if (BPM_Value > BPM_Max_Value) {
      BPM_Max_Value = BPM_Value;
    }
    Serial.print("Max BPM:");
    Serial.println(BPM_Max_Value);

    Serial.print("Oxygen_in_Blood(SpO2)=");
    Oxygen_in_Blood = pox.getSpO2();
    Serial.print(Oxygen_in_Blood);
    Serial.println("%");
    Serial.println("");
    tsLastReport = millis();
  }

}
void setup()
{
  Serial.begin(115200);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setRotation(0);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("BPM");
  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
}

void loop() {
  
  BPM();
  display.clearDisplay();
  display.setRotation(0);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("MaxBPM");
  display.setCursor(70, 0);
  display.print("=");
  display.setCursor(87, 0);
  display.print(BPM_Max_Value);

  display.setCursor(10, 26);
  display.print("BPM");
  display.setCursor(61, 26);
  display.print("=");
  display.setCursor(77, 26);
  display.print(BPM_Value);

  display.setCursor(10, 50);
  display.print("SpO2");
  display.setCursor(61, 50);
  display.print("=");
  display.setCursor(77, 50);
  display.print(Oxygen_in_Blood);
  display.print("%");

  display.display();
}
