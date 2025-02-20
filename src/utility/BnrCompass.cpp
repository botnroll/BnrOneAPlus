/*
  BnrCompass.h - Library for interfacing with the Compass for Bot'n Roll ONE A Plus (www.botnroll.com)
  Created by José  Cruz, February 20, 2025.
  v_1.0.0
  Released into the public domain.
*/

#include "Wire.h"
#include "BnrCompass.h"
///////////////////////////////////////////////////////////////////////
//Private Routines
///////////////////////////////////////////////////////////////////////
void BnrCompass::i2cConnect(byte sensorAddress)
{
  _sensorAddress = sensorAddress;
	Wire.begin();                           // join i2c bus (address optional for master)
}


float BnrCompass::read_bearing()
{
  byte highByte, lowByte;  // highByte and lowByte store the bearing and fine stores decimal place of bearing

   Wire.beginTransmission(_sensorAddress);    //start communication with CMPS11
   Wire.write(2);                      //Send the register we wish to start reading from
   Wire.endTransmission();

   Wire.requestFrom(_sensorAddress, 2);       // Request 4 bytes from CMPS11
   while(Wire.available() < 2);        // Wait for bytes to become available
   highByte = Wire.read();
   lowByte = Wire.read();
   
  return (float)((highByte<<8)+lowByte)/10;
}

char BnrCompass::read_roll()
{
  char roll;       // Store  roll values of CMPS11, chars are used because they support signed value

  Wire.beginTransmission(_sensorAddress);     //start communication with CMPS11
  Wire.write(5);                       //Send the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(_sensorAddress, 1);        // Request 4 bytes from CMPS11
  while(Wire.available() < 1);         // Wait for bytes to become available
  roll = Wire.read();
  return roll;
}

char BnrCompass::read_pitch()
{
  char pitch;       // Store pitch values of CMPS11, chars are used because they support signed value

   Wire.beginTransmission(_sensorAddress);     //start communication with CMPS11
   Wire.write(4);                       //Send the register we wish to start reading from
   Wire.endTransmission();

   Wire.requestFrom(_sensorAddress, 1);        // Request 4 bytes from CMPS11
   while(Wire.available() < 1);         // Wait for bytes to become available
   pitch = Wire.read();

  return pitch;
}