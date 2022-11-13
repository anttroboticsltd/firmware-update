#include"Edu_bot_esp32.h"

void Ultrasonic_sensor_init() {
  pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
  digitalWrite(TRIG_PIN, LOW);
  Serial.println("Ultrasonic Sensor Initialize.");
}

int Read_Distance_CM() {
  // Clears the trigPin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  int distance = duration / 29 / 2;
  Serial.print("distance: ");
  Serial.println(distance);
  return distance;
  delay(1);
}

float Read_Distance_INCH() {
  // Clears the trigPin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  float distance = duration / 29 / 2 * 0.394;
  Serial.print("distance: ");
  Serial.println(distance);
  return distance;
  delay(1);
}
