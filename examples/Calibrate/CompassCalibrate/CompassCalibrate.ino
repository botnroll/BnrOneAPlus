/**
 * This example was created by José Cruz (www.botnroll.com)
 * on 18 December 2024
 *
 * How the program works:
 * This example calibrates the CMPS11 compass connected to Bot'n Roll ONE A I2C
 * BUS. Also works for CMPS10 compass. Follow the instructions on the LCD and
 * press the push buttons accordingly. The robot will rotate a few seconds
 * calibrating the compass.
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
#include <Wire.h>

BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2            // Slave Select (SS) pin for SPI communication
#define CMPS_ADDRESS 0x60  // Define CMPS_ADDRESS of CMPS11

void setup() {
  Wire.begin();  // Start the I2C bus
  Serial.begin(57600);
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  delay(500);
}

float readBearing() {
  byte high_byte, low_byte;  // high_byte and low_byte store the bearing and
                             // fine stores decimal place of bearing

  Wire.beginTransmission(CMPS_ADDRESS);  // start communication with CMPS11
  Wire.write(2);  // Send the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(CMPS_ADDRESS, 2);  // Request 4 bytes from CMPS11
  while (Wire.available() < 2);       // Wait for bytes to become available
  high_byte = Wire.read();
  low_byte = Wire.read();

  return (float)((high_byte << 8) + low_byte) / 10;
}

char readRoll() {
  char roll;  // Store  roll value of CMPS11, chars are used because they
              // support signed value

  Wire.beginTransmission(CMPS_ADDRESS);  // start communication with CMPS11
  Wire.write(5);  // Send the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(CMPS_ADDRESS, 1);  // Request 4 bytes from CMPS11
  while (Wire.available() < 1);       // Wait for bytes to become available
  roll = Wire.read();
  return roll;
}

char readPitch() {
  char pitch;  // Store pitch value of CMPS11, chars are used because they
               // support signed value

  Wire.beginTransmission(CMPS_ADDRESS);  // start communication with CMPS11
  Wire.write(4);  // Send the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(CMPS_ADDRESS, 1);  // Request 4 bytes from CMPS11
  while (Wire.available() < 1);       // Wait for bytes to become available
  pitch = Wire.read();

  return pitch;
}

void calibrateCMPS11() {
  one.move(-30, 30);  // Slowly rotate the compass on the horizontal plane in
                      // all directions

  Wire.beginTransmission(CMPS_ADDRESS);  // start communication with CMPS11
  Wire.write(0);     // Send the register we wish to start reading from
  Wire.write(0xF0);  // Calibration sequence byte 1
  Wire.endTransmission();
  delay(30);

  Wire.beginTransmission(CMPS_ADDRESS);  // start communication with CMPS11
  Wire.write(0);     // Send the register we wish to start reading from
  Wire.write(0xF5);  // Calibration sequence byte 2
  Wire.endTransmission();
  delay(30);

  Wire.beginTransmission(CMPS_ADDRESS);  // start communication with CMPS11
  Wire.write(0);     // Send the register we wish to start reading from
  Wire.write(0xF7);  // Calibration sequence byte 2
  Wire.endTransmission();
  delay(30);

  one.move(-20, 20);  // Slowly rotate the compass on the horizontal plane in
                      // all directions
  delay(15000);

  Wire.beginTransmission(CMPS_ADDRESS);  // start communication with CMPS11
  Wire.write(0);     // Send the register we wish to start reading from
  Wire.write(0xF8);  // Exit calibration mode
  Wire.endTransmission();
  delay(30);
  one.move(0, 0);  // Stop rotation
}

void compassRead() {
  float bearing;
  char roll;
  char pitch;
  char temp[20];

  bearing = readBearing();
  roll = readRoll();
  pitch = readPitch();

  Serial.print("Bearing:");
  Serial.print(bearing);
  Serial.print("   roll:");
  Serial.print((int)roll);
  Serial.print("   pitch:");
  Serial.println((int)pitch);

  one.lcd1("Bearing: ", bearing);
  sprintf(temp, "Rol:%d Pit:%d      ", (int)roll, (int)pitch);
  one.lcd2(temp);
}

void loop() {
  float bearing;
  char roll;
  char pitch;
  char temp[20];

  one.lcd1("   Press PB1   ");
  one.lcd2(" to calibrate  ");
  while (one.readButton() != 1) delay(50);

  one.lcd1("  Calibrating");
  one.lcd2("   Compass... ");
  delay(1000);
  calibrateCMPS11();

  one.lcd1("  Calibrating");
  one.lcd2("    Finnished");
  delay(1000);

  while (one.readButton() != 1) {
    compassRead();
    delay(100);
  }
}
