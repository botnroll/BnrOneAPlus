/**
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot draws a square by taking in speed values in rpm (rotations per
 * minute) and using the ShapeGenerator class.
 * Explore other shapes and methods from the ShapeGenerator class
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp

#include "utils/ShapeGenerator.h"

// Constants definition
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define MINIMUM_BATTERY_V 10.5  // Safety voltage for discharging the battery

// Slip factor depends on the surface
// wood: 1.0
// vinyl: 0.985
// carpet: 0.985
const float SLIP_FACTOR = 0.94;

BnrOneAPlus one;  // Object to control the Bot'n Roll ONE A
ShapeGenerator one_draw(one, SLIP_FACTOR);

void setup() {
  Serial.begin(115200);   // Set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // Start SPI communication module
  one.stop();             // Stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // Battery discharge protection
  one.lcd1("  Draw Square   ");
  one.lcd2("www.botnroll.com");
  delay(3000);
  one_draw.square(300);
}

void loop() {
  // Empty loop
}
