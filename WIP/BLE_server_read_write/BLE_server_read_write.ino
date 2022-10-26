#include <ArduinoBLE.h>
long previousMillis = 0;
int interval = 0;
int ledState = LOW;

//Device information
BLEService myledService("58:BF:25:9B:35:52"); 
// Read and writeable by a central client
//BLEByteCharacteristic myswitchCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 4);
BLECharacteristic myswitchCharacteristic("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 4);



byte PortValues[4] = {0b00001111, 0b00001110, 60, 70 };
  

void setup() {
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
  // set initial value for  characteristic
  //myswitchCharacteristic.writeValue(mystring);
  myswitchCharacteristic.writeValue(PortValues, 4);
  // advertise service/charecteristic
  BLE.advertise();

}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    //print MAC 
    Serial.println(central.address());
    // while the central connected to peripheral:
    // while (central.connected()) {
    //   if (myswitchCharacteristic.written()) {
    //     switch (myswitchCharacteristic.value()) {   
    //       case 01:
    //         //Turn on 
    //         Serial.println("LED on");
    //         digitalWrite(LED_BUILTIN, HIGH);            
    //         break;
    //       case 02:
    //           //Blink Fast
    //           Serial.println("LED fast blink");
    //           digitalWrite(LED_BUILTIN, HIGH);         
    //           delay(500);
    //           digitalWrite(LED_BUILTIN, LOW);         
    //           delay(500);
    //           digitalWrite(LED_BUILTIN, HIGH);      
    //           delay(500);
    //           digitalWrite(LED_BUILTIN, LOW);       
    //         break;
    //       case 03:
    //         //Blink slow
    //         Serial.println("LED slow blink");
    //         digitalWrite(LED_BUILTIN, HIGH);         
    //           delay(1000);
    //           digitalWrite(LED_BUILTIN, LOW);         
    //           delay(1000);
    //           digitalWrite(LED_BUILTIN, HIGH);      
    //           delay(1000);
    //           digitalWrite(LED_BUILTIN, LOW);       
    //         break;
    //       default:
    //         //Off by Default
    //         Serial.println(F("LED off"));
    //         digitalWrite(LED_BUILTIN, LOW);          
    //         break;
    //     }
    //   }
    // }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // will turn the LED off
  }
}