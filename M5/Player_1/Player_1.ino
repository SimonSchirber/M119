#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>


BLEService myledService("58:BF:25:9B:35:52"); 
// Read and writeable by a central client. Each float is 4 bytes. (6 values = 24 bytes total to send)
BLECharacteristic myswitchCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 4);

//Change float to 4 bytes to send over BLE byte server
void float2byte(byte bytes_temp[4], float float_variable){ 
  memcpy(bytes_temp, (unsigned char*) (&float_variable), 4);
}
//combined byte string to send over chareteristic BLE 
byte bytecomb[4] = {};
//Initialize where in bytecomb/posiiton to write each byte
byte *byteAy = bytecomb;
float Ay = 0;

void WriteIMUvalue(BLEDevice central, BLECharacteristic myswitchCharacteristic){
  float ax, ay, az;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    Serial.print("Ay:");
    Serial.println(ay);
  }
  float2byte(byteAy, ay);
  myswitchCharacteristic.writeValue(bytecomb, 4);
}

void setup() {
  Serial.begin(9600);
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
    while (1);
  }
  //Show Advertised Name and service that its offering
  BLE.setLocalName("Simon IoT");
  BLE.setAdvertisedService(myledService);
  // add characteristic to the service
  myledService.addCharacteristic(myswitchCharacteristic);
  BLE.addService(myledService);
  myswitchCharacteristic.writeValue(bytecomb, 4);
  BLE.advertise();
  //Every time a read is performed update the value
  myswitchCharacteristic.setEventHandler(BLERead, WriteIMUvalue);
  
  IMU.begin();
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU Initialized");
  Serial.println("BLE started: Waiting for a central client connection");
  delay(1000);
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) { 
    Serial.print("Bluetooth central Paired: "); 
    while (central.connected()){
      //Just wait while connected for reads and update accordingly
      continue;
    }
  }
}

