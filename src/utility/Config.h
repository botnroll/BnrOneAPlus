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
   * @param maxValues
   */
  void SetSensorMax(int maxValues[8]);

  /**
   * @brief Set the Min Values
   *
   * @param minValues
   */
  void SetSensorMin(int minValues[8]);

  /**
   * @brief Set the Threshold
   *
   * @param threshold
   */
  void SetThreshold(int threshold);

  /**
   * @brief Set the Correction Factor
   *
   * @param correctionFactor
   */
  void SetCorrectionFactor(int correctionFactor);

  inline int* GetSensorMin() const { return _sensorMin; };

  inline int* GetSensorMax() const { return _sensorMax; };

  inline int GetThreshold() const { return _threshold; };

  inline int GetCorrectionFactor() const { return _correctionFactor; };

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

  byte LoadArrayValues(byte eeprom_address, int array[8]) const;

  byte LoadWord(byte eeprom_address, int& outValue) const;

  byte LoadByte(byte eeprom_address, int& outValue) const;

  void PrintArray(String text, int array[8]) const;

  void PrintValue(String text, int value) const;

  byte SaveArrayValues(byte eeprom_address, int array[8]) const;

  byte SaveWord(byte eeprom_address, int value) const;

  byte SaveByte(byte eeprom_address, int value) const;

  int _sensorMax[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
  int _sensorMin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int _threshold = 50;  // Line follower limit between white and black
  int _correctionFactor = 6;
  byte _initMemoryAddress = 100;
  byte _sensorMaxMemAdd = _initMemoryAddress;
  byte _sensorMinMemAdd = _initMemoryAddress + 16;
  byte _thresholdMemAdd = _initMemoryAddress + 32;
  byte _correctionFactorMemAdd = _initMemoryAddress + 34;
};
