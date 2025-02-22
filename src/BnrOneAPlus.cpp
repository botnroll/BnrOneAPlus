/*
  BnrOneAPlus.cpp - Library for interfacing with Bot'n Roll ONE A+ Arduino
  Compatible from www.botnroll.com Created by José Cruz, December 18, 2024.
  Released into the public domain.
*/
#include "BnrOneAPlus.h"

#include "EEPROM.h"
#include "SPI.h"
#include "include/ArduinoCommands.h"
#include "include/SpiCommands.h"

#define DELAY_TR 20  // 20 MinStable:15  Crash:14
#define DELAY_SS 20  // 20 Crash: No crash even with 0 (ZERO)

void BnrOneAPlus::spiConnect(byte sspin) {
  _sspin = sspin;
  pinMode(_sspin, OUTPUT);

  // Initializes the SPI bus by setting SCK and MOSI to outputs, pulling SCK and
  // MOSI low.
  SPI.begin();
  // Sets the order of the bits shifted out of and into the SPI bus MSBFIRST
  // (most-significant bit first).
  SPI.setBitOrder(MSBFIRST);
  // Sets the SPI data mode: that is, clock polarity and phase.
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  // SPI in hold state by pulling SS high.
  digitalWrite(_sspin, HIGH);
  delayMicroseconds(DELAY_SS);
}

byte BnrOneAPlus::spiRequestByte(byte command) {
  byte value = (byte)0xFF;
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte numBytes = 2;
  // Select the SPI Slave device to start communication
  digitalWrite(_sspin, LOW);
  SPI.transfer(command);  // Sends one byte
  delayMicroseconds(DELAY_TR);

  for (i = 0; i < numBytes; ++i) {
    SPI.transfer(buffer[i]);  // Sends one byte
    delayMicroseconds(DELAY_TR);
  }
  value = SPI.transfer(0x00);  // Reads one byte
  digitalWrite(_sspin, HIGH);  // Closes communication with slave device.
  delayMicroseconds(DELAY_SS);

  return value;
}

int BnrOneAPlus::spiRequestWord(byte command) {
  byte value[2] = {0, 0};
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte numBytes = 2;
  digitalWrite(_sspin,
               LOW);      // Select the SPI Slave device to start communication.
  SPI.transfer(command);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (i = 0; i < numBytes; ++i) {
    SPI.transfer(buffer[i]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  for (i = 0; i < 2; ++i) {
    value[i] = SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(_sspin, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);
  i = 0;
  i = value[0];
  i = i << 8;
  i = i + value[1];
  return i;
}

float BnrOneAPlus::spiRequestFloat(byte command) {
  float f;
  char value[sizeof(float)];
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte numBytes = 2;
  digitalWrite(_sspin,
               LOW);      // Select the SPI Slave device to start communication.
  SPI.transfer(command);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (i = 0; i < numBytes; ++i) {
    SPI.transfer(buffer[i]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  for (i = 0; i < 4; ++i) {
    value[i] = (char)SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(_sspin, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);
  memcpy(&f, value, sizeof f);  // receive data
  return f;
}

void BnrOneAPlus::spiSendData(byte command, byte buffer[], byte numBytes) {
  // Select the SPI Slave device to start communication.
  digitalWrite(_sspin, LOW);
  SPI.transfer(command);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (int k = 0; k < numBytes; k++) {
    SPI.transfer(buffer[k]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(_sspin, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);
}

void BnrOneAPlus::move(int leftSpeed, int rightSpeed) {
  byte leftSpeed_H = highByte(leftSpeed);
  byte leftSpeed_L = lowByte(leftSpeed);
  byte rightSpeed_H = highByte(rightSpeed);
  byte rightSpeed_L = lowByte(rightSpeed);

  byte buffer[] = {
      KEY1, KEY2, leftSpeed_H, leftSpeed_L, rightSpeed_H, rightSpeed_L};
  spiSendData(COMMAND_MOVE, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::moveRAW(int leftDutyCycle, int rightDutyCycle) {
  byte leftPower_H = highByte(leftDutyCycle);
  byte leftPower_L = lowByte(leftDutyCycle);
  byte rightPower_H = highByte(rightDutyCycle);
  byte rightPower_L = lowByte(rightDutyCycle);

  byte buffer[] = {
      KEY1, KEY2, leftPower_H, leftPower_L, rightPower_H, rightPower_L};
  spiSendData(COMMAND_MOVE_RAW, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::move1m(byte motorId, int speed) {
  byte speed_H = highByte(speed);
  byte speed_L = lowByte(speed);

  byte buffer[] = {KEY1, KEY2, motorId, speed_H, speed_L};
  spiSendData(COMMAND_MOVE_1M, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::stop() {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_STOP, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::stop1m(byte motorId) {
  byte buffer[] = {KEY1, KEY2, motorId};
  spiSendData(COMMAND_STOP_1M, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::brake(byte leftTorque, byte rightTorque) {
  byte buffer[] = {KEY1, KEY2, leftTorque, rightTorque};
  spiSendData(COMMAND_BRAKE_SET_T, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::brake1m(byte motorId, byte torque) {
  byte buffer[] = {KEY1, KEY2, motorId, torque};
  spiSendData(COMMAND_BRAKE_1M, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::brake() {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_BRAKE_MAX_T, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::resetLeftEncoder() {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_ENCL_RESET, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::resetEncR() {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_ENCR_RESET, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::setLed(boolean state) {
  byte buffer[] = {KEY1, KEY2, (byte)state};
  spiSendData(COMMAND_LED, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::obstacleSensorsEmitters(boolean state) {
  byte buffer[] = {KEY1, KEY2, (byte)state};
  spiSendData(COMMAND_IR_EMITTERS, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::setMinBatteryV(float batmin) {
  char data[sizeof(float)];              // 4bytes
  memcpy(data, &batmin, sizeof batmin);  // send data
  byte buffer[] = {
      KEY1, KEY2, (byte)data[0], (byte)data[1], (byte)data[2], (byte)data[3]};
  spiSendData(COMMAND_SET_BAT_MIN, buffer, sizeof(buffer));
  delay(25);  // Wait while command is processed
}

void BnrOneAPlus::setPid(int Kp, int Ki, int Kd) {
  byte Kp_H = highByte(Kp);
  byte Kp_L = lowByte(Kp);
  byte Ki_H = highByte(Ki);
  byte Ki_L = lowByte(Ki);
  byte Kd_H = highByte(Kd);
  byte Kd_L = lowByte(Kd);
  byte buffer[] = {KEY1, KEY2, Kp_H, Kp_L, Ki_H, Ki_L, Kd_H, Kd_L};
  spiSendData(COMMAND_SET_PID, buffer, sizeof(buffer));
  delay(35);  // Delay for EEPROM writing
}

void BnrOneAPlus::setMotors(int SMPow, int Ks, int ctrlPulses) {
  byte SMPow_H = highByte(SMPow);
  byte SMPow_L = lowByte(SMPow);
  byte Ks_H = highByte(Ks);
  byte Ks_L = lowByte(Ks);
  byte ctrlPulses_H = highByte(ctrlPulses);
  byte ctrlPulses_L = lowByte(ctrlPulses);
  byte buffer[] = {
      KEY1, KEY2, SMPow_H, SMPow_L, Ks_H, Ks_L, ctrlPulses_H, ctrlPulses_L};
  spiSendData(COMMAND_SET_MOTORS, buffer, sizeof(buffer));
  delay(35);  // Delay for EEPROM writing
}

byte BnrOneAPlus::readButton() {
  int adc;
  byte button;
  adc = spiRequestWord(COMMAND_BUT_READ);
  if (adc >= 0 && adc < 100)  // 0-82
  {
    button = 1;
  } else if (adc >= 459 && adc < 571)  // 509-521
  {
    button = 2;
  } else if (adc >= 629 && adc < 737)  // 679-687
  {
    button = 3;
  } else {
    button = 0;
  }
  return button;
}

float BnrOneAPlus::readBattery() {
  return (float)((float)(spiRequestWord(COMMAND_BAT_READ)) / 50.7);
}

int BnrOneAPlus::readAndResetLeftEncoder() {
  return spiRequestWord(COMMAND_ENCL);
}

int BnrOneAPlus::readAndResetRightEncoder() {
  return spiRequestWord(COMMAND_ENCR);
}

int BnrOneAPlus::readAndIncrementLeftEncoder() {
  return spiRequestWord(COMMAND_ENCL_INC);
}

int BnrOneAPlus::readAndIncrementRightEncoder() {
  return spiRequestWord(COMMAND_ENCR_INC);
}

void BnrOneAPlus::readFirmware(byte *firm1, byte *firm2, byte *firm3) {
  byte value[3] = {0, 0, 0};
  int k = 0;
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_FIRMWARE,
              buffer,
              sizeof(buffer));  // Request data from master
  digitalWrite(_sspin,
               LOW);  // Select the SPI Slave device to start communication.
  delayMicroseconds(20);
  for (k = 0; k < 3; k++) {
    value[k] = SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(20);
  }
  *firm1 = value[0];
  *firm2 = value[1];
  *firm3 = value[2];
  digitalWrite(_sspin, HIGH);  // Close communication with slave device.
}

byte BnrOneAPlus::readObstacleSensors() {
  return spiRequestByte(COMMAND_OBSTACLES);
}

byte BnrOneAPlus::readLeftRangeSensor() {
  return spiRequestByte(COMMAND_RANGE_LEFT);
}

byte BnrOneAPlus::readRightRangeSensor() {
  return spiRequestByte(COMMAND_RANGE_RIGHT);
}

int BnrOneAPlus::readAdc(byte adcChannel) {
  byte command = 0x00;
  switch (adcChannel) {
    case 0:
      command = COMMAND_ADC0;
      break;
    case 1:
      command = COMMAND_ADC1;
      break;
    case 2:
      command = COMMAND_ADC2;
      break;
    case 3:
      command = COMMAND_ADC3;
      break;
    case 4:
      command = COMMAND_ADC4;
      break;
    case 5:
      command = COMMAND_ADC5;
      break;
    case 6:
      command = COMMAND_ADC6;
      break;
    case 7:
      command = COMMAND_ADC7;
      break;
  }
  return spiRequestWord(command);
}

int BnrOneAPlus::readDBG(byte index) {
  byte command = 0x00;
  switch (index) {
    case 0:
      command = 0xB9;
      break;
    case 1:
      command = 0xB8;
      break;
    case 2:
      command = 0xB7;
      break;
    case 3:
      command = 0xB6;
      break;
  }
  return spiRequestWord(command);
}

float BnrOneAPlus::readDBGf() {
  byte command = 0xB5;
  return spiRequestFloat(command);
}

/**************************************************************/
/**** LCD LINE 1 Handlers *************************************/
/**************************************************************/
void BnrOneAPlus::lcd1(String string) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(byte string[]) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const char string[]) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, "%s", string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(int number) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%d", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(unsigned int number) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%u", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(long int number) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%ld", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(double number) {
  int i;
  int intg;
  int dec;
  byte buffer[18];
  char string[19];
  bool flag_neg = 0;

  if (number < -0.0001) {
    flag_neg = 1;
    number *= -1.0;
  }
  dec = round((number - ((double)(int)number)) * 100.0) % 100;
  intg = (dec == 0 ? round(number) : (int)number);
  sprintf(string, "%d.%02d            ", intg, dec);

  buffer[0] = KEY1;
  buffer[1] = KEY2;
  if (flag_neg == 1) buffer[2] = '-';
  for (i = 0; i < 16; ++i) {
    buffer[i + 2 + flag_neg] = string[i];
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const char string[], int number) {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  b = sprintf(string1, "%d", number);
  for (i = 0; i < b; ++i) {
    buffer[i + a] = string1[i];
  }
  for (i = a + b; i < 18; ++i) {
    buffer[i] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const char string[], unsigned int number) {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  b = sprintf(string1, "%u", number);
  for (i = 0; i < b; ++i) {
    buffer[i + a] = string1[i];
  }
  for (i = a + b; i < 18; ++i) {
    buffer[i] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const char string[], long int number) {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  b = sprintf(string1, "%ld", number);
  for (i = 0; i < b; ++i) {
    buffer[i + a] = string1[i];
  }
  for (i = a + b; i < 18; ++i) {
    buffer[i] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const char string[], double number) {
  int i, a, b;
  char string1[19];
  char string2[19];
  byte buffer[18];
  int intg, dec;
  bool flag_neg = 0;

  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  if (number < -0.0001) {
    flag_neg = 1;
    number *= -1.0;
  }
  dec = round((number - ((double)(int)number)) * 100.0) % 100;
  intg = (dec == 0 ? round(number) : (int)number);
  b = sprintf(string2, "%d.%02d            ", intg, dec);
  if (flag_neg == 1) {
    buffer[a] = '-';
    a++;
  }
  for (i = 0; i < b; ++i) {
    if ((i + a) < 18) buffer[i + a] = string2[i];
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(unsigned char stringA[8], unsigned char stringB[8]) {
  int i, a;
  byte buffer[18];
  char string1[17], string2[17];
  for (i = 0; i < 16; ++i) {
    if (i < 8)
      string2[i] = stringA[i];
    else
      string2[i] = stringB[i - 8];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(unsigned int num1, unsigned int num2) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%u %u", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(unsigned int num1,
                       unsigned int num2,
                       unsigned int num3) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%u %u %u", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(unsigned int num1,
                       unsigned int num2,
                       unsigned int num3,
                       unsigned int num4) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%4u%4u%4u%4u", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(int num1, int num2) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%d %d", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(int num1, int num2, int num3) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%d %d %d", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(int num1, int num2, int num3, int num4) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%4d%4d%4d%4d", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

/**************************************************************/
/**** LCD LINE 2 Handlers *************************************/
/**************************************************************/
void BnrOneAPlus::lcd2(String string) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(byte string[]) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const char string[]) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(int number) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%d", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(unsigned int number) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%u", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(long int number) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%ld", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(double number) {
  int i;
  int intg;
  int dec;
  byte buffer[18];
  char string[19];
  bool flag_neg = 0;

  if (number < -0.0001) {
    flag_neg = 1;
    number *= -1.0;
  }
  dec = round((number - ((double)(int)number)) * 100.0) % 100;
  intg = (dec == 0 ? round(number) : (int)number);
  sprintf(string, "%d.%02d            ", intg, dec);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  if (flag_neg == 1) buffer[2] = '-';
  for (i = 0; i < 16; ++i) {
    buffer[i + 2 + flag_neg] = string[i];
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const char string[], int number) {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  b = sprintf(string1, "%d", number);
  for (i = 0; i < b; ++i) {
    buffer[i + a] = string1[i];
  }
  for (i = a + b; i < 18; ++i) {
    buffer[i] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const char string[], unsigned int number) {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  b = sprintf(string1, "%u", number);
  for (i = 0; i < b; ++i) {
    buffer[i + a] = string1[i];
  }
  for (i = a + b; i < 18; ++i) {
    buffer[i] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const char string[], long int number) {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  b = sprintf(string1, "%ld", number);
  for (i = 0; i < b; ++i) {
    buffer[i + a] = string1[i];
  }
  for (i = a + b; i < 18; ++i) {
    buffer[i] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const char string[], double number) {
  int i, a, b;
  char string1[19];
  char string2[19];
  byte buffer[18];
  int intg, dec;
  bool flag_neg = 0;

  for (i = 0; i < 16; ++i) {
    string2[i] = string[i];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  a += 2;
  if (number < -0.0001) {
    flag_neg = 1;
    number *= -1.0;
  }
  dec = round((number - ((double)(int)number)) * 100.0) % 100;
  intg = (dec == 0 ? round(number) : (int)number);
  b = sprintf(string2, "%d.%02d            ", intg, dec);
  if (flag_neg == 1) {
    buffer[a] = '-';
    a++;
  }
  for (i = 0; i < b; ++i) {
    if ((i + a) < 18) buffer[i + a] = string2[i];
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(unsigned char stringA[], unsigned char stringB[]) {
  int i, a;
  byte buffer[18];
  char string1[17], string2[17];
  for (i = 0; i < 16; ++i) {
    if (i < 8)
      string2[i] = stringA[i];
    else
      string2[i] = stringB[i - 8];
  }
  string2[16] = 0;
  a = sprintf(string1, string2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < a; ++i) {
    buffer[i + 2] = string1[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = ' ';
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(unsigned int num1, unsigned int num2) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%u %u", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(unsigned int num1,
                       unsigned int num2,
                       unsigned int num3) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%u %u %u", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(unsigned int num1,
                       unsigned int num2,
                       unsigned int num3,
                       unsigned int num4) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%4u%4u%4u%4u", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(int num1, int num2) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%d %d", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(int num1, int num2, int num3) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%d %d %d", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(int num1, int num2, int num3, int num4) {
  int i, a = 0;
  byte buffer[18];
  char string[17];
  a = sprintf(string, "%4d%4d%4d%4d", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

int *BnrOneAPlus::readLineSensor() {
  static int reading[8];
  byte value[16];
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte numBytes = 2;
  digitalWrite(_sspin,
               LOW);  // Select the SPI Slave device to start communication.
  SPI.transfer(COMMAND_LINE_READ);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (int i = 0; i < numBytes; ++i) {
    SPI.transfer(buffer[i]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  for (i = 0; i < 16; ++i) {
    value[i] = (char)SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(_sspin, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);

  for (i = 0; i < 8; ++i) {
    reading[i] = (int)value[i * 2];
    reading[i] = reading[i] << 8;
    reading[i] += (int)value[1 + (i * 2)];
  }
  return reading;
}

int BnrOneAPlus::readLine() {
  auto reading = readLineSensor();
  return _lineDetector.ComputeLine(reading);
}
