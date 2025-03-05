/**
 * This example was created by the Laboratory of Automation and Robotics for
 * the RoboParty Fun Challenge (https://lar.dei.uminho.pt/) on 18 December 2024
 *
 * Updated on February 2025 by José Cruz
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * This program detects automatic start and does the automatic end on the
 * RoboParty Fun Challenge.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

void setup() {
  one.spiConnect(SSPIN);                  // start SPI communication module
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.stop();                             // stop motors
  one.lcd1(" FUN CHALLENGE");             // print on LCD line 1
}

void loop() {
  one.lcd2(" Press a button ");
  while (one.readButton() == 0)
    ;
  one.lcd2("    Moving!!!   ");
  one.move(80, 80);
  delay(1500);
  one.brake(100, 100);
  delay(100);
  one.move(-80, -80);
  delay(2000);
  one.brake(100, 100);
  delay(100);
}
