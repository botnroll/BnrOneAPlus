/**
 * This example was created by José Cruz (www.botnroll.com)
 * on the 26th of February 2025
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot moves and messages are printed on the LCD identifying the
 * movements.
 */

#include <BnrColorSensor.h>  // Bot'n Roll Color Sensor library
#include <BnrOneAPlus.h>     // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp

// constants definitions
#define SENSOR_ADDRESS 0x2C  // default factory address
#define SSPIN 2              // Slave Select (SS) pin for SPI communication

// object declaration
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A
BnrColorSensor colorsensor(
    SENSOR_ADDRESS);  // declaration of object variable to control Bot'n
                      // Roll Color Sensor

void setup() {
  Serial.begin(115200);   // set baud rate to 57600bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  colorsensor.setRgbStatus(ENABLE);  // Enable/Disable RGB scanning
}

void printRGBValues(byte rgbL[3], byte rgbR[3]) {
  Serial.print("Left R:");
  Serial.print(rgbL[0]);
  Serial.print("G:");
  Serial.print(rgbL[1]);
  Serial.print("B:");
  Serial.print(rgbL[2]);

  Serial.print("Right R:");
  Serial.print(rgbR[0]);
  Serial.print("G:");
  Serial.print(rgbR[1]);
  Serial.print("B:");
  Serial.print(rgbR[2]);

  Serial.println();
}

void loop() {
  byte rgbL[3] = {0, 0, 0};
  byte rgbR[3] = {0, 0, 0};

  colorsensor.readRGBL(rgbL[0], rgbL[1], rgbL[2]);  // Read Left RGB sensor
  colorsensor.readRGBR(rgbR[0], rgbR[1], rgbR[2]);  // Read Right RGB sensor

  one.lcd1(rgbL[0], rgbL[1], rgbL[2]);
  one.lcd2(rgbR[0], rgbR[1], rgbR[2]);

  delay(10);
}
