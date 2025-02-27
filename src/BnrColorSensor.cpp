/*
  BnrColorSensor.cpp - Library for interfacing with Bot'n Roll Color Sensor for
  Bot'n Roll ONE A Plus (www.botnroll.com) Created by José Cruz, November 18,
  2024. v_1.0.0 Released into the public domain.
*/

#include "BnrColorSensor.h"

#include "Wire.h"

BnrColorSensor::BnrColorSensor(const byte sensor_address) {
  sensor_address_ = sensor_address;
  Wire.begin();  // join i2c bus (address optional for master)
}

void BnrColorSensor::i2cSendData(const byte command,
                                 byte buffer[],
                                 const byte num_bytes) {
  Wire.beginTransmission(sensor_address_);  // transmit to device #0x10
  Wire.write(command);                      // sends one byte
  for (int k = 0; k < num_bytes; k++) {
    Wire.write(buffer[k]);  // sends one byte
  }
  Wire.endTransmission();  // stop transmitting
}

void BnrColorSensor::request2Bytes(const byte command,
                                   byte& byte1,
                                   byte& byte2) {
  byte data[2] = {0, 0};
  int i = 0;
  Wire.beginTransmission(sensor_address_);  // transmit to device
  Wire.write(command);                      // sends the command
  Wire.endTransmission();                   // stop transmitting
  delay(1);                                 // This delay is very important!
  Wire.requestFrom((int)sensor_address_, (int)2);  // requests 2 bytes
  while (Wire.available()) {
    data[i] = Wire.read();
    i++;
  }
  byte1 = data[0];
  byte2 = data[1];
}

void BnrColorSensor::request3Bytes(const byte command,
                                   byte& byte1,
                                   byte& byte2,
                                   byte& byte3) {
  byte data[3] = {0, 0, 0};
  int i = 0;
  Wire.beginTransmission(sensor_address_);  // transmit to device
  Wire.write(command);                      // sends the command
  Wire.endTransmission();                   // stop transmitting
  delay(1);                                 // This delay is very important!
  Wire.requestFrom((int)sensor_address_, (int)3);  // requests 3 bytes
  while (Wire.available()) {
    data[i] = Wire.read();
    i++;
  }
  byte1 = data[0];
  byte2 = data[1];
  byte3 = data[2];
}

///////////////////////////////////////////////////////////////////////
// setup routines
///////////////////////////////////////////////////////////////////////
void BnrColorSensor::setSensorAddress(const byte new_address) {
  byte buffer[] = {new_address, KEY1, new_address, KEY2};
  i2cSendData(COMMAND_ADDRESS_CFG, buffer, sizeof(buffer));
  sensor_address_ = new_address;
  delay(2);
}
void BnrColorSensor::setRgbStatus(const byte status) const {
  byte buffer[] = {status, KEY1, status, KEY2};
  i2cSendData(COMMAND_RGB_CFG, buffer, sizeof(buffer));
  delay(2);
}

///////////////////////////////////////////////////////////////////////
// Data reading routines
///////////////////////////////////////////////////////////////////////
void BnrColorSensor::readFirmware(byte& firm1, byte& firm2) const {
  byte value[2] = {0};
  request2Bytes(COMMAND_FIRMWARE, value[0], value[1]);
  firm1 = value[0];
  firm2 = value[1];
}

void BnrColorSensor::readRGBL(byte& red, byte& green, byte& blue) {
  byte value[3] = {0};
  request3Bytes(COMMAND_RGBL, value[0], value[1], value[2]);
  red = value[0];
  green = value[1];
  blue = value[2];
}

void BnrColorSensor::readRGBR(byte& red, byte& green, byte& blue) {
  byte value[3] = {0};
  request3Bytes(COMMAND_RGBR, value[0], value[1], value[2]);
  red = value[0];
  green = value[1];
  blue = value[2];
}
