/**
 * @brief Config class to store variables for the operation of BotnRoll One A.
 * Some variables are read from config.
 * Other variables are computed from values read from config.
 * You can read, modify them in the code and then Save the relevant ones for
 * next time.
 */

#include "Config.h"

#include <EEPROM.h>  // EEPROM reading and writing

Config::Config(byte eeprom_address) {
  init_memory_address_ = eeprom_address;
  sensor_max_mem_add_ = init_memory_address_;
  sensor_min_mem_add_ = init_memory_address_ + 16;
  threshold_mem_add_ = init_memory_address_ + 32;
  correction_factor_mem_add_ = init_memory_address_ + 34;
}

void Config::VerifyAndCorrectArray(int* array,
                                   const int min,
                                   const int max,
                                   const int defaultValue) {
  bool setDefaultValue = false;
  for (int i = 0; i < 8; ++i) {
    if (array[i] < min || array[i] > max) {
      setDefaultValue = true;
      break;
    }
  }

  if (setDefaultValue) {
    for (int i = 0; i < 8; ++i) {
      array[i] = defaultValue;
    }
  }
}

template <class T>
void Config::VeriyAndCorrectValue(T& value,
                                  const T min,
                                  const T max,
                                  const T defaultValue) {
  if (value < min || value > max) {
    value = defaultValue;
  }
}

void Config::Load() {
  LoadArrayValues(sensor_max_mem_add_, sensor_max_);
  VerifyAndCorrectArray(sensor_max_, 200, 1000, 800);
  LoadArrayValues(sensor_min_mem_add_, sensor_min_);
  VerifyAndCorrectArray(sensor_min_, 0, 200, 20);
  LoadWord(threshold_mem_add_, threshold_);
  VeriyAndCorrectValue(threshold_, 0, 500, 50);
  LoadByte(correction_factor_mem_add_, correction_factor_);
  VeriyAndCorrectValue(correction_factor_, 0, 50, 6);
}

void Config::Print() const {
  PrintArray("Sensor Max:", sensor_max_);
  PrintArray("Sensor Min:", sensor_min_);
  PrintValue("Threshold:", threshold_);
  PrintValue("Correction Factor:", correction_factor_);
}

void Config::SaveSensorMax() const {
  SaveArrayValues(sensor_max_mem_add_, sensor_max_);
}

void Config::SaveSensorMin() const {
  SaveArrayValues(sensor_min_mem_add_, sensor_min_);
}

void Config::SaveThreshold() const { SaveWord(threshold_mem_add_, threshold_); }

void Config::SaveCorrectionFactor() const {
  SaveByte(correction_factor_mem_add_, correction_factor_);
}

void Config::Save() const {
  SaveArrayValues(sensor_max_mem_add_, sensor_max_);
  SaveArrayValues(sensor_min_mem_add_, sensor_min_);
  SaveWord(threshold_mem_add_, threshold_);
  SaveByte(correction_factor_mem_add_, correction_factor_);
}

void Config::SetSensorMax(const int maxValues[8]) {
  for (int i = 0; i < 8; ++i) {
    sensor_max_[i] = maxValues[i];
  }
}

void Config::SetSensorMin(const int minValues[8]) {
  for (int i = 0; i < 8; ++i) {
    sensor_min_[i] = minValues[i];
  }
}

void Config::SetThreshold(const int value) { threshold_ = value; }

void Config::SetCorrectionFactor(const int value) {
  correction_factor_ = value;
}

byte Config::LoadArrayValues(byte eeprom_address, int out_array[8]) const {
  for (int i = 0; i < 8; ++i) {
    out_array[i] = (int)EEPROM.read(eeprom_address);
    out_array[i] = (out_array[i] << 8);
    eeprom_address += 1;
    out_array[i] += (int)EEPROM.read(eeprom_address);
    eeprom_address += 1;
  }

  return eeprom_address;
}

byte Config::LoadWord(byte eeprom_address, int& out_value) const {
  out_value = (int)EEPROM.read(eeprom_address);
  out_value = (out_value << 8);
  eeprom_address += 1;
  out_value += (int)EEPROM.read(eeprom_address);
  eeprom_address += 1;
  return eeprom_address;
}

byte Config::LoadByte(byte eeprom_address, int& out_value) const {
  out_value = (int)EEPROM.read(eeprom_address);
  eeprom_address += 1;
  return eeprom_address;
}

void Config::PrintArray(const String& text, const int array[8]) const {
  Serial.begin(115200);
  Serial.println(text);
  for (int i = 0; i < 8; ++i) {
    Serial.print(array[i]);
    Serial.print("    ");
  }
  Serial.println();
  Serial.end();
}

void Config::PrintValue(const String& text, const int value) const {
  Serial.begin(115200);
  Serial.println(text);
  Serial.println(value);
  Serial.end();
}

byte Config::SaveArrayValues(byte eeprom_address, const int array[8]) const {
  for (int i = 0; i < 8; ++i) {
    EEPROM.write(eeprom_address, high_byte(array[i]));
    eeprom_address += 1;
    EEPROM.write(eeprom_address, low_byte(array[i]));
    eeprom_address += 1;
  }
  return eeprom_address;
}

byte Config::SaveWord(byte eeprom_address, const int value) const {
  EEPROM.write(eeprom_address, high_byte(value));
  eeprom_address += 1;
  EEPROM.write(eeprom_address, low_byte(value));
  eeprom_address += 1;
  return eeprom_address;
}

byte Config::SaveByte(byte eeprom_address, const int value) const {
  EEPROM.write(eeprom_address, low_byte(value));
  eeprom_address += 1;
  return eeprom_address;
}