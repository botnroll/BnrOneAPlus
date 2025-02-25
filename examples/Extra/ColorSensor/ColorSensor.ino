/**
 * This example was created by José Cruz (www.botnroll.com)
 * on the 18th December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot moves and messages are printed on the LCD identifying the
 * movements.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>          // SPI communication library required by BnrOneAPlus.cpp

// object declaration
BnrOneAPlus one;             // object to control the Bot'n Roll ONE A
BnrColorSensor colorsensor;  // declaration of object variable to control Bot'n Roll Color Sensor

// constants definitions
#define SENSOR_ADDRESS 0x2C  // default factory address
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

void setup() {
  Serial.begin(57600);                     // set baud rate to 57600bps for printing values at
                                           // serial monitor.
  one.spiConnect(SSPIN);                   // start SPI communication module
  one.stop();                              // stop motors
  colorsensor.i2cConnect(SENSOR_ADDRESS);  // Enable I2C communication
  colorsensor.setRgbStatus(ENABLE);        // Enable/Disable RGB scanning
}

void loop() {
  byte rgbL[3] = {0, 0, 0};
  byte rgbR[3] = {0, 0, 0};

  colorsensor.readRGBL(&rgbL[0], &rgbL[1], &rgbL[2]);  // Read Left RGB sensor
  colorsensor.readRGBR(&rgbR[0], &rgbR[1], &rgbR[2]);  // Read Right RGB sensor     //  0xF1 //Save calibration data

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

  one.lcd1(rgbL[0], rgbL[1], rgbL[2]);
  one.lcd2(rgbR[0], rgbR[1], rgbR[2]);

  delay(10);
}
