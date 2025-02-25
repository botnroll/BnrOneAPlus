/**
 * This example was created by José Cruz (www.botnroll.com)
 * on 18 December 2024
 *
 * Updated on February 2025 by José Cruz
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * This example demonstrates the use of servos that can be used in a Pan & Tilt
 * system or a Gripper.
 *
 *
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
#include <Servo.h>
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

Servo g_servo1;
Servo g_servo2;

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.lcd1("Bot'n Roll ONE A");
  one.lcd2("www.botnroll.com");
  delay(1000);

  g_servo1.attach(3);
  g_servo2.attach(5);
}

void servo_open() { g_servo2.write(120); }

void servo_close() { g_servo2.write(0); }

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

  one.lcd1("servo 1: ", pos_servo1);
  one.lcd2("servo 2: ", pos_servo2);
  switch (servo) {
    case 1:
      g_servo1.write(pos_servo1);
      break;
    case 2:
      g_servo2.write(pos_servo2);
      break;
  }
  delay(200);
}
