/*
 This example was created by José Cruz (www.botnroll.com)
 on the 18th December 2024

 This code example is in the public domain.
 http://www.botnroll.com

Description:
Read the battery voltage and print its value on the LCD and Serial Monitor.
Also configures the minimum battery voltage that causes the robot to stop if
voltage is below the defined value. It is VERY important that you define this
minimum voltage correctly to preserve your robot battery life.
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

void setup() {
  // setup routines -> runs only once when program starts
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(10.5);  // define de minimum battery voltage. Robot stops
                             // if voltage is below the specified value!
}

void loop() {
  float battery = one.readBattery();  // read battery voltage
  one.lcd2("Battery V: ", battery);   // print data on LCD line 2
  Serial.print("Battery V: ");        // print data on serial monitor.
  Serial.println(battery);            // print data on serial monitor.
  delay(200);                         // wait 200 milliseconds
}
