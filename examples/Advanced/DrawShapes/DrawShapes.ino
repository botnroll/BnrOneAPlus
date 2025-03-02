/**
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot draws shapes by taking in speed values in rpm (rotations per
 * minute) and using the ShapeGenerator class.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp

#include "utility/ShapeGenerator.h"

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

void drawShapes() { one_draw.square(300); }

void setup() {
  Serial.begin(115200);   // Set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // Start SPI communication module
  one.stop();             // Stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // Battery discharge protection
  one.lcd1("  Draw Shapes   ");
  one.lcd2("www.botnroll.com");
  Serial.println("Get ready");
  delay(3000);
  Serial.println("Go");
  drawShapes();
}

void loop() {
  // Empty loop
}
