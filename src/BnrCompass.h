/*
  BnrCompass.h - Library for interfacing with the Compass for Bot'n Roll ONE A
  Plus (www.botnroll.com) Created by José  Cruz, February 20, 2025. v_1.0.0
  Released into the public domain.
*/

#pragma once

#include "Arduino.h"

class BnrCompass {
 public:
  /********************************
   * @brief  setup routines        *
   *********************************/

  /**
   * @brief Initializes the I2C communication given an Address
   *
   * @param sensorAddress
   * @return void
   */
  BnrCompass(const byte sensor_address);  // Enable I2C communication

  /**
   * @brief Reads the current bearing of the Compass
   *
   * @return float
   */
  float readBearing() const;

  /**
   * @brief Reads the current roll value of the Compass
   *
   * @return char
   */
  char readRoll() const;

  /**
   * @brief Reads the current pitch value of the Compass
   *
   * @return char
   */
  char readPitch() const;

 private:
  byte sensor_address_;
};
