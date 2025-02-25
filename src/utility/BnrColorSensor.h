/*
  BnrColorSensor.h - Library for interfacing with Bot'n Roll Color Sensor for Bot'n Roll ONE A Plus (www.botnroll.com)
  Created by José  Cruz, November 18, 2024.
  v_1.0.0
  Released into the public domain.
*/

#pragma once

#include "Arduino.h"

#define KEY1 0xAA  // key used in critical commands
#define KEY2 0x55  // key used in critical commands
#define OFF 0
#define ON 1
#define DISABLE 0
#define ENABLE 1

/*User Commands*/
#define COMMAND_FIRMWARE 0xFE     // Read firmware version
#define COMMAND_RGBL 0xCF         // Read RGB sensor1
#define COMMAND_RGBR 0xCE         // Read RGB sensor2
#define COMMAND_SONARS 0xCC       // Read sonar sensors
#define COMMAND_ADDRESS_CFG 0xF0  // Change I2C slave address
#define COMMAND_SONAR_CFG 0xEF    // Turn ON/OFF sonars
#define COMMAND_RGB_CFG 0xEE      // Turn ON/OFF RGB color sensors

class BnrColorSensor {
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
  void i2cConnect(byte sensorAddress);  // Enable I2C communication

  /**
   * @brief Sets a new I2C Address on the Color sensor
   *
   * @param newAddress
   * @return void
   */
  void setSensorAddress(byte newAddress);  // Change I2C Address

  /**
   * @brief Sets the status of the RGB Sensor enabling or disabling the RGB scanning
   *
   * @param status
   * @return void
   */
  void setRgbStatus(byte status);  // Enable/Disable RGB scanning

  // reading routines

  /**
   * @brief Reads the current Sensor Firmware and writes it on the bytes given as parameter
   *
   * @param byte* major version
   * @param byte* minor version
   * @return void
   */
  void readFirmware(byte*, byte*);

  /**
   * @brief Reads the current RGB values of the left Sensor and writes it on the bytes given as parameter
   *
   * @param byte* Red Value
   * @param byte* Green Value
   * @param byte* Blue Value
   * @return void
   */
  void readRGBL(byte*, byte*, byte*);

  /**
   * @brief Reads the current RGB values of the right Sensor and writes it on the bytes given as parameter
   *
   * @param byte* Red Value
   * @param byte* Green Value
   * @param byte* Blue Value
   * @return void
   */
  void readRGBR(byte*, byte*, byte*);

 private:
  /**
   * @brief Sends the selected command to the Color Sensor
   *
   * @param byte command
   * @param byte[] buffer
   * @param byte numBytes
   * @return void
   */
  void i2cSendData(byte command, byte buffer[], byte numBytes);

  /**
   * @brief Requests 2 bytes from the Color Sensor
   *
   * @param byte*
   * @param byte*
   * @param byte*
   * @return void
   */
  void request2Bytes(byte, byte*, byte*);

  /**
   * @brief Requests 2 bytes from the Color Sensor
   *
   * @param byte*
   * @param byte*
   * @param byte*
   * @param byte*
   * @return void
   */
  void request3Bytes(byte, byte*, byte*, byte*);

  byte sensorAddress_;
};
