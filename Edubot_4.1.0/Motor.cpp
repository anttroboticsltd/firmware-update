#include"Edu_bot_esp32.h"

//frequency for PWM
const int freq = 5000;
//resolution for PWM
const int resolution = 8;

//PWM Channel
int RM1_Channel = 0;
int RM2_Channel = 1;
int LM1_Channel = 2;
int LM2_Channel = 3;

void Motor_init() {
  //Config channel with freq with resulation.
  ledcSetup(RM1_Channel, freq, resolution);
  ledcSetup(RM2_Channel, freq, resolution);
  ledcSetup(LM1_Channel, freq, resolution);
  ledcSetup(LM2_Channel, freq, resolution);

  // Config channel with I/O
  //Motor pin in GPIO.h
  ledcAttachPin(RM1, RM1_Channel);
  ledcAttachPin(RM2, RM2_Channel);
  ledcAttachPin(LM1, LM1_Channel);
  ledcAttachPin(LM2, LM2_Channel);

  //PWM write
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
  Serial.println("Motor Initialize.");
}

void Motor1_On() {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(RM1_Channel, 255);
  ledcWrite(RM2_Channel, 0);
}
void Motor1_Off() {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
}
void Motor1_On_Delay(int Delay) {
  ledcWrite(RM1_Channel, 255);
  ledcWrite(RM2_Channel, 0);
  delay(Delay * 1000);
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
}
void Motor1_Off_Delay(int Delay) {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  delay(Delay * 1000);
}

void Robot_Move_Stop() {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
}
void Robot_Move_Stop_Delay(int Delay) {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
  delay(1000 * Delay);
}

void Robot_Move_Right(int Delay) {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 255);
  ledcWrite(LM2_Channel, 0);
  delay(Delay * 1000);
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
}
void Robot_Move_Left(int Delay) {
  ledcWrite(RM1_Channel, 255);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
  delay(Delay * 1000);
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
}

void Robot_Move_Forward_Speed(int Speed) {
  ledcWrite(RM1_Channel, Speed);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, Speed);
  ledcWrite(LM2_Channel, 0);
}
void Robot_Move_Backward_Speed(int Speed) {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, Speed);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, Speed);
}
void Robot_Move_Right_Speed(int Speed) {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, Speed);
  ledcWrite(LM2_Channel, 0);
}
void Robot_Move_Left_Speed(int Speed) {
  ledcWrite(RM1_Channel, Speed);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
}

void Robot_Move_Forward_Delay_Speed(int Delay, int Speed) {
  ledcWrite(RM1_Channel, Speed);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, Speed);
  ledcWrite(LM2_Channel, 0);
  delay(Delay * 1000);
}
void Robot_Move_Backward_Delay_Speed(int Delay, int Speed) {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, Speed);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, Speed);
  delay(Delay * 1000);
}
void Robot_Move_Right_Delay_Speed(int Delay, int Speed) {
  ledcWrite(RM1_Channel, 0);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, Speed);
  ledcWrite(LM2_Channel, 0);
  delay(Delay * 1000);
}
void Robot_Move_Left_Delay_Speed(int Delay, int Speed) {
  ledcWrite(RM1_Channel, Speed);
  ledcWrite(RM2_Channel, 0);
  ledcWrite(LM1_Channel, 0);
  ledcWrite(LM2_Channel, 0);
  delay(Delay * 1000);
}
