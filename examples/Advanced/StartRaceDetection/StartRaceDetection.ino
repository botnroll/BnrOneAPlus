/*
 * This example was created by José Cruz (www.botnroll.com)
 * on 18 December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * This program detects automatic start on the race challenge.
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define OFF 0

void setup() {
  one.spiConnect(SSPIN);             // start SPI communication module
  one.stop();                        // stop motors
  one.lcd1("IR testing");            // print on LCD line 1
  one.lcd2("STOP");                  // print on LCD line 2
  one.obstacleSensorsEmitters(OFF);  // deactivate obstacles IR emitters
  delay(4000);  // time to stabilize IR sensors (DO NOT REMOVE!!!)
  static byte start = 0;
  while (!start) {
    start = automaticStart();
  }
  one.move(50, 50);  // the robot move forward
  one.lcd2("GO");    // remove when racing for best performance!
}

bool automaticStart() {
  bool green_light = one.readObstacleSensors();  // read actual IR sensors state

  if (!green_light)
  {
    unsigned long int tempo_A = millis();  // read time
    while (!green_light)
    {
      green_light = one.readObstacleSensors();  // read actual IR sensors state
      // if 'green light' detected for more than 50ms
      if ((millis() - tempo_A) > 50)
      {
        return true;  // start Race
      }
    }
  }
  return false;
}

void loop() {}