#include "BnrOneAPlus.h"

#include "EEPROM.h"
#include "SPI.h"
#include "include/ArduinoCommands.h"
#include "include/SpiCommands.h"

#define DELAY_TR 20  // 20 MinStable:15  Crash:14
#define DELAY_SS 20  // 20 Crash: No crash even with 0 (ZERO)

void BnrOneAPlus::spiConnect(const byte sspin) {
  sspin_ = sspin;
  pinMode(sspin_, OUTPUT);

  // Initializes the SPI bus by setting SCK and MOSI to outputs,
  // pulling SCK and MOSI low.
  SPI.begin();
  // Sets the order of the bits shifted out of and into the SPI bus MSBFIRST
  // (most-significant bit first).
  SPI.setBitOrder(MSBFIRST);
  // Sets the SPI data mode: that is, clock polarity and phase.
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  // SPI in hold state by pulling SS high.
  digitalWrite(sspin_, HIGH);
  delayMicroseconds(DELAY_SS);
}

byte BnrOneAPlus::spiRequestByte(const byte command) const {
  byte value = (byte)0xFF;
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte num_bytes = 2;
  // Select the SPI Slave device to start communication
  digitalWrite(sspin_, LOW);
  SPI.transfer(command);  // Sends one byte
  delayMicroseconds(DELAY_TR);

  for (i = 0; i < num_bytes; ++i) {
    SPI.transfer(buffer[i]);  // Sends one byte
    delayMicroseconds(DELAY_TR);
  }
  value = SPI.transfer(0x00);  // Reads one byte
  digitalWrite(sspin_, HIGH);  // Closes communication with slave device.
  delayMicroseconds(DELAY_SS);

  return value;
}

int BnrOneAPlus::spiRequestWord(const byte command) const {
  byte value[2] = {0, 0};
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte num_bytes = 2;
  // Select the SPI Slave device to start communication.
  digitalWrite(sspin_, LOW);
  SPI.transfer(command);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (i = 0; i < num_bytes; ++i) {
    SPI.transfer(buffer[i]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  for (i = 0; i < 2; ++i) {
    value[i] = SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(sspin_, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);
  i = 0;
  i = value[0];
  i = i << 8;
  i = i + value[1];

  return i;
}

float BnrOneAPlus::spiRequestFloat(const byte command) const {
  float f;
  char value[sizeof(float)];
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte num_bytes = 2;
  // Select the SPI Slave device to start communication.
  digitalWrite(sspin_, LOW);
  SPI.transfer(command);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (i = 0; i < num_bytes; ++i) {
    SPI.transfer(buffer[i]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  for (i = 0; i < 4; ++i) {
    value[i] = (char)SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(sspin_, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);
  memcpy(&f, value, sizeof f);  // receive data

  return f;
}

void BnrOneAPlus::spiSendData(const byte command,
                              const byte buffer[],
                              const byte num_bytes) const {
  // Select the SPI Slave device to start communication.
  digitalWrite(sspin_, LOW);
  SPI.transfer(command);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (int k = 0; k < num_bytes; k++) {
    SPI.transfer(buffer[k]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(sspin_, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);
}

void BnrOneAPlus::move(const int left_speed, const int right_speed) const {
  byte leftSpeed_H = highByte(left_speed);
  byte leftSpeed_L = lowByte(left_speed);
  byte rightSpeed_H = highByte(right_speed);
  byte rightSpeed_L = lowByte(right_speed);

  byte buffer[] = {
      KEY1, KEY2, leftSpeed_H, leftSpeed_L, rightSpeed_H, rightSpeed_L};
  spiSendData(COMMAND_MOVE, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::moveRAW(const int left_duty_cycle,
                          const int right_duty_cycle) const {
  byte leftPower_H = highByte(left_duty_cycle);
  byte leftPower_L = lowByte(left_duty_cycle);
  byte rightPower_H = highByte(right_duty_cycle);
  byte rightPower_L = lowByte(right_duty_cycle);

  byte buffer[] = {
      KEY1, KEY2, leftPower_H, leftPower_L, rightPower_H, rightPower_L};
  spiSendData(COMMAND_MOVE_RAW, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::move1m(const byte motor_id, const int speed) const {
  byte speed_H = highByte(speed);
  byte speed_L = lowByte(speed);

  byte buffer[] = {KEY1, KEY2, motor_id, speed_H, speed_L};
  spiSendData(COMMAND_MOVE_1M, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::stop() const {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_STOP, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::stop1m(const byte motor_id) const {
  byte buffer[] = {KEY1, KEY2, motor_id};
  spiSendData(COMMAND_STOP_1M, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::brake(const byte left_torque, const byte right_torque) const {
  byte buffer[] = {KEY1, KEY2, left_torque, right_torque};
  spiSendData(COMMAND_BRAKE_SET_T, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::brake1m(const byte motor_id, const byte torque) const {
  byte buffer[] = {KEY1, KEY2, motor_id, torque};
  spiSendData(COMMAND_BRAKE_1M, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::brake() const {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_BRAKE_MAX_T, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::resetLeftEncoder() {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_ENCL_RESET, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::resetRightEncoder() const {
  byte buffer[] = {KEY1, KEY2};
  spiSendData(COMMAND_ENCR_RESET, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::setLed(const boolean state) const {
  byte buffer[] = {KEY1, KEY2, (byte)state};
  spiSendData(COMMAND_LED, buffer, sizeof(buffer));
  delay(2);  // Wait while command is processed
}

void BnrOneAPlus::obstacleSensorsEmitters(const boolean state) const {
  byte buffer[] = {KEY1, KEY2, (byte)state};
  spiSendData(COMMAND_IR_EMITTERS, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::setMinBatteryV(const float min_battery_V) {
  char data[sizeof(float)];                            // 4bytes
  memcpy(data, &min_battery_V, sizeof min_battery_V);  // send data
  byte buffer[] = {
      KEY1, KEY2, (byte)data[0], (byte)data[1], (byte)data[2], (byte)data[3]};
  spiSendData(COMMAND_SET_BAT_MIN, buffer, sizeof(buffer));
  delay(25);  // Wait while command is processed
}

void BnrOneAPlus::setPid(const int &kp, const int &ki, const int &kd) const {
  byte buffer[] = {KEY1,
                   KEY2,
                   highByte(kp),
                   lowByte(kp),
                   highByte(ki),
                   lowByte(ki),
                   highByte(kd),
                   lowByte(kd)};
  spiSendData(COMMAND_SET_PID, buffer, sizeof(buffer));
  delay(35);  // Delay for EEPROM writing
}

void BnrOneAPlus::setMotors(const int motor_power,
                            const int ks,
                            const int ctrl_pulses) const {
  const byte buffer[] = {KEY1,
                         KEY2,
                         highByte(motor_power),
                         lowByte(motor_power),
                         highByte(ks),
                         lowByte(ks),
                         highByte(ctrl_pulses),
                         lowByte(ctrl_pulses)};
  spiSendData(COMMAND_SET_MOTORS, buffer, sizeof(buffer));
  delay(35);  // Delay for EEPROM writing
}

byte BnrOneAPlus::readButton() const {
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

float BnrOneAPlus::readBattery() const {
  return (float)((float)(spiRequestWord(COMMAND_BAT_READ)) / 50.7);
}

int BnrOneAPlus::readAndResetLeftEncoder() const {
  return spiRequestWord(COMMAND_ENCL);
}

int BnrOneAPlus::readAndResetRightEncoder() const {
  return spiRequestWord(COMMAND_ENCR);
}

int BnrOneAPlus::readAndIncrementLeftEncoder() const {
  return spiRequestWord(COMMAND_ENCL_INC);
}

int BnrOneAPlus::readAndIncrementRightEncoder() const {
  return spiRequestWord(COMMAND_ENCR_INC);
}

void BnrOneAPlus::readFirmware(const byte *firm1,
                               const byte *firm2,
                               const byte *firm3) const {
  byte value[3] = {0, 0, 0};
  int k = 0;
  byte buffer[] = {KEY1, KEY2};
  // Request data from master
  spiSendData(COMMAND_FIRMWARE, buffer, sizeof(buffer));
  // Select the SPI Slave device to start communication.
  digitalWrite(sspin_, LOW);
  delayMicroseconds(20);
  for (k = 0; k < 3; k++) {
    value[k] = SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(20);
  }
  *firm1 = value[0];
  *firm2 = value[1];
  *firm3 = value[2];
  digitalWrite(sspin_, HIGH);  // Close communication with slave device.
}

byte BnrOneAPlus::readObstacleSensors() const {
  return spiRequestByte(COMMAND_OBSTACLES);
}

byte BnrOneAPlus::readLeftRangeSensor() const {
  return spiRequestByte(COMMAND_RANGE_LEFT);
}

byte BnrOneAPlus::readRightRangeSensor() const {
  return spiRequestByte(COMMAND_RANGE_RIGHT);
}

int BnrOneAPlus::readAdc(const byte adc_channel) const {
  byte command = 0x00;
  switch (adc_channel) {
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

int BnrOneAPlus::readDBG(const byte index) const {
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
void BnrOneAPlus::lcd1(const String &string_in) const {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd1(byte string_in[]) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd1(const char string_in[]) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd1(const int number) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%d", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const unsigned int number) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%u", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const long int number) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%ld", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const double number) const {
  int i;
  int intg;
  int dec;
  byte buffer[18];
  char string_in[19];
  bool flag_neg = 0;

  if (number < -0.0001) {
    flag_neg = 1;
    number *= -1.0;
  }
  dec = round((number - ((double)(int)number)) * 100.0) % 100;
  intg = (dec == 0 ? round(number) : (int)number);
  sprintf(string_in, "%d.%02d            ", intg, dec);

  buffer[0] = KEY1;
  buffer[1] = KEY2;
  if (flag_neg == 1) buffer[2] = '-';
  for (i = 0; i < 16; ++i) {
    buffer[i + 2 + flag_neg] = string_in[i];
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const char string_in[], const int number) const {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd1(const char string_in[],
                       const unsigned int number) const {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd1(const char string_in[], const long int number) const {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd1(const char string_in[], const double number) const {
  int i, a, b;
  char string1[19];
  char string2[19];
  byte buffer[18];
  int intg, dec;
  bool flag_neg = 0;

  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd1(const unsigned char string_a[8],
                       const unsigned char string_b[8]) const {
  int i, a;
  byte buffer[18];
  char string1[17], string2[17];
  for (i = 0; i < 16; ++i) {
    if (i < 8)
      string2[i] = string_a[i];
    else
      string2[i] = string_b[i - 8];
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

void BnrOneAPlus::lcd1(const unsigned int num1, const unsigned int num2) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%u %u", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const unsigned int num1,
                       const unsigned int num2,
                       const unsigned int num3) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%u %u %u", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const unsigned int num1,
                       const unsigned int num2,
                       const unsigned int num3,
                       const unsigned int num4) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%4u%4u%4u%4u", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const int num1, const int num2) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%d %d", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const int num1, const int num2, const int num3) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%d %d %d", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L1, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd1(const int num1,
                       const int num2,
                       const int num3,
                       const int num4) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%4d%4d%4d%4d", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
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
void BnrOneAPlus::lcd2(const String &string_in) const {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd2(const byte string_in[]) const {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd2(const char string_in[]) {
  int i, a;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd2(const int number) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%d", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const unsigned int number) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%u", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const long int number) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%ld", number);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const double number) const {
  int i;
  int intg;
  int dec;
  byte buffer[18];
  char string_in[19];
  bool flag_neg = 0;

  if (number < -0.0001) {
    flag_neg = 1;
    number *= -1.0;
  }
  dec = round((number - ((double)(int)number)) * 100.0) % 100;
  intg = (dec == 0 ? round(number) : (int)number);
  sprintf(string_in, "%d.%02d            ", intg, dec);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  if (flag_neg == 1) buffer[2] = '-';
  for (i = 0; i < 16; ++i) {
    buffer[i + 2 + flag_neg] = string_in[i];
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const char string_in[], const int number) const {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd2(const char string_in[],
                       const unsigned int number) const {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd2(const char string_in[], const long int number) const {
  int i, a, b;
  byte buffer[18];
  char string1[19], string2[19];
  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd2(const char string_in[], const double number) const {
  int i, a, b;
  char string1[19];
  char string2[19];
  byte buffer[18];
  int intg, dec;
  bool flag_neg = 0;

  for (i = 0; i < 16; ++i) {
    string2[i] = string_in[i];
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

void BnrOneAPlus::lcd2(const unsigned char string_a[],
                       const unsigned char string_b[]) const {
  int i, a;
  byte buffer[18];
  char string1[17], string2[17];
  for (i = 0; i < 16; ++i) {
    if (i < 8)
      string2[i] = string_a[i];
    else
      string2[i] = string_b[i - 8];
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

void BnrOneAPlus::lcd2(const unsigned int num1, const unsigned int num2) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%u %u", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const unsigned int num1,
                       const unsigned int num2,
                       const unsigned int num3) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%u %u %u", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const unsigned int num1,
                       const unsigned int num2,
                       const unsigned int num3,
                       const unsigned int num4) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%4u%4u%4u%4u", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const int num1, const int num2) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%d %d", num1, num2);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const int num1, const int num2, const int num3) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%d %d %d", num1, num2, num3);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

void BnrOneAPlus::lcd2(const int num1,
                       const int num2,
                       const int num3,
                       const int num4) const {
  int i, a = 0;
  byte buffer[18];
  char string_in[17];
  a = sprintf(string_in, "%4d%4d%4d%4d", num1, num2, num3, num4);
  buffer[0] = KEY1;
  buffer[1] = KEY2;
  for (i = 0; i < 16; ++i) {
    buffer[i + 2] = string_in[i];
  }
  for (i = a; i < 16; ++i) {
    buffer[i + 2] = (' ');
  }
  spiSendData(COMMAND_LCD_L2, buffer, sizeof(buffer));
  delay(4);  // Wait while command is processed
}

int *BnrOneAPlus::readLineSensor() const {
  static int reading[8];
  byte value[16];
  int i;
  byte buffer[] = {KEY1, KEY2};
  byte num_bytes = 2;
  // Select the SPI Slave device to start communication.
  digitalWrite(sspin_, LOW);
  SPI.transfer(COMMAND_LINE_READ);  // Send one byte
  delayMicroseconds(DELAY_TR);
  for (int i = 0; i < num_bytes; ++i) {
    SPI.transfer(buffer[i]);  // Send one byte
    delayMicroseconds(DELAY_TR);
  }
  for (i = 0; i < 16; ++i) {
    value[i] = (char)SPI.transfer(0x00);  // Reads one byte
    delayMicroseconds(DELAY_TR);
  }
  digitalWrite(sspin_, HIGH);  // Close communication with slave device.
  delayMicroseconds(DELAY_SS);

  for (i = 0; i < 8; ++i) {
    reading[i] = (int)value[i * 2];
    reading[i] = reading[i] << 8;
    reading[i] += (int)value[1 + (i * 2)];
  }

  return reading;
}

int BnrOneAPlus::readLine() {
  const auto reading = readLineSensor();

  return line_detector_.ComputeLine(reading);
}
