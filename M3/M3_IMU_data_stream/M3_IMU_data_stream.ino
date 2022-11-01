#include <ArduinoBLE.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_LSM6DS3.h>


float myax;
float myay;
float myaz;
float mygx;
float mygy;
float mygz; 

//Offsets
float offax;
float offay;
float offaz;
float offgx;
float offgy;
float offgz; 


void updateIMUvalues(){
  //Accel Values
  // if (IMU.accelerationAvailable()) {
  //   IMU.readAcceleration(myax, myay, myaz);
  //   Serial.print("Ax:");
  //   Serial.print(myax - offax);
  //   Serial.print(", Ay:");
  //   Serial.print(myay - offax);
  //   Serial.print(", Az:");
  //   Serial.println(myaz - offaz);
  // }
  //Gyro Values
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(mygx, mygy, mygz);
    Serial.print("Gx:");
    Serial.print(mygx - offgx);
    Serial.print(", Gy:");
    Serial.print(mygy - offgy);
    Serial.print(", Gz:");
    Serial.println(mygz - offgz);
  }
}

void setup() {
  Serial.begin(9600);
  //wait until serial montitor to initiate
  while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  IMU.begin();
  IMU.readAcceleration(offax, offay, offaz);
  IMU.readGyroscope(offgx, offgy, offgz);
}

void loop() {
  // listen for BLE peripherals to connect:
  updateIMUvalues();
  delay(200);
  
}