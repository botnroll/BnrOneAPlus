/* 
 This example was created by José Cruz (www.botnroll.com)
 on the 18th December 2024
 
 This code example is in the public domain. 
 http://www.botnroll.com

Description:
An external IR 40khz source can be detected by the IR sensors. Another Bot´n Roll ONE A+ emitting IR will be detected.
This function can be used to remote control the robot with IR.
This function is used in the race challenge to detect the start of the race!

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
    one.spiConnect(SSPIN);   // start the communication module
    one.stop();              // stop motors
    one.obstacleEmitters(ON);// activate IR emitter LEDs
}

void loop()
{
    byte ir_sensors=0;

    ir_sensors=one.readIRSensors();      // read actual IR sensors state
    Serial.print("IR Sensors ");         // print data on serial monitor.
    Serial.println(ir_sensors);          // print data on serial monitor.
    one.lcd2("IR Sensors:", ir_sensors); // print text on LCD line 2
}

