#include"Edu_bot_esp32.h"

void USB_sensor_init() {
  pinMode(USB_SENSOR_1_D, INPUT);
  pinMode(USB_SENSOR_2_D, INPUT);
  pinMode(USB_SENSOR_1_A, INPUT);
  pinMode(USB_SENSOR_2_A, INPUT);
  Serial.println("USB Sensor Initialize.");
}

int D_Read_USB_1() {
  int value = digitalRead(USB_SENSOR_1_D);
  return value;
}

int D_Read_USB_2() {
  int value = digitalRead(USB_SENSOR_2_D);
  return value;
}
int A_Read_USB_P_1() {
  int value = map(analogRead(USB_SENSOR_1_A), 0, 4095, 0, 100);
  Serial.print("A_Read_USB_P_1: ");
  Serial.println(value);
  return value;
}
int A_Read_USB_P_2() {
  int value = map(analogRead(USB_SENSOR_2_A), 0, 4095, 0, 100);
  Serial.print("A_Read_USB_P_2: ");
  Serial.println(value);
  return value;
}
int A_Read_USB_1() {
  int value = analogRead(USB_SENSOR_1_A);
  Serial.print("A_Read_USB_1: ");
  Serial.println(value);
  return value;
}

int A_Read_USB_2() {
  int value = analogRead(USB_SENSOR_2_A);
  Serial.print("A_Read_USB_2: ");
  Serial.println(value);
  return value;
}
