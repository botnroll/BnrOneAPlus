/*
  BnrCompass.h - Library for interfacing with the Compass for Bot'n Roll ONE A Plus
  (www.botnroll.com) Created by José  Cruz, February 20, 2025. v_1.0.0 Released into the public
  domain.
*/

#include "BnrCompass.h"

#include "Wire.h"

///////////////////////////////////////////////////////////////////////
// Private Routines
///////////////////////////////////////////////////////////////////////
void BnrCompass::i2cConnect(byte sensorAddress) {
  sensorAddress_ = sensorAddress;
  Wire.begin();  // join i2c bus (address optional for master)
}

float BnrCompass::readBearing() {
  byte highByte, lowByte;  // highByte and lowByte store the bearing and fine stores decimal place of bearing

  Wire.beginTransmission(sensorAddress_);  // start communication with CMPS11
  Wire.write(2);                           // Send the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(sensorAddress_, 2);  // Request 4 bytes from CMPS11

  unsigned long startTime = millis();
  while (Wire.available() < 2) {
    if (millis() - startTime > 100) {  // Timeout after 100ms
      // Serial.println("Timeout: Compass not responding.");
      return -1;  // Return an error value (e.g., -1)
    }
  }
  highByte = Wire.read();
  lowByte = Wire.read();

  return (float)((highByte << 8) + lowByte) / 10;
}

char BnrCompass::readRoll() {
  char roll;  // Store  roll values of CMPS11, chars are used because they support signed value

  Wire.beginTransmission(sensorAddress_);  // start communication with CMPS11
  Wire.write(5);                           // Send the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(sensorAddress_, 1);  // Request 4 bytes from CMPS11
  unsigned long startTime = millis();
  while (Wire.available() < 1) {
    if (millis() - startTime > 100) {  // Timeout after 100ms
      // Serial.println("Timeout: Compass not responding.");
      return -1;  // Return an error value (e.g., -1)
    }
  }
  roll = Wire.read();
  return roll;
}

char BnrCompass::readPitch() {
  char pitch;  // Store pitch values of CMPS11, chars are used because they support signed value

  Wire.beginTransmission(sensorAddress_);  // start communication with CMPS11
  Wire.write(4);                           // Send the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(sensorAddress_, 1);  // Request 4 bytes from CMPS11
  unsigned long startTime = millis();
  while (Wire.available() < 1) {
    if (millis() - startTime > 100) {  // Timeout after 100ms
      // Serial.println("Timeout: Compass not responding.");
      return -1;  // Return an error value (e.g., -1)
    }
  }
  pitch = Wire.read();

  return pitch;
}