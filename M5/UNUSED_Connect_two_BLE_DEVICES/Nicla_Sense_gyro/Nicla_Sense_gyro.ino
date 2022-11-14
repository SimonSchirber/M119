#include "Arduino_BHY2.h"

SensorXYZ accelerometer(SENSOR_ID_ACC);
SensorXYZ gyro(SENSOR_ID_GYRO);
SensorQuaternion mag(SENSOR_ID_RV);

void setup(){
  Serial.begin(115200);
  BHY2.begin();
  //start accelerometer, gyro, and magnetometer
  accelerometer.begin();
  gyro.begin();
  mag.begin();
}

void loop(){
  static auto lastCheck = millis();

  // Update function should be continuously polled
  BHY2.update();

  // Check sensor values every second  
  if (millis() - lastCheck >= 500) {
  //   lastCheck = millis();
    short accX = accelerometer.x();
    float accY = accelerometer.y();
    float accZ = accelerometer.z();

    // float gyroX = gyro.x();
    // float gyroY = gyro.y();
    // float gyroZ = gyro.z();
    // Serial.println(gyroY);
    // Serial.println(gyroZ);
    if (accY > 3000){
      Serial.println("Flipped Up");
    }
    else if (accZ > 3000){
      Serial.println("Flipped Down");
    }
    else{
      Serial.println("Steady");
    }

    // Serial.println(String("acceleration: ") + accelerometer.toString());
    // Serial.println(String("gyroscope: ") + gyro.toString());
    // Serial.println(String("Magnetometer: ") + mag.toString());

  }
}