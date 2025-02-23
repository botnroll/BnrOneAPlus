/**
 * BnrOneAPlus.h - Library for interfacing with Bot'n Roll ONE A+
 * Arduino Compatible
 * Released into public domain
 * www.botnroll.com
 */

#pragma once

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
  void spiConnect(const byte sspin);

  /**
   * @brief Set the minimum battery voltage for battery protection
   *
   * @param min_battery_V
   */
  void setMinBatteryV(const float min_battery_V) const;

  /**
   * @brief Set the PID parameters for PID control
   *
   * @param kp proportional gain
   * @param ki integral gain
   * @param kd differential gain
   */
  void setPid(const int kp, const int ki, const int kd) const;

  /**
   * @brief Set the Motors configuration params:
   *    - moving power
   *    - ks gain
   *    - number of pulses per iteration at max speed
   *
   * @param moving_power
   * @param ks
   * @param max_speed_pulses
   */
  void setMotors(const int moving_power,
                 const int ks,
                 const int max_speed_pulses) const;

  /**
   * @brief Turn on/off the obstacle sensors IR emitters
   *
   * @param state
   */
  void obstacleSensorsEmitters(const bool state) const;

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
  byte readObstacleSensors() const;

  /**
   * @brief reads the value of the left range sensor
   *
   * @return byte
   */
  byte readLeftRangeSensor() const;

  /**
   * @brief reads the value of the right range sensor
   *
   * @return byte
   */
  byte readRightRangeSensor() const;

  /**
   * @brief reads the value of the specified ADC channel
   *
   * @param adc_channel id of the Adc channel to read
   * @return int
   */
  int readAdc(const byte adc_channel) const;

  /**
   * @brief reads the value of the button pressed
   *
   * @return byte
   *  0 if no button is pressed
   *  1 if button 1 is pressed
   *  2 if button 2 is pressed
   *  3 if button 3 is pressed
   */
  byte readButton() const;

  /**
   * @brief reads the battery voltage
   *
   * @return float
   */
  float readBattery() const;

  /**
   * @brief computes a line value in the range [-100, 100]
   */
  int readLine();

  /**
   * @brief reads the line sensor and outputs a vector of 8 integers
   */
  int* readLineSensor() const;

  /**
   * @brief reads the value of the left encoder and resets its value
   *
   * @return int
   */
  int readAndResetLeftEncoder() const;

  /**
   * @brief reads the value of the right encoder and resets its value
   *
   * @return int
   */
  int readAndResetRightEncoder() const;

  /**
   * @brief reads the value of the left encoder and keep it's current value
   * so that it increments over time
   *
   * @return int
   */
  int readAndIncrementLeftEncoder() const;

  /**
   * @brief reads the value of the right encoder and keep it's current value
   * so that it increments over time
   *
   * @return int
   */
  int readAndIncrementRightEncoder() const;

  /**
   * @brief reads the value of the debug registers
   *
   * @return int
   */
  int readDBG(const byte register_index) const;

  /**
   * @brief reads the value of the debug registers
   *
   * @return float
   */
  float readDBGf() const;

  /**
   * @brief reads the firmware version of the board
   *
   */
  void readFirmware(byte*, byte*, byte*) const;

  /********************************
   * @brief  writing routines      *
   *********************************/

  /**
   * @brief Set the Led ON/OFF
   *
   * @param state
   */
  void setLed(const bool state) const;

  /**
   * @brief sets the speed of the motors
   *
   * @param left_speed speed of the left motor
   * @param right_speed speed of the right motor
   */
  void move(const int left_speed, const int right_speed) const;

  /**
   * @brief Helper method to send the right move_rpm command together with left and right rpm values
   *
   * @param command one of the available move rpm commands
   * @param left_rpm
   * @param right_rpm
   */
  void sendMoveRpm(const byte command, const int left_rpm, const int right_rpm) const;

  /**
   * @brief sets the speed of the motors by specifying the rpm values
   *
   * @param left_rpm
   * @param right_rpm
   */
  void moveRpm(const int left_rpm, const int right_rpm) const;

  /**
   * @brief sets the speed of the motors by specifying the rpm values and waits for the encoders readings
   *
   * @param left_rpm speed of the left motor
   * @param right_rpm speed of the right motor
   * @param out_left_encoder variable to store the left encoder reading
   * @param out_ight_encoder variable to store the right encoder reading
   */
  void moveRpmGetEncodersGetEncoders(const int left_rpm, const int right_rpm, int& out_left_encoder, int& out_right_encoder) const;

  /**
   * @brief sets the speed of the motors by specifying the pwm values
   *
   * @param left_duty_cycle
   * @param right_duty_cycle
   */
  void moveRAW(const int left_duty_cycle, const int right_duty_cycle) const;

  /**
   * @brief sets the speed of a single motor
   *
   * @param motor_id motor id (0 for left, 1 for right)
   * @param speed
   */
  void move1m(const byte motor_id, const int speed) const;

  /**
   * @brief stops the motors
   *
   */
  void stop() const;

  /**
   * @brief stops the motors by applying the maximum torque
   *
   */
  void brake() const;

  /**
   * @brief stops the motors by applying the specified torque
   *
   * @param left_torque
   * @param right_torque
   */
  void brake(const byte left_torque, const byte right_torque) const;

  /**
   * @brief stops a single motor by applying the maximum torque
   *
   * @param motor_id motor id (0 for left, 1 for right)
   */
  void stop1m(const byte motor_id) const;

  /**
   * @brief stops a single motor by applying the specified torque
   *
   * @param motor_id
   * @param torque
   */
  void brake1m(const byte motor_id, const byte torque) const;

  /**
   * @brief resets the left encoder
   *
   */
  void resetLeftEncoder() const;

  /**
   * @brief resets the right encoder
   *
   */
  void resetRightEncoder() const;

  // LCD Line 1 write routines
  void lcd1(const String& string) const;        //<-- writes a string to the LCD
  void lcd1(const byte string[]) const;        //<-- writes a string to the LCD
  void lcd1(const char string[]) const;        //<-- writes a string to the LCD
  void lcd1(const int number) const;           //<-- writes a number to the LCD
  void lcd1(const unsigned int number) const;  //<-- writes a number to the LCD
  void lcd1(const long int number) const;      //<-- writes a number to the LCD
  void lcd1(const double number) const;        //<-- writes a number to the LCD
  void lcd1(
      const char string[],
      const int number) const;  //<-- writes a string and a number to the LCD
  void lcd1(const char string[],
            const unsigned int number)
      const;  //<-- writes a string and a number to the LCD
  void lcd1(const char string[],
            const long int number)
      const;  //<-- writes a string and a number to the LCD
  void lcd1(
      const char string[],
      const double number) const;  //<-- writes a string and a number to the LCD
  void lcd1(const unsigned char string_a[8],
            const unsigned char string_b[8])
      const;  //<-- writes two strings to the LCD
  void lcd1(const int num1,
            const int num2) const;  //<-- writes two numbers to the LCD
  void lcd1(const unsigned int num1,
            const unsigned int num2) const;  //<-- writes two numbers to the LCD
  void lcd1(const int num1,
            const int num2,
            const int num3) const;  //<-- writes three numbers to the LCD
  void lcd1(const int num1,
            const int num2,
            const int num3,
            const int num4) const;  //<-- writes four numbers to the LCD
  void lcd1(
      const unsigned int num1,
      const unsigned int num2,
      const unsigned int num3) const;  //<-- writes three numbers to the LCD
  void lcd1(
      const unsigned int num1,
      const unsigned int num2,
      const unsigned int num3,
      const unsigned int num4) const;  //<-- writes four numbers to the LCD

  // LCD Line 2 write routines
  void lcd2(const String& string) const;       //<-- writes a string to the LCD
  void lcd2(const byte string[]) const;        //<-- writes a string to the LCD
  void lcd2(const char string[]) const;        //<-- writes a string to the LCD
  void lcd2(const int number) const;           //<-- writes a number to the LCD
  void lcd2(const unsigned int number) const;  //<-- writes a number to the LCD
  void lcd2(const long int number) const;      //<-- writes a number to the LCD
  void lcd2(const double number) const;        //<-- writes a number to the LCD
  void lcd2(const char string[],
            int number) const;  //<-- writes a string and a number to the LCD
  void lcd2(
      const char string[],
      unsigned int number) const;  //<-- writes a string and a number to the LCD
  void lcd2(
      const char string[],
      long int number) const;  //<-- writes a string and a number to the LCD
  void lcd2(const char string[],
            double number) const;  //<-- writes a string and a number to the LCD
  void lcd2(const unsigned char string_a[8],
            const unsigned char string_b[8])
      const;  //<-- writes two strings to the LCD
  void lcd2(const int num1,
            const int num2) const;  //<-- writes two numbers to the LCD
  void lcd2(const unsigned int num1,
            const unsigned int num2) const;  //<-- writes two numbers to the LCD
  void lcd2(const int num1,
            const int num2,
            const int num3) const;  //<-- writes three numbers to the LCD
  void lcd2(const int num1,
            const int num2,
            const int num3,
            const int num4) const;  //<-- writes four numbers to the LCD
  void lcd2(
      const unsigned int num1,
      const unsigned int num2,
      const unsigned int num3) const;  //<-- writes three numbers to the LCD
  void lcd2(
      const unsigned int num1,
      const unsigned int num2,
      const unsigned int num3,
      const unsigned int num4) const;  //<-- writes four numbers to the LCD

 private:
  byte spiRequestByte(const byte command) const;
  int spiRequestWord(const byte command) const;
  float spiRequestFloat(const byte command) const;
  void spiSendDataOnly(const byte command,
                   const byte buffer[],
                   const byte num_bytes) const;
  void spiSendData(const byte command,
                    const byte buffer[],
                    const byte num_bytes) const;
  byte sspin_;
  LineDetector line_detector_;
};
