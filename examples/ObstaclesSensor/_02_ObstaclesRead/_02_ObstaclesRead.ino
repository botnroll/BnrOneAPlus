/*
 This example was created by José Cruz (www.botnroll.com)
 on the 22th August 2022
 Updated on February 2025 by José Cruz

 This code example is in the public domain.
 http://www.botnroll.com

Description:
Read obstacle sensors range distance for left and right IR sensors.
Range varies from 0 to 25:
 -> 0 means no obstacle is detected
 -> 25 means obstacle is very close to the sensor
The robot has new readings every 25ms (40 readings per second)
Note: Valid for TSSP4056 IR sensors shipped with robots from 2023.
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A library
#include <SPI.h>          // SPI communication library required by BnrOne.cpp
BnrOneAPlus one;          // object to control the Bot'n Roll ONE A Plus

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define OFF 0
#define ON 1

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.obstacleSensorsEmitters(ON);  // activate IR emitters
}

void loop() {
  // read left obstacle sensor range
  byte left_range = one.readLeftRangeSensor();
  // read right obstacle sensor range
  byte right_range = one.readRightRangeSensor();
  one.lcd1("Left range: ", left_range);
  one.lcd2("Right range: ", right_range);
  Serial.print("L: ");
  Serial.print(left_range);
  Serial.print("   R: ");
  Serial.println(right_range);
  delay(25);  // The robot has new readings every 25ms (40 readings per second)
}