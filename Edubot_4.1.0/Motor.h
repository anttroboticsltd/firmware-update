#ifndef Motor_H
#define Motor_H
#include"Edu_bot_esp32.h"


/**
   Sec: 0.
   motor initialization.
*/
void Motor_init();

/**
   Sec: 1.
   motor 1 all
*/
void Motor1_On();
void Motor1_Off();
void Motor1_On_Delay(int Delay);
void Motor1_Off_Delay(int Delay);

/**
   Sec: 2.
   motor 2 all
*/
void Motor2_On();
void Motor2_Off();
void Motor2_On_Delay(int Delay);
void Motor2_Off_Delay(int Delay);

/**
   Sec: 3.
*/
void Robot_Move_Stop();
void Robot_Move_Stop_Delay(int Delay);

/**
   Sec: 4.
   Robot movement using only delay.
*/
void Robot_Move_Forward_Delay(int Delay);
void Robot_Move_Backward_Delay(int Delay);
void Robot_Move_Right_Delay(int Delay);
void Robot_Move_Left_Delay(int Delay);
void Robot_Move_Stop_Delay(int Delay);

/**
   Sec: 5.
   Robot movement using only speed.
*/
void Robot_Move_Forward_Speed(int Speed);
void Robot_Move_Backward_Speed(int Speed);
void Robot_Move_Right_Speed(int Speed);
void Robot_Move_Left_Speed(int Speed);

/**
   Sec: 6.
   Robot movement using delay & speed.
*/
void Robot_Move_Forward_Delay_Speed(int Delay, int Speed);
void Robot_Move_Backward_Delay_Speed(int Delay, int Speed);
void Robot_Move_Right_Delay_Speed(int Delay, int Speed);
void Robot_Move_Left_Delay_Speed(int Delay, int Speed);


#endif
