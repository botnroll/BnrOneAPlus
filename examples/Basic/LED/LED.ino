/**
 * @brief     This example was created by José Cruz (www.botnroll.com)
 *   on the 18th December 2024
 *
 *   This code example is in the public domain.
 *   http://www.botnroll.com
 *
 *   Description:
 *   The LED is switched ON and OFF every second.
 *   A message is sent to the Serial Monitor accordingly.
 *
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start the communication module
  one.stop();             // stop motors
}

void loop() {
  one.setLed(HIGH);           // turn LED ON
  Serial.println("LED ON");   // print data on serial monitor.
  delay(1000);                // wait 1 second
  one.setLed(LOW);            // turn LED OFF
  Serial.println("LED OFF");  // print data on serial monitor.
  delay(1000);                // wait 1 second
}
