/**
 * @brief This example was created by José Cruz (www.botnroll.com)
 * on 26th of February 2025
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 */

#include <BnrCompass.h>   // Compass library
#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp

// constants definition
#define SSPIN 2       // Slave Select (SS) pin for SPI communication
#define ADDRESS 0x60  // CMPS11 I2C address

BnrOneAPlus one;              // declaration of the Bot'n Roll ONE A+
BnrCompass compass(ADDRESS);  // declaration of the CMPS12 compass

void setup() {
  Serial.begin(115200);
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
}

void loop() {
  float bearing;
  char roll, pitch;

  bearing = compass.readBearing();
  roll = compass.readRoll();
  pitch = compass.readPitch();

  Serial.print("Bearing:");
  Serial.print(bearing);
  Serial.print("   roll:");
  Serial.print((int)roll);
  Serial.print("   pitch:");
  Serial.println((int)pitch);

  one.lcd1("Bea   Pit   Rol");
  one.lcd2((int)bearing, (int)pitch, (int)roll);

  delay(50);
}
