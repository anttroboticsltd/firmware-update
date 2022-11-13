#ifndef Edu_bot_esp32
#define Edu_bot_esp32

#include <Arduino.h>
#include <esp32-hal-ledc.h>
#include <analogWrite.h>
#include <FastLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Tone32.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "Motor.h"
#include "Led.h"
#include "Buzzer.h"
#include "Ultrasonic.h"
#include "IR_Sensor.h"
#include "Temperature_sensor.h"
#include "USB_sensor.h"
#include "LiquidCrystal_I2C.h"


#define BAUDRATE        115200
#define NUM_LEDS        1
#define LED             2
#define TEMP_SENSOR     19
#define BUZZER          23
#define LM2             13
#define LM1             15
#define RM2             4
#define RM1             16
#define IR_SENSOR_1_D   18
#define IR_SENSOR_2_D   17
#define IR_SENSOR_3_D   5
#define IR_SENSOR_1_A   34
#define IR_SENSOR_2_A   35
#define IR_SENSOR_3_A   27
#define TRIG_PIN        12
#define ECHO_PIN        14
#define USB_SENSOR_1_D  25    // Ldr SENSOR
#define USB_SENSOR_2_D  26    // Smoke SENSOR
#define USB_SENSOR_1_A  34    // smoke : 20%, 1100
#define USB_SENSOR_2_A  35    // ldr : 3%, 100

#define URL_fw_Version  "https://raw.githubusercontent.com/jahidulislamrahat97/chip-version-updater-xyz/main/abc/bin_version.txt"
#define URL_fw_Bin      "https://raw.githubusercontent.com/jahidulislamrahat97/chip-version-updater-xyz/main/abc/fw.bin"
#define ssid            "ANTT Robotics TP"
#define password        "digi@anti@123"

#endif
