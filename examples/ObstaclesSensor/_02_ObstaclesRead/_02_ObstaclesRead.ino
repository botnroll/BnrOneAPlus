/* 
 This example was created by José Cruz (www.botnroll.com)
 on the 18th December 2024
 
 This code example is in the public domain. 
 http://www.botnroll.com

Description:
Read obstacle sensors range distance for left and right IR sensors.
Range varies from 0 to 25:
 -> 0 means no obstacle is detected
 -> 25 means obstacle is very close to the sensor
The robot has new readings every 25ms (40 readings per second)
Note: Valid for TSSP4056 IR sensors shipped with robots from 2023.
*/

#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;           // declaration of object variable to control the Bot'n Roll ONE A+

//constants definition
#define SSPIN  2       // Slave Select (SS) pin for SPI communication

void setup() 
{
    Serial.begin(57600);     // set baud rate to 57600bps for printing values at serial monitor.
    one.spiConnect(SSPIN);   // start SPI communication module
    one.stop();              // stop motors
    one.obstacleEmitters(ON);// activate IR emitters
}

void loop()
{
    byte rangeL=one.readRangeL(); // read left obstacle sensor range 
    byte rangeR=one.readRangeR(); // read right obstacle sensor range
    one.lcd1("Range Left : ",rangeL);
    one.lcd2("Range Right: ",rangeR);
    Serial.print("L: ");Serial.print(rangeL);Serial.print("   R: ");Serial.println(rangeR);
    delay(25); //The robot has new readings every 25ms (40 readings per second)
}