#include "LineDetector.h"

namespace
{
    constexpr int _refMax = 1000;
}

LineDetector::LineDetector()
  : _cfgLoaded(false), _previousLineValue(0), _config(Config()),
    _scalingFactor{ 1, 1, 1, 1, 1, 1, 1, 1 }
{}

int LineDetector::ComputeLine(int readings[8])
{
  const int maxRange = 8 * 1000;
  const int midRange = maxRange / 2;
  LoadIfNecessary();
  auto normalised = NormaliseReadings(readings);
  const auto pruned = Prune(normalised);
  auto lineValue = ComputeLineValue(pruned);
  lineValue = FilterLineValue(lineValue, midRange, maxRange);
  lineValue = NormaliseLineValue(lineValue, 8);
  lineValue = CapValue(lineValue, -100, 100);
  return lineValue;
}

void LineDetector::SetConfig(const Config& configIn)
{
  _config = configIn;
  CalculateScalingFactors();
  _cfgLoaded = true;
}

void LineDetector::LoadConfig()
{
  _config.Load();
  CalculateScalingFactors();
  _cfgLoaded = true;
}

int LineDetector::Normalise(const int reading, const int minimum, const float scale) const
{
  return int(float(reading - minimum) * scale);
}

int* LineDetector::NormaliseReadings(const int sensorReading[8]) const
{
  auto sensorMin = _config.GetSensorMin();
  static int sensorNormalised[8];
  for (int i = 0; i < 8; ++i)
  {
    sensorNormalised[i] = Normalise(sensorReading[i],
                                    sensorMin[i],
                                    _scalingFactor[i]);
  }
  return sensorNormalised;
}

float LineDetector::CalculateFactor(const int ref, const int min, const int max) const
{
  return (float(ref) / float(max - min));
}

void LineDetector::CalculateScalingFactors()
{
  static const auto sensorMin = _config.GetSensorMin();
  static const auto sensorMax = _config.GetSensorMax();
  for (int i = 0; i < 8; ++i)
  {
    _scalingFactor[i] = CalculateFactor(_refMax,
                                        sensorMin[i],
                                        sensorMax[i]);
  }
}

void LineDetector::LoadIfNecessary()
{
  if (!_cfgLoaded)
  {
    LoadConfig();
  }
}

int LineDetector::GetMaxFromArray(const int inputArray[8]) const
{
  int maxValue = inputArray[0];
  for (int i = 1; i < 8; ++i)
  {
    if (inputArray[i] > maxValue)
    {
      maxValue = inputArray[i];
    }
  }
  return maxValue;
}

int LineDetector::ComputeLineValue(const int readings[8]) const
{
  int lineValue = -1;
  int maxReading = GetMaxFromArray(readings);
  if (maxReading > _config.GetThreshold())
  {
    lineValue = ComputeMeanGaussian(readings);
  }
  return lineValue;
}

int LineDetector::CapValue(const int value, const int lowerLimit, const int upperLimit) const
{
  if (value < lowerLimit) {
    return lowerLimit;
  } else if (value > upperLimit) {
    return upperLimit;
  } else {
    return value;
  }
}

int LineDetector::ConvertRange(const int xValue,
                               const int xMin, const int xMax,
                               const int yMin, const int yMax) const
{
  const auto xRange = xMax - xMin;
  const auto yRange = yMax - yMin;

  // Avoid division by zero
  if (xRange == 0) {
    return (yMin + (yRange / 2));
  }
  // Calculate the converted value
  const int y = ((float(xValue - xMin) / float(xRange)) * yRange) + yMin;
  return y;
}

int LineDetector::NormaliseLineValue(const int lineValue, const int size) const
{
  const int xMin = 0;
  const int xMax = size * _refMax;  // should be 8000
  const int yMin = -100 - _config.GetCorrectionFactor();
  const int yMax = 100 + _config.GetCorrectionFactor();
  const int convertedLineValue = ConvertRange(lineValue, xMin, xMax, yMin, yMax);
  const int cappedLineValue = CapValue(convertedLineValue, -100, 100);
  return cappedLineValue;
}

int LineDetector::FilterLineValue(const int lineValue_in, const int refValue, const int maxValue)
{
  int lineValue = lineValue_in;
  // out of the line -> all white
  if (lineValue == -1) {
    if (_previousLineValue > refValue) {
      lineValue = maxValue;
    } else {
      lineValue = 0;
    }
  }
  // possible reading errors
  else if (lineValue < -1 || lineValue > maxValue) {
    lineValue = _previousLineValue;
  }
  // if normal values
  else {
    _previousLineValue = lineValue;
  }
  return lineValue;
}

long int LineDetector::Sum(const long int reading[8]) const
{
  long int sum = 0;
  for (int i = 0; i < 8; ++i)
  {
    sum += reading[i];
  }
  return sum;
}

long* LineDetector::CastToLongInt(const int intArray[8]) const
{
  static long longIntArray[8];
  for(size_t i = 0; i < 8; ++i)
  {
    longIntArray[i] = static_cast<long>(intArray[i]);
  }
  return longIntArray;
}

int LineDetector::ComputeMeanGaussian(const int reading[8]) const
{
  int value[8];
  int newValue = 500;
  for (int i = 0; i < 8; ++i)
  {
    value[i] = newValue;
    newValue += 1000;
  }
  static long int product[8];
  for (int i = 0; i < 8; ++i)
  {
    product[i] = (static_cast<long int>(value[i]) * static_cast<long int>(reading[i]));
  }

  const auto sumProduct = Sum(product);
  const auto sumProbability = Sum(CastToLongInt(reading));

  float mean = 0;
  if (sumProbability != 0)
  {
    mean = sumProduct / float(sumProbability);
  }
  return int(mean);
}

void LineDetector::GetMaxValueAndIndex(const int array[8], int& maxValue, int& maxValueIndex) const
{
  maxValueIndex = 0;
  maxValue = array[0];

  for (int i = 1; i < 8; ++i) {
    if (array[i] > maxValue) {
      maxValue = array[i];
      maxValueIndex = i;
    }
  }
}

int* LineDetector::Prune(int readings[8]) const
{
  int maxValue, maxIndex;
  GetMaxValueAndIndex(readings, maxValue, maxIndex);
  if (maxValue < _config.GetThreshold()) {
    if ((maxIndex == 0) || (maxIndex == 7)) {
      readings[maxIndex] = _config.GetThreshold();
    }
  }
  return readings;
}

Config LineDetector::GetConfig() const
{
  return _config;
}