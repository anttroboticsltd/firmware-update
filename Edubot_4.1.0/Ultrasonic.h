#ifndef Ultrasonic_H
#define Ultrasonic_H
#include"Edu_bot_esp32.h"

void Ultrasonic_sensor_init();
int Read_Distance_CM();
float Read_Distance_INCH();
void Obstacle_Avoider_Robot(int Speed);

#endif
