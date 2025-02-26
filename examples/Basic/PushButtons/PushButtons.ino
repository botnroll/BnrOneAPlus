/**
 * This example was created by José Cruz (www.botnroll.com)
 * on the 18th December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * Read the Push Buttons state and print the result on the robot LCD and Serial
 * Monitor. Press PB1, PB2 or PB3 to interact with Bot'n Roll ONE A+
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
}

void loop() {
  byte pbutton = one.readButton();      // read the Push Button value
  one.lcd2(" Push Button: ", pbutton);  // print data on LCD line 2
  Serial.print(" Push Button: ");       // print data on serial monitor.
  Serial.println(pbutton);              // print data on serial monitor.
  delay(100);                           // wait 100 milliseconds
}
