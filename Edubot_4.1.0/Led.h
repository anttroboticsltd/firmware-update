#ifndef Led_H
#define Led_H
#include"Edu_bot_esp32.h"

void Led_init();
void Led_On();
void Led_On_Delay(int Delay);
void Led_Off();
void Led_Off_Delay(int Delay);

void RGB_Led_On(int R, int G, int B);
void RGB_Led_On_Delay(int R, int G, int B, int Delay);

void Led_On_Set_Brightness(int Brightness);
void Led_On_Set_BrightnesS_Delay(int Brightness, int Delay);

#endif
