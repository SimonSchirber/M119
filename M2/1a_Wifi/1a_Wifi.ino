#include <WiFiNINA.h>
#include "Secrets.h"
  

int status = WL_IDLE_STATUS;             
int ledState = LOW;                       
unsigned long previousMillisInfo = 0;     
unsigned long previousMillisLED = 0;      
const int intervalInfo = 5000;            

void setup() {
  Serial.begin(9600);
  //Wait for serial port to open to begin program
  while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  //loop until connected to Wi-Fi)
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    //status = WiFi.begin(ssid, pass);
    status = WiFi.beginEnterprise(ssid, username, pass);
    delay(5000);
    Serial.println(status);
  }
  Serial.println("Connected to Network");
  Serial.println("---------------------------------------");
}

void loop() {
  //relatve time since sketch started running
  unsigned long currentMillisInfo = millis();
  // check time since last update
  if (currentMillisInfo - previousMillisInfo >= intervalInfo) {
    previousMillisInfo = currentMillisInfo;
    Serial.println("Board Info:");
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    // print SSID:
    Serial.println();
    Serial.println("Network Information:");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);
    Serial.println("---------------------------------------");
  }
  unsigned long currentMillisLED = millis();
  // convert signal strength to lenght of time to turn on led
  int intervalLED = WiFi.RSSI() * -10;
  if (currentMillisLED - previousMillisLED >= intervalLED) {
    previousMillisLED = currentMillisLED;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
  }
}
