#include <Wire.h>
#include "walkStepCounter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SH1106 display(OLED_SDA, OLED_SCL);

float x, y, z;
int StepsCount = 0, prev = 0;
int threshold = 3;

void Wlak_Step_for_loop() {
  int error;
  double dT;
  accel_t_gyro_union accel_t_gyro;

  // Read the raw values.
  error = read_gyro_accel_vals((uint8_t*) &accel_t_gyro);

  // Get the time of reading for rotation computations
  unsigned long t_now = millis();


  float FS_SEL = 131;
  float gyro_x = (accel_t_gyro.value.x_gyro - base_x_gyro) / FS_SEL;
  float gyro_y = (accel_t_gyro.value.y_gyro - base_y_gyro) / FS_SEL;
  float gyro_z = (accel_t_gyro.value.z_gyro - base_z_gyro) / FS_SEL;

  float accel_x = accel_t_gyro.value.x_accel;
  float accel_y = accel_t_gyro.value.y_accel;
  float accel_z = accel_t_gyro.value.z_accel;

  // Get angle values from accelerometer
  float RADIANS_TO_DEGREES = 180 / 3.14159;
  //float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
  float accel_angle_y = atan(-1 * accel_x / sqrt(pow(accel_y, 2) + pow(accel_z, 2))) * RADIANS_TO_DEGREES;
  float accel_angle_x = atan(accel_y / sqrt(pow(accel_x, 2) + pow(accel_z, 2))) * RADIANS_TO_DEGREES;

  float accel_angle_z = atan(sqrt(pow(accel_x, 2) + pow(accel_y, 2)) / accel_z) * RADIANS_TO_DEGREES;;
  //float accel_angle_z = 0;

  // Compute the (filtered) gyro angles
  float dt = (t_now - get_last_time()) / 1000.0;
  float gyro_angle_x = gyro_x * dt + get_last_x_angle();
  float gyro_angle_y = gyro_y * dt + get_last_y_angle();
  float gyro_angle_z = gyro_z * dt + get_last_z_angle();

  // Compute the drifting gyro angles
  float unfiltered_gyro_angle_x = gyro_x * dt + get_last_gyro_x_angle();
  float unfiltered_gyro_angle_y = gyro_y * dt + get_last_gyro_y_angle();
  float unfiltered_gyro_angle_z = gyro_z * dt + get_last_gyro_z_angle();

  // Apply the complementary filter to figure out the change in angle - choice of alpha is
  // estimated now.  Alpha depends on the sampling rate...
  float alpha = 0.96;
  float angle_x = alpha * gyro_angle_x + (1.0 - alpha) * accel_angle_x;
  float angle_y = alpha * gyro_angle_y + (1.0 - alpha) * accel_angle_y;
  float angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle

  // Update the saved data with the latest values
  set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);


  //Finding the magnitude of acceleration from the combined data from gyroscope and accelerometer.
  int mag = sqrt(pow(x - angle_x, 2) + pow(y - angle_y, 2) + pow(z - angle_z, 2));

  //If the magnitude is greater than threshold and the previous magnitude is lesser than threshold value increase count.
  if (mag >= threshold && prev < threshold)
  {
    StepsCount += 1;
    Serial.print("steps= ");
    Serial.println(StepsCount);
  }

  prev = mag;
  x = angle_x;
  y = angle_y;
  z = angle_z;
}

void setup()
{
  int error;
  uint8_t c;
  Serial.begin(115200);
  Wire.begin();

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setRotation(0);
  display.setTextSize(2);
  display.setCursor(8, 30);
  display.print("Pedometer");


  error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
  error = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);

  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);

  calibrate_sensors();
  set_last_read_angle_data(millis(), 0, 0, 0, 0, 0, 0);
}



void loop()
{

  Wlak_Step_for_loop();
  display.clearDisplay();
  display.setRotation(0);
  display.setTextSize(2);

  display.setCursor(30, 14);
  display.print("STEPS");
 
  display.setCursor(50, 36);
  display.print(StepsCount);
  display.display();
  
  delay(50); // Delay so we don't swamp the serial port
  
}
