/**
 * @brief Config class to store variables for the operation of BotnRoll One A.
 * Some variables are read from config.
 * Other variables are computed from values read from config.
 * You can read, modify them in the code and then Save the relevant ones for
 * next time.
 */

#pragma once

#include <Arduino.h>

using byte = uint8_t;

class Config {
 public:
  Config(byte eeprom_address = 100);

  /**
   * @brief Read EEPROM values <> Ler valores da EEPROM
   * Do not change this order <> Nao alterar a ordem
   */
  void Load();

  /**
   * @brief Prints the config values in the serial terminal
   * Note: You need to open a terminal serial connection to the Bot'n Roll One A
   * with baudrate 57600
   */
  void Print() const;

  void SaveSensorMin() const;

  void SaveSensorMax() const;

  void SaveThreshold() const;

  void SaveCorrectionFactor() const;

  /**
   * @brief saves the config values into EEPROM
   */
  void Save() const;

  /**
   * @brief Set the Max Values
   *
   * @param max_values
   */
  void SetSensorMax(const int max_values[8]);

  /**
   * @brief Set the Min Values
   *
   * @param min_values
   */
  void SetSensorMin(const int min_values[8]);

  /**
   * @brief Set the Threshold
   *
   * @param threshold
   */
  void SetThreshold(const int threshold);

  /**
   * @brief Set the Correction Factor
   *
   * @param correction_factor
   */
  void SetCorrectionFactor(const int correction_factor);

  inline const int* GetSensorMin() const { return sensor_min_; };

  inline const int* GetSensorMax() const { return sensor_max_; };

  inline int GetThreshold() const { return threshold_; };

  inline int GetCorrectionFactor() const { return correction_factor_; };

 private:
  void VerifyAndCorrectArray(int* array,
                             const int min,
                             const int max,
                             const int defaultValue);

  template <class T>
  void VeriyAndCorrectValue(T& value,
                            const T min,
                            const T max,
                            const T defaultValue);

  byte LoadArrayValues(byte eeprom_address, int out_array[8]) const;

  byte LoadWord(byte eeprom_address, int& out_value) const;

  byte LoadByte(byte eeprom_address, int& out_value) const;

  void PrintArray(const String& text, const int array[8]) const;

  void PrintValue(const String& text, const int value) const;

  byte SaveArrayValues(byte eeprom_address, const int array[8]) const;

  byte SaveWord(byte eeprom_address, const int value) const;

  byte SaveByte(byte eeprom_address, const int value) const;

  int sensor_max_[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
  int sensor_min_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int threshold_ = 50;  // Line follower limit between white and black
  int correction_factor_ = 6;
  byte init_memory_address_ = 100;
  byte sensor_max_mem_add_ = init_memory_address_;
  byte sensor_min_mem_add_ = init_memory_address_ + 16;
  byte threshold_mem_add_ = init_memory_address_ + 32;
  byte correction_factor_mem_add_ = init_memory_address_ + 34;
};
