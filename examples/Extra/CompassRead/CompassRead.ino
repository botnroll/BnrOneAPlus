/**
 * @brief This example was created by José Cruz (www.botnroll.com)
 * on 18 December 2024
 * Updated on February 2025 by José Cruz
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 */

#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp

BnrOneAPlus one;    // declaration of object variable to control the Bot'n Roll ONE A+
BnrCompass compass; // declaration of object variable to control the CMPS11 compass

// constants definition
#define SSPIN 2       // Slave Select (SS) pin for SPI communication
#define ADDRESS 0x60  // CMPS11 I2C address

void setup()
{
  Serial.begin(57600);
  one.spiConnect(SSPIN);   // start SPI communication module
  one.stop();              // stop motors
  compass.i2cConnect(ADDRESS);
}

void loop()
{
   float bearing;
   char roll, pitch;

   bearing = compass.read_bearing();
   roll = compass.read_roll();
   pitch = compass.read_pitch();

   Serial.print("Bearing:"); Serial.print(bearing);
   Serial.print("   roll:"); Serial.print((int)roll);
   Serial.print("   pitch:"); Serial.println((int)pitch);
    
   delay(50);
}
