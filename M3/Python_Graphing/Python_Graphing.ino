#include <ArduinoBLE.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_LSM6DS3.h>

//Device information
BLEService myledService("58:BF:25:9B:35:52"); 
// Read and writeable by a central client. Each float is 4 bytes. (6 values = 24 bytes total to send)
BLECharacteristic myswitchCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 24);


//BLEFloatCharacteristic myswitchCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
//byte PortValues[4] = {0b00001111, 7, 600, -70};

//Change float to 4 bytes to send over BLE byte server
void float2byte(byte bytes_temp[4],float float_variable){ 
  memcpy(bytes_temp, (unsigned char*) (&float_variable), 4);
}
//combined byte string to send over chareteristic BLE 
byte bytecomb[24] = {};
//dedicate each memory space of a byte to be in the combined byte
byte *mybyteax = bytecomb;
byte *mybyteay = bytecomb + 4;
byte *mybyteaz = bytecomb + 8;
byte *mybytegx = bytecomb + 12;
byte *mybytegy = bytecomb + 16;
byte *mybytegz = bytecomb + 20;
float myax;
float myay = 1;
float myaz;
float mygx;
float mygy;
float mygz;  

void updatecombfloat(){
  float2byte(mybyteax, myax);
  float2byte(mybyteay, myay);
  float2byte(mybyteaz, myaz);
  float2byte(mybytegx, mygx);
  float2byte(mybytegy, mygy);
  float2byte(mybytegz, mygz);
  myswitchCharacteristic.writeValue(bytecomb, 24);
}

void updateIMUvalues(BLEDevice central, BLECharacteristic myswitchCharacteristic){
  //Accel Values
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(myax, myay, myaz);
    Serial.print("Ax:");
    Serial.print(myax);
    Serial.print(", Ay:");
    Serial.print(myay);
    Serial.print(", Az:");
    Serial.print(myaz);
  }
  //Gyro Values
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(mygx, mygy, mygz);
    Serial.print(", Gx:");
    Serial.print(mygx);
    Serial.print(", Gy:");
    Serial.print(mygy);
    Serial.print(", Gz:");
    Serial.println(mygz);
  }
  updatecombfloat();
}



void setup() {
  updatecombfloat();
  Serial.begin(9600);
  //wait until serial montitor to initiate
  while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  // Start BLE, loop until it has begun
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
  myswitchCharacteristic.writeValue(bytecomb, 24);
  myswitchCharacteristic.setEventHandler(BLERead, updateIMUvalues);
  // advertise service/charecteristic
  BLE.advertise();
  IMU.begin();
  Serial.println("Waiting for a central client connection");
  
}

void loop() {
  // listen for BLE peripherals to connect:
  
  BLEDevice central = BLE.central();
  
  digitalWrite(LED_BUILTIN, HIGH);
  if (central) { 
    //Serial.print("Connected to central: ");
    //Serial.println(central.address());   
    while (central.connected()){
      //everytime a read is performed, update gyro valuse and print
      
    }
      digitalWrite(LED_BUILTIN, LOW);
      //Serial.println("Disconnected from central: ");
  }
  
}