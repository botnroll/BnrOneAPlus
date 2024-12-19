/* 
 This example was created by José Cruz (www.botnroll.com)
 on the 18th January 2024
 
 This code example is in the public domain. 
 http://www.botnroll.com

Description:
Infra-Red obstacle detection LEDs (IRE1 and IRE2) are switched ON and OFF every second in this example code. 
For Race of Champinons start, IR LED´s must be OFF.
Messages are printed on the LCD and Serial Monitor accordingly.
WARNING!!!
Infra-Red light can damage your eyes if you look to emitting IR LED's.
You will not be able to see the LED's emitting light because human eyes cannot see infra-red light.
You can see the IR LED's light using a camera (from you cell phone or smart-phone, for example).
Placing an obstacle 2cm in front of the LEDS should switch IRS1 and IRS2 ON.
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
}

void loop()
{
    one.obstacleEmitters(ON);          // activate IR emitter LEDs
    Serial.println("IR Emitters ON "); // print data on serial monitor.
    one.lcd2(" IR Emitters ON ");      // print text on LCD line 2
    delay(1000);                       // wait 1 second
    one.obstacleEmitters(OFF);         // deactivate IR emitter LEDs
    Serial.println("IR Emitters OFF"); // print data on serial monitor.
    one.lcd2(" IR Emitters OFF ");     // print text on LCD line 2
    delay(1000);                       // wait 1 second
}