#define ECG_Analog_Pin 33 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define ECG_LO_P_Pin 16 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define ECG_LO_N_Pin 17 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int ECG_Value = 0;
void setup() {
// initialize the serial communication:
Serial.begin(115200);
pinMode(ECG_LO_P_Pin, INPUT); // Setup for leads off detection LO +
pinMode(ECG_LO_N_Pin, INPUT); // Setup for leads off detection LO -
}
void loop() {
if((digitalRead(ECG_LO_P_Pin) == 1)||(digitalRead(ECG_LO_N_Pin) == 1))
{
Serial.println('!');
}
else{
ECG_Value = analogRead(ECG_Analog_Pin);
}
//Wait for a bit to keep serial data from saturating
delay(10);
}
