/**
 * This example was created by José Cruz (www.botnroll.com)
 * on the 18th December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot moves and messages are printed on the LCD identifying the
 * movements.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A

// constants definition
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.setPid(2200, 245, 60);  // set PID parameters for robot movement
  one.lcd1(" Motors Testing ");
  one.lcd2(" Press a button ");
  // Wait a button to be pushed <> Espera que pressione um botão
  while (one.readButton() == 0);
}

void moveForwards(const int speed, const int time) {
  one.lcd2("    Forward ");  // print data on LCD line 2
  one.move(speed, speed);    // Forward
  delay(time);               // wait 1 second
}

void moveBackwards(const int speed, const int time) {
  one.lcd2("   Backwards ");
  one.move(-speed, -speed);  // Backwards
  delay(time);
}

void rotateLeft(const int speed, const int time) {
  one.lcd2("  Rotate Left ");
  one.move(-speed, speed);  // Rotate Left
  delay(time);
}

void rotateRight(const int speed, const int time) {
  one.lcd2("  Rotate Right ");
  one.move(speed, -speed);  // Rotate Right
  delay(time);
}

void stop(const int time) {
  one.lcd2("     Stop   ");
  one.stop();  // Stop Motors
  delay(time);
}

void brake(const int torque, const int time) {
  one.lcd2("     Brake    ");
  one.brake();  // Stop motors with torque
  delay(time);
}

void loop() {
  moveForwards(50, 1000);
  stop(500);
  moveBackwards(50, 1000);
  stop(500);
  rotateRight(50, 500);
  stop(500);
  rotateLeft(50, 500);
  stop(500);
  moveForwards(100, 1000);
  stop(500);
  moveBackwards(100, 1000);
  brake(100, 800);
  stop(1500);
}
