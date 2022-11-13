#include <Arduino_LSM6DS3.h>
#include <ArduinoBLE.h>

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";
BLEService gestureService(deviceServiceUuid); 
BLEByteCharacteristic gestureCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);
int gesture = -1;

//Device information
BLEService myledService("58:BF:25:9B:35:52"); 
// Read and writeable by a central client. Each float is 4 bytes. (6 values = 24 bytes total to send)
BLECharacteristic myswitchCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 4);
BLEDevice central;

//Change float to 4 bytes to send over BLE byte server
void float2byte(byte bytes_temp[4], float float_variable){ 
  memcpy(bytes_temp, (unsigned char*) (&float_variable), 4);
}
//combined byte string to send over chareteristic BLE 
byte bytecomb[4] = {};
//Initialize where in bytecomb/posiiton to write each byte
byte *byteAy = bytecomb;
float Ay = 0;
byte value = 7;
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
  BLE.debug(Serial);
  //Show Advertised Name and service that its offering
  BLE.setLocalName("Simon IoT");
  //Computer
  BLE.setAdvertisedService(myledService);
  myledService.addCharacteristic(myswitchCharacteristic);
  myswitchCharacteristic.writeValue(bytecomb, 4);
  BLE.addService(myledService);
  //Left Controller
  BLE.setAdvertisedService(gestureService);
  gestureService.addCharacteristic(gestureCharacteristic);
  BLE.addService(gestureService);
  gestureCharacteristic.writeValue(-1);
  
  BLE.advertise();
  
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
  BLE.poll();
  
  BLEDevice leftcontroller = BLE.central();
  if (leftcontroller){
    Serial.println("Left Controller Connected");
    Serial.println("Waiting for Computer Connection");
    BLEDevice central = BLE.central();
    while (!central){
      BLEDevice central = BLE.central(); 
    }
    Serial.println("Computer Connected");
    while (leftcontroller.connected()){
      if (gestureCharacteristic.written()) {
        gesture = gestureCharacteristic.value();
        Serial.print("Value Written:");
        Serial.println(gesture);
      }

      // BLEDevice central = BLE.central();
      // if (central){
      //   Serial.println("Computer Connected");
        
      // }

    }
  }
  //BLEDevice central = BLE.central();
  
  if (central) { 
    Serial.print("Bluetooth central Paired: "); 
    while (central.connected()){
      //Just wait while connected for reads and update accordingly
      continue;
    }
  }
}

