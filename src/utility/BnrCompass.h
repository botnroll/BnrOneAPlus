/*
  BnrColorSensor.h - Library for interfacing with Bot'n Roll Color Sensor for Bot'n Roll ONE A Plus (www.botnroll.com)
  Created by José  Cruz, November 18, 2024.
  v_1.0.0
  Released into the public domain.
*/

#pragma once

#include "Arduino.h"


class BnrCompass
{
    public:
        /********************************
         * @brief  setup routines        *
         *********************************/
        
        /**
         * @brief Initializes the I2C communication given an Address
         * 
         * @param sensorAddress 
         * @return void
         */
        void i2cConnect(byte sensorAddress);    //Enable I2C communication
        
        /**
         * @brief Reads the current bearing of the Compass
         * 
         * @return float
         */
        float read_bearing();
        
        /**
         * @brief Reads the current roll value of the Compass
         * 
         * @return char
         */
        char read_roll();
        
        /**
         * @brief Reads the current pitch value of the Compass
         * 
         * @return char
         */
        char read_pitch();

    private:
        byte _sensorAddress;
};

