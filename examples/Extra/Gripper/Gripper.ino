/**
 * This example was created by José Cruz (www.botnroll.com)
 * on 18 December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * This example demonstrates the use of the Gripper.
 *
 * NOTE:
 * Gripper1 values vary between  80 - 160 (upwards - downwards) - (130
 * corresponds to Horizontal) Gripper2 values vary between  18 - 120 (closed -
 * opened)
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
#include <servo.h>
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

servo gripper1;
servo gripper2;

void setup() {
  Serial.begin(115200);    // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.lcd1("Bot'n Roll ONE A");
  one.lcd2("www.botnroll.com");
  delay(1000);

  gripper1.attach(3);
  gripper2.attach(5);
}

void gripper_open() { gripper2.write(120); }

void gripper_close() { gripper2.write(0); }

void loop() {
  static int pos_servo1 = 140;
  static int pos_servo2 = 120;
  static int servo = 1;
  int button;

  button = one.readButton();
  switch (button) {
    case 1:
      if (servo == 1)
        pos_servo1 += 5;
      else
        pos_servo2 += 5;
      break;
    case 2:
      if (servo == 1)
        pos_servo1 -= 5;
      else
        pos_servo2 -= 5;
      break;
    case 3:
      servo++;
      if (servo > 2) servo = 1;
      break;
  }

  if (pos_servo1 > 200) pos_servo1 = 200;
  if (pos_servo2 > 200) pos_servo2 = 200;
  if (pos_servo1 < 0) pos_servo1 = 0;
  if (pos_servo2 < 0) pos_servo2 = 0;

  one.lcd1("Gripper 1: ", pos_servo1);
  one.lcd2("Gripper 2: ", pos_servo2);
  switch (servo) {
    case 1:
      gripper1.write(pos_servo1);
      break;
    case 2:
      gripper2.write(pos_servo2);
      break;
  }
  delay(200);
}
