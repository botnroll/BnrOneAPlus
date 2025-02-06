/**
 *  Line detector class
 *
 *  It assumes that the array of sensors of a sensor peripheral are arranged in a straight line and equally spaced.
 *  Converts line sensor readings into a relative line position on the sensor.
 *  It provides methods to compute the location of the line relative to the device.
 *  It outputs values in a range [-100, 100] where 0 (zero) corresponds to the centre of the sensor peripheral.
*/

#pragma once

#include "Config.h"

class LineDetector
{
public:
    LineDetector();

    /**
     * @brief Given an input as a set of sensors readings it computes a relative location of a
     * line along the length of the sensor and expresses the output in a range [-100, 100]
     * where 0 (zero) corresponds to the line being at the centre of the sensor.
     *
     * @param readings
     * @return int
     */
    int ComputeLine(int readings[8]);

    /**
     * @brief Loads values from config and calculates the corresponding scaling factors
     */
    void LoadConfig();

    /**
     * @brief Set the config by providing it as input
     *
     * @param configIn
     */
    void SetConfig(const Config& configIn);

    /**
     * @brief Normalize values for each sensor reading
     *
     * @param sensor_reading array containing sensor readings
     * @return array
     */
    int* NormaliseReadings(const int sensorReading[8]) const;

private:
    /**
     * @brief normalise a reading taking the minimum value of the range and a scale factor
     * @param reading a sensor readings
     * @param minimum the minimum value of sensor reading
     * @param scale scaling factor
     * @return normalised value
     */
    int Normalise(const int reading, const int minimum, const float scale) const;

    /**
     * @brief Calculates the scaling factor given a reference value
     *  and a range defined by min and max values
     *  This scaling factor is useful in normalising values
     *
     * @return float scaling factor
     */
    float CalculateFactor(const int ref, const int min, const int max) const;

    /**
     * @brief Calculates and stores the scaling factors (in _scalingFactors)
     */
    void CalculateScalingFactors();

    /**
     * @brief Loads values from config if NOT loaded before
     */
    void LoadIfNecessary();

    /**
     * @brief Get the Max From Array
     *
     * @param inputArray
     * @return int
     */
    int GetMaxFromArray(const int inputArray[8]) const;

    /**
     * @brief Computes a line value in the range [0, ref_max]
     *
     * @param readings
     * @return int
     */
    int ComputeLineValue(const int readings[8]) const;

    /**
     * @brief Caps the value to lower and upper limits
     *
     * @return capped value
     */
    int CapValue(const int value, const int lowerLimit, const int upperLimit) const;

    /**
     * @brief Converts a value x given in the range [xMin : xMax]
     *  to a new value in the range [yMin : yMax]
     *
     * @return value converted to the new range
     */
    int ConvertRange(const int xValue, const int xMin, const int xMax,
                       const int yMin, const int yMax) const;

    /**
     * @brief Converts a line value in the range of [0, 8000] to a new range e.g. [-106, 106]
     *  This method involves two steps:
     *  1) Converting the value in the range of [0, 8000] to a new range using a correction factor
     *      e.g. for a correction factor of 6% the new range is [-106, 106]
     *      The new range extends beyond 100 by a little margin specified by the correction
     *      factor in the config.
     *      The purpose of this is to enable adjusting the sensitivity of the readings near the
     *      extremities. For a sensor that is typically higher (away from the floor) this
     *      correction factor should be higher to prevent large jumps in the readings close to
     *      the extremities. By increasing the correction factor we are narrowing the sensitivity
     *      of the sensors and by reducing it we are extending the sensitivity out.
     *      There is a balance to achieve by tuning this value correctly in order to get the
     *      best of both worlds:
     *      - Smooth transitions in sensor value near the extremeties
     *      - Maximum sensitivity region of the sensor
     *  2) Capping the values to the desired range [-100, 100]
     *
     * @param lineValue input line value
     * @param size size of sensor readings
     * @return int normalised line value
     */
    int NormaliseLineValue(const int lineValue, const int size) const;

    /**
     * @brief Filters the line value to handle edge cases
     *        such as no line detected or reading errors
     * @param lineValue the computed line value
     * @param refValue the reference value to help determining which side
     * is the line in case it's no longer detected (e.g. middle of the range)
     * @param maxValue the maximum value of the range
     * @return int
     */
    int FilterLineValue(const int lineValue_in, const int refValue, const int maxValue);

    /**
     * @brief Computes the sum of all the values in the input array
     *
     * @param reading
     * @return long int
     */
    long int Sum(const long int reading[8]) const;

    /**
     * @brief Casts an array of integers to an array of long integers
     *
     * @param intArray
     * @return long*
     */
    long* CastToLongInt(const int intArray[8]) const;

    /**
     * @brief Lets assume the line detected gives us a discrete gaussian
     *         where the probabilities are given by each sensor reading and
     *         the values are pre-determined based on each sensor location:
     *          |sensor id | value  | probability |
     *          |----------|--------|-------------|
     *          |    0     |   1    |  reading[0] |
     *          |    1     |   2    |  reading[1] |
     *          |  (...)   | (...)  |    (...)    |
     *          |    7     |   8    |  reading[7] |
     *         We can compute the mean of the gaussian (location of line) by:
     *          1) computing the product for each sensor: product = value * probability
     *          2) computing sumProducts = product[0] + product[1] + ... + product[7]
     *          3) computing sum_probabilities = reading[0] + reading[1] + ... + reading[7]
     *          4) computing mean = sumProducts / sum_probabilities
     *
     * @param readings
     * @return int
     */
    int ComputeMeanGaussian(const int reading[8]) const;

    /**
     * @brief Returns the max value and its corresponding index from a list
     *
     * @return def
     */
    void GetMaxValueAndIndex(const int array[8], int& maxValue, int& maxValueIndex) const;

    /**
     * @brief Deals with edge cases such when the max readings is on one of the sensors at the
     *  extremety. In such cases it bumps up the value of the sensor to be at the threshold level
     *
     * @param readings
     */
    int* Prune(int readings[8]) const;

    /**
     * @brief Get the Config object
     */
    Config GetConfig() const;

    // flag to signal whether or not the config values have been read already
    bool _cfgLoaded = false;
    int _previousLineValue = 0;
    Config _config = Config();
    // array of 8 elements with correction factor for each line sensor
    float _scalingFactor[8];
};