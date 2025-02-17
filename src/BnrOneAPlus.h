/*
  BnrOneAPlus.h - Library for interfacing with Bot'n Roll ONE A+ Arduino
  Compatible from www.botnroll.com Released into public domain.
*/

#pragma once BnrOneAPlus_h

#include <string.h>

#include "Arduino.h"
#include "utility/LineDetector.h"

class BnrOneAPlus {
 public:
  /********************************
   * @brief  setup routines        *
   *********************************/

  /**
   * @brief establishes a connection with the Bot'n Roll ONE A+ board
   *
   * @param sspin
   */
  void spiConnect(byte sspin);

  /**
   * @brief Set the minimum battery voltage for battery protection
   *
   * @param minBatteryV
   */
  void setMinBatteryV(float minBatteryV);

  /**
   * @brief Set the PID parameters for PID control
   *
   * @param Kp proportional gain
   * @param Ki integral gain
   * @param Kd differential gain
   */
  void setPid(int Kp, int Ki, int Kd);

  /**
   * @brief Set the Motors configuration params:
   *    - moving power
   *    - ks gain
   *    - number of pulses per iteration at max speed
   *
   * @param movingPower
   * @param ks
   * @param pulsesAtMaxSpeed
   */
  void setMotors(int movingPower, int ks, int pulsesAtMaxSpeed);

  /**
   * @brief Turn on/off the obstacle sensors IR emitters
   *
   * @param state
   */
  void obstacleSensorsEmitters(bool state);

  /********************************
   * @brief  reading routines      *
   *********************************/

  /**
   * @brief reads the value of the obstacle sensors
   *
   * @return float
   *   0 if no obstacles are detected
   *   1 if obstacle on the left side only
   *   2 if obstacle on the right side only
   *   3 if obstacles on both sides
   */
  byte readObstacleSensors();

  /**
   * @brief reads the value of the left range sensor
   *
   * @return byte
   */
  byte readLeftRangeSensor();

  /**
   * @brief reads the value of the right range sensor
   *
   * @return byte
   */
  byte readRightRangeSensor();

  /**
   * @brief reads the value of the specified ADC channel
   *
   * @param adcChannel id of the Adc channel to read
   * @return int
   */
  int readAdc(byte adcChannel);

  /**
   * @brief reads the value of the button pressed
   *
   * @return byte
   *  0 if no button is pressed
   *  1 if button 1 is pressed
   *  2 if button 2 is pressed
   *  3 if button 3 is pressed
   */
  byte readButton();

  /**
   * @brief reads the battery voltage
   *
   * @return float
   */
  float readBattery();

  /**
   * @brief computes a line value in the range [-100, 100]
   */
  int readLine();

  /**
   * @brief reads the line sensor and outputs a vector of 8 integers
   */
  int* readLineSensor();

  /**
   * @brief reads the value of the left encoder and resets its value
   *
   * @return int
   */
  int readAndResetLeftEncoder();


  /**
   * @brief reads the value of the right encoder and resets its value
   *
   * @return int
   */
  int readAndResetRightEncoder();

  /**
   * @brief reads the value of the left encoder and keep it's current value
   * so that it increments over time
   *
   * @return int
   */
  int readAndIncrementLeftEncoder();

  /**
   * @brief reads the value of the right encoder and keep it's current value
   * so that it increments over time
   *
   * @return int
   */
  int readAndIncrementRightEncoder();

  /**
   * @brief reads the value of the debug registers
   *
   * @return int
   */
  int readDBG(byte registerIndex);

  /**
   * @brief reads the value of the debug registers
   *
   * @return float
   */
  float readDBGf();

  /**
   * @brief reads the firmware version of the board
   *
   */
  void readFirmware(byte*, byte*, byte*);

  /********************************
   * @brief  writing routines      *
   *********************************/

  /**
   * @brief Set the Led ON/OFF
   *
   * @param state
   */
  void setLed(bool state);

  /**
   * @brief sets the speed of the motors
   *
   * @param leftSpeed speed of the left motor
   * @param rightSpeed speed of the right motor
   */
  void move(int leftSpeed, int rightSpeed);

  /**
   * @brief sets the speed of the motors by specifying the pwm values
   *
   * @param leftDutyCycle
   * @param rightDutyCycle
   */
  void moveRAW(int leftDutyCycle, int rightDutyCycle);

  /**
   * @brief sets the speed of a single motor
   *
   * @param motorId motor id (0 for left, 1 for right)
   * @param speed
   */
  void move1m(byte motorId, int speed);

  /**
   * @brief stops the motors
   *
   */
  void stop();

  /**
   * @brief stops the motors by applying the maximum torque
   *
   */
  void brake();

  /**
   * @brief stops the motors by applying the specified torque
   *
   * @param leftTorque
   * @param rightTorque
   */
  void brake(byte leftTorque, byte rightTorque);

  /**
   * @brief stops a single motor by applying the maximum torque
   *
   * @param motorId motor id (0 for left, 1 for right)
   */
  void stop1m(byte motorId);

  /**
   * @brief stops a single motor by applying the specified torque
   *
   * @param motorId
   * @param torque
   */
  void brake1m(byte motorId, byte torque);

  /**
   * @brief resets the left encoder
   *
   */
  void resetLeftEncoder();

  /**
   * @brief resets the right encoder
   *
   */
  void resetEncR();

  // LCD Line 1 write routines
  void lcd1(String string);        //<-- writes a string to the LCD
  void lcd1(byte string[]);        //<-- writes a string to the LCD
  void lcd1(const char string[]);  //<-- writes a string to the LCD
  void lcd1(int number);           //<-- writes a number to the LCD
  void lcd1(unsigned int number);  //<-- writes a number to the LCD
  void lcd1(long int number);      //<-- writes a number to the LCD
  void lcd1(double number);        //<-- writes a number to the LCD
  void lcd1(const char string[],
            int number);  //<-- writes a string and a number to the LCD
  void lcd1(const char string[],
            unsigned int number);  //<-- writes a string and a number to the LCD
  void lcd1(const char string[],
            long int number);  //<-- writes a string and a number to the LCD
  void lcd1(const char string[],
            double number);  //<-- writes a string and a number to the LCD
  void lcd1(unsigned char stringA[8],
            unsigned char stringB[8]);  //<-- writes two strings to the LCD
  void lcd1(int num1, int num2);        //<-- writes two numbers to the LCD
  void lcd1(unsigned int num1,
            unsigned int num2);  //<-- writes two numbers to the LCD
  void lcd1(int num1,
            int num2,
            int num3);  //<-- writes three numbers to the LCD
  void lcd1(int num1,
            int num2,
            int num3,
            int num4);  //<-- writes four numbers to the LCD
  void lcd1(unsigned int num1,
            unsigned int num2,
            unsigned int num3);  //<-- writes three numbers to the LCD
  void lcd1(unsigned int num1,
            unsigned int num2,
            unsigned int num3,
            unsigned int num4);  //<-- writes four numbers to the LCD

  // LCD Line 2 write routines
  void lcd2(String string);        //<-- writes a string to the LCD
  void lcd2(byte string[]);        //<-- writes a string to the LCD
  void lcd2(const char string[]);  //<-- writes a string to the LCD
  void lcd2(int number);           //<-- writes a number to the LCD
  void lcd2(unsigned int number);  //<-- writes a number to the LCD
  void lcd2(long int number);      //<-- writes a number to the LCD
  void lcd2(double number);        //<-- writes a number to the LCD
  void lcd2(const char string[],
            int number);  //<-- writes a string and a number to the LCD
  void lcd2(const char string[],
            unsigned int number);  //<-- writes a string and a number to the LCD
  void lcd2(const char string[],
            long int number);  //<-- writes a string and a number to the LCD
  void lcd2(const char string[],
            double number);  //<-- writes a string and a number to the LCD
  void lcd2(unsigned char stringA[8],
            unsigned char stringB[8]);  //<-- writes two strings to the LCD
  void lcd2(int num1, int num2);        //<-- writes two numbers to the LCD
  void lcd2(unsigned int num1,
            unsigned int num2);  //<-- writes two numbers to the LCD
  void lcd2(int num1,
            int num2,
            int num3);  //<-- writes three numbers to the LCD
  void lcd2(int num1,
            int num2,
            int num3,
            int num4);  //<-- writes four numbers to the LCD
  void lcd2(unsigned int num1,
            unsigned int num2,
            unsigned int num3);  //<-- writes three numbers to the LCD
  void lcd2(unsigned int num1,
            unsigned int num2,
            unsigned int num3,
            unsigned int num4);  //<-- writes four numbers to the LCD

 private:
  byte spiRequestByte(byte command);
  int spiRequestWord(byte command);
  float spiRequestFloat(byte command);
  void spiSendData(byte command, byte buffer[], byte numBytes);
  byte _sspin;
  LineDetector _lineDetector;
};
