#include"Edu_bot_esp32.h"

//frequency for PWM
const int freq = 5000;
//resolution for PWM
const int resolution = 8;
//PWM Channel
int BUZZER_Channel = 4;

//Super Mario
int mario_melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
int mario_tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

//Underwolrd
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

//Jingle Bells
int Jingle_Bells_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};
int Jingle_Bells_tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// We wish you a merry Christmas
int wish_melody[] = {
  NOTE_B3,
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};
int wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};


// Santa Claus is coming to town
int santa_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};
int santa_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};


void Buzzer_init() {
  //Config channel with freq with resulation.
  ledcSetup(BUZZER_Channel, freq, resolution);
  // Config channel with I/O
  //Motor pin in GPIO.h
  ledcAttachPin(BUZZER, BUZZER_Channel);
  Serial.println("Buzzer Initialize.");
}


void Buzzer_On() {
  ledcWriteNote(BUZZER_Channel, NOTE_C, 4);
}
void Buzzer_On_Delay(int Delay) {
  ledcWriteNote(BUZZER_Channel, NOTE_C, 4);
  delay(Delay * 1000);
}
void Buzzer_Off() {
  ledcWriteTone(BUZZER_Channel, 0);
}
void Buzzer_Off_Delay(int Delay) {
  ledcWriteTone(BUZZER_Channel, 0);
  delay(Delay * 1000);
}
void Buzzer_On_Set_Vol(int Vol) {
  ledcWrite(BUZZER_Channel, Vol);
}
void Buzzer_On_Set_Vol_Delay(int Vol, int Delay) {
  ledcWrite(BUZZER_Channel, Vol);
  delay(1000 * Delay);
}


/*********************Music*********************/

void Play_Super_Mario() {
  int size = sizeof(mario_melody) / sizeof(int);
  for (int i = 0; i <= size; i++) {
    int noteDuration = 1000 / mario_tempo[i];
    tone(BUZZER, mario_melody[i], noteDuration, BUZZER_Channel);
    noTone(BUZZER, BUZZER_Channel);
  }
}

void Play_Underworld() {
  int size = sizeof(underworld_melody) / sizeof(int);
  for (int i = 0; i <= size; i++) {
    int noteDuration = 1000 / underworld_tempo[i];
    tone(BUZZER, underworld_melody[i], noteDuration, BUZZER_Channel);
    noTone(BUZZER, BUZZER_Channel);
  }
}

void Play_Jingle_Bells() {
  int size = sizeof(Jingle_Bells_melody) / sizeof(int);
  for (int i = 0; i <= size; i++) {
    int noteDuration = 1000 / Jingle_Bells_tempo[i];
    tone(BUZZER, Jingle_Bells_melody[i], noteDuration, BUZZER_Channel);
    noTone(BUZZER, BUZZER_Channel);
  }
}

void Play_Wish_Christmas() {
  int size = sizeof(wish_melody) / sizeof(int);
  for (int i = 0; i <= size; i++) {
    int noteDuration = 1000 / wish_tempo[i];
    tone(BUZZER, wish_melody[i], noteDuration, BUZZER_Channel);
    noTone(BUZZER, BUZZER_Channel);
  }
}

void Play_Santa_Claus() {
  int size = sizeof(santa_melody) / sizeof(int);
  for (int i = 0; i <= size; i++) {
    int noteDuration = 1000 / santa_tempo[i];
    tone(BUZZER, santa_melody[i], noteDuration, BUZZER_Channel);
    noTone(BUZZER, BUZZER_Channel);
  }
}
