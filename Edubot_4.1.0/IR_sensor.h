#ifndef IR_Sensor_H
#define IR_Sensor_H
#include"Edu_bot_esp32.h"

void IR_sensor_init();
int Read_IR_Sensor_1();
int Read_IR_Sensor_2();
int Read_IR_Sensor_3();
void Follow_black_line();
void Follow_white_line();


#endif
