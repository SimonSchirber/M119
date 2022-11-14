#include <ArduinoBLE.h>

//////////For Nicla Sense #1 of 3///////////
#include "Arduino_BHY2.h"
SensorXYZ accelerometer(SENSOR_ID_ACC);
SensorXYZ gyro(SENSOR_ID_GYRO);
SensorQuaternion mag(SENSOR_ID_RV);

float multiplier = -1;

BLEService myledService("58:BF:25:9B:35:52"); 
// Read and writeable by a central client. Each float is 4 bytes. (6 values = 24 bytes total to send)
BLECharacteristic myswitchCharacteristic("79B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 4);

//Change float to 4 bytes to send over BLE byte server
void float2byte(byte bytes_temp[4], float float_variable){ 
  memcpy(bytes_temp, (unsigned char*) (&float_variable), 4);
}
//combined byte string to send over chareteristic BLE 
byte bytecomb[4] = {};
//Initialize where in bytecomb/posiiton to write each byte
byte *stateByte = bytecomb;
float Ay = 0;

//////////For 33 IoT////////
// void writeIMUvalue(BLEDevice central, BLECharacteristic myswitchCharacteristic){
//   float ax, ay, az;
//   if (IMU.accelerationAvailable()) {
//     IMU.readAcceleration(ax, ay, az);
//     Serial.print("Ay:");
//     Serial.println(ay);
//   }
//   float2byte(stateByte, ay);
//   myswitchCharacteristic.writeValue(bytecomb, 4);
// }

//////////For Nicla Sense #2 of 3///////////
void writeIMUvalue(BLEDevice central, BLECharacteristic myswitchCharacteristic){
  short accY = accelerometer.y();
  float accZ = accelerometer.z();
  if (accY > 3000){
    Serial.println("Flipped up");
    float2byte(stateByte, 1);
  }
 else if (accY < -300){
    Serial.println("Flipped Down");
    float2byte(stateByte, -1);
  }
  else{
    Serial.println("Steady");
    float2byte(stateByte, 0);
  }
  myswitchCharacteristic.writeValue(bytecomb, 4);
}

void setup() {
  Serial.begin(115200);
  //////////For Nicla Sense #3 of 3///////////
  BHY2.begin();
  accelerometer.begin();
  gyro.begin();
  mag.begin();

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
    while (1);
  }
  Serial.println("BLE started: Waiting for a central client connection");
  BLE.setLocalName("Simon P2");
  BLE.setAdvertisedService(myledService);
  myledService.addCharacteristic(myswitchCharacteristic);
  BLE.addService(myledService);
  myswitchCharacteristic.writeValue(bytecomb, 4);
  BLE.advertise();
  myswitchCharacteristic.setEventHandler(BLERead, writeIMUvalue );
  
 

  ////////////////////IOt #3 of 3//////////////////////
  // IMU.begin();
  // if (!IMU.begin()) {
  //   Serial.println("Failed to initialize IMU!");
  //   while (1);
  // }

  delay(1000);
}




void loop() {
  BLEDevice central = BLE.central();
  if (central) { 
    Serial.print("Bluetooth central Paired: "); 
    while (central.connected()){
      BHY2.update();
      continue;
    }
  }
  //Bluetooth Disconnected
}
