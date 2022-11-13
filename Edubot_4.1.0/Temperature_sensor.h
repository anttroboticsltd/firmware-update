#ifndef Temperature_sensor_H
#define Temperature_sensor_H
#include"Edu_bot_esp32.h"

void Temperature_sensor_init();
float Read_Temperature_C();
float Read_Temperature_F();
void Print_Temperature_C();
void Print_Temperature_F();

#endif
