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
  _initMemoryAddress = eeprom_address;
  _sensorMaxMemAdd = _initMemoryAddress;
  _sensorMinMemAdd = _initMemoryAddress + 16;
  _thresholdMemAdd = _initMemoryAddress + 32;
  _correctionFactorMemAdd = _initMemoryAddress + 34;
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
  LoadArrayValues(_sensorMaxMemAdd, _sensorMax);
  VerifyAndCorrectArray(_sensorMax, 200, 1000, 800);
  LoadArrayValues(_sensorMinMemAdd, _sensorMin);
  VerifyAndCorrectArray(_sensorMin, 0, 200, 20);
  LoadWord(_thresholdMemAdd, _threshold);
  VeriyAndCorrectValue(_threshold, 0, 500, 50);
  LoadByte(_correctionFactorMemAdd, _correctionFactor);
  VeriyAndCorrectValue(_correctionFactor, 0, 50, 6);
}

void Config::Print() const {
  PrintArray("Sensor Max:", _sensorMax);
  PrintArray("Sensor Min:", _sensorMin);
  PrintValue("Threshold:", _threshold);
  PrintValue("Correction Factor:", _correctionFactor);
}

void Config::SaveSensorMax() const {
  SaveArrayValues(_sensorMaxMemAdd, _sensorMax);
}

void Config::SaveSensorMin() const {
  SaveArrayValues(_sensorMinMemAdd, _sensorMin);
}

void Config::SaveThreshold() const { SaveWord(_thresholdMemAdd, _threshold); }

void Config::SaveCorrectionFactor() const {
  SaveByte(_correctionFactorMemAdd, _correctionFactor);
}

void Config::Save() const {
  SaveArrayValues(_sensorMaxMemAdd, _sensorMax);
  SaveArrayValues(_sensorMinMemAdd, _sensorMin);
  SaveWord(_thresholdMemAdd, _threshold);
  SaveByte(_correctionFactorMemAdd, _correctionFactor);
}

void Config::SetSensorMax(int maxValues[8]) {
  for (int i = 0; i < 8; ++i) {
    _sensorMax[i] = maxValues[i];
  }
}

void Config::SetSensorMin(int minValues[8]) {
  for (int i = 0; i < 8; ++i) {
    _sensorMin[i] = minValues[i];
  }
}

void Config::SetThreshold(int value) { _threshold = value; }

void Config::SetCorrectionFactor(int value) { _correctionFactor = value; }

byte Config::LoadArrayValues(byte eeprom_address, int array[8]) const {
  for (int i = 0; i < 8; ++i) {
    array[i] = (int)EEPROM.read(eeprom_address);
    array[i] = (array[i] << 8);
    eeprom_address += 1;
    array[i] += (int)EEPROM.read(eeprom_address);
    eeprom_address += 1;
  }
  return eeprom_address;
}

byte Config::LoadWord(byte eeprom_address, int& outValue) const {
  outValue = (int)EEPROM.read(eeprom_address);
  outValue = (outValue << 8);
  eeprom_address += 1;
  outValue += (int)EEPROM.read(eeprom_address);
  eeprom_address += 1;
  return eeprom_address;
}

byte Config::LoadByte(byte eeprom_address, int& outValue) const {
  outValue = (int)EEPROM.read(eeprom_address);
  eeprom_address += 1;
  return eeprom_address;
}

void Config::PrintArray(String text, int array[8]) const {
  Serial.begin(57600);
  Serial.println(text);
  for (int i = 0; i < 8; ++i) {
    Serial.print(array[i]);
    Serial.print("    ");
  }
  Serial.println();
  Serial.end();
}

void Config::PrintValue(String text, int value) const {
  Serial.begin(57600);
  Serial.println(text);
  Serial.println(value);
  Serial.end();
}

byte Config::SaveArrayValues(byte eeprom_address, int array[8]) const {
  for (int i = 0; i < 8; ++i) {
    EEPROM.write(eeprom_address, highByte(array[i]));
    eeprom_address += 1;
    EEPROM.write(eeprom_address, lowByte(array[i]));
    eeprom_address += 1;
  }
  return eeprom_address;
}

byte Config::SaveWord(byte eeprom_address, int value) const {
  EEPROM.write(eeprom_address, highByte(value));
  eeprom_address += 1;
  EEPROM.write(eeprom_address, lowByte(value));
  eeprom_address += 1;
  return eeprom_address;
}

byte Config::SaveByte(byte eeprom_address, int value) const {
  EEPROM.write(eeprom_address, lowByte(value));
  eeprom_address += 1;
  return eeprom_address;
}