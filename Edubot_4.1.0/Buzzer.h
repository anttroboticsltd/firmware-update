#ifndef Buzzer_H
#define Buzzer_H
#include"Edu_bot_esp32.h"

void Buzzer_init();

void Buzzer_On();
void Buzzer_On_Delay(int Delay);

void Buzzer_Off();
void Buzzer_Off_Delay(int Delay);

void Buzzer_On_Set_Vol(int Vol);
void Buzzer_On_Set_Vol_Delay(int Vol, int Delay);

void Play_Super_Mario();
void Play_Space_Gun();
void Play_Fibonacci();
void Play_Random();
void Play_Underworld();
void Play_Jingle_Bells();
void Play_Wish_Christmas();
void Play_Santa_Claus();


#endif
