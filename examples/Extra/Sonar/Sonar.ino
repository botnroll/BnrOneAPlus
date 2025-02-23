/**
 * HC-SR04 Ping distance sensor:
 * VCC to arduino 5v
 * GND to arduino GND
 * Echo to Arduino pin 6
 * Trig to Arduino pin 7
 *
 * This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 *
 * This example was created by José Cruz (www.botnroll.com)
 * on 18 December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>          // required by BnrOneAPlus.cpp

// constants definitions
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

#define echo_pin 6  // Echo Pin
#define trig_pin 7  // Trigger Pin

BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

#define maximum_range 200  // Maximum range (200cm)
#define minimum_range 0    // Minimum range

int Sonar() {
  unsigned long duration;  // Duration used to calculate distance
  int distance;
  unsigned long tempo = micros();

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH, 11640);
  // this routine has fixed time (16 milliseconds)
  delayMicroseconds(16000 - (micros() - tempo));

  // Calculate the distance (in cm) based on the speed of sound
  distance = (int)(duration / 58.2);
  if ((distance >= maximum_range) || (distance <= minimum_range)) {
    distance = -1;
  }

  return distance;
}

void setup() {
  // setup routines
  Serial.begin(115200);    // set baud rate to 115200bps for printing values in
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI module
  one.stop();             // stop motors

  one.lcd1("www.botnroll.com");
  one.lcd2(" ");

  // SONAR setup
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loop() {
  long distance = Sonar();
  one.lcd2("distance: ", distance);
  Serial.println(distance);
  delay(50);
}
