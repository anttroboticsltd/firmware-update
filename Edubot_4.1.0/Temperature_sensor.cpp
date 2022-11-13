#include"Edu_bot_esp32.h"

OneWire oneWire(TEMP_SENSOR);         // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor

void Temperature_sensor_init() {
  sensors.begin();
  Serial.println("Temperature Sensor Initialize.");
}

float Read_Temperature_C() {
  sensors.requestTemperatures();
  float Temperature_C = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(Temperature_C);
  return Temperature_C;
}

float Read_Temperature_F() {
  sensors.requestTemperatures();
  float Temperature_F = DallasTemperature::toFahrenheit(sensors.getTempCByIndex(0));
  Serial.print("Temperature: ");
  Serial.println(Temperature_F);
  return Temperature_F;
}
