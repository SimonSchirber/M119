#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_LSM6DS3.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  //Display Failure
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  //Gyroscope or Acceleromter Failure
  if (!IMU.begin()) {
    display.println("Failed to initialize IMU!");
    while (1);
  }
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
}
void loop() {
  mystyling();
}
void mystyling(void) {
  display.clearDisplay();
  //Accelerometer read
  float ax, ay, az;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    Serial.print("X Acceleration:");
    Serial.print(ax);
    Serial.print(",");
    Serial.print("Y Acceleration:");
    Serial.print(ay);
    Serial.print(",");
    Serial.print("Z Acceleration:");
    Serial.println(az);
  }
  //Gyroscope read
  float gx, gy, gz;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
    Serial.print("X Tilt:");
    Serial.print(gx);
    Serial.print(",");
    Serial.print("Y Tilt:");
    Serial.print(gy);
    Serial.print(",");
    Serial.print("Z Tilt:");
    Serial.println(gz);
  }
  //Initialize Top of Display 
  display.setTextSize(1);            
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);
  display.println("  Accel      Gyro");
  //Display  Text Numbersdisplay.setTextSize(1);             
  display.println((String)"AX: " + ax + "  GX: " + gx); 
  display.println((String)"AY: " + ay + "  GY: " + gy); 
  display.println((String)"AZ: " + az + "  GZ: " + gz); 
  display.display();
  delay(200);
}

