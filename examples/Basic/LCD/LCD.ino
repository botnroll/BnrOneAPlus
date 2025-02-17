/*
 This example was created by José Cruz (www.botnroll.com)
 on the 18th December 2024

 This code example is in the public domain.
 http://www.botnroll.com

Description:
Print a message on the robot LCD.
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

void setup() {
  Serial.begin(57600);    // set baud rate to 57600bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start the communication module
  one.stop();             // stop motors
}

void loop() {
  one.lcd1(" LCD Test OK !! ");            // print data on LCD line 1
  one.lcd2("www.botnroll.com");            // print data on LCD line 2
  Serial.println("Message sent to LCD!");  // print data on serial monitor.
  delay(1000);                             // wait 1 second
}
