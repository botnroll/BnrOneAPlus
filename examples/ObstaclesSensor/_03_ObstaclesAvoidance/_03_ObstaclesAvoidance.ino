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
Note: Valid for TSSP4056 sensors shipped with robots from 2023.
*/

#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;           // declaration of object variable to control the Bot'n Roll ONE A+

//constants definition
#define SSPIN  2       // Slave Select (SS) pin for SPI communication
int motL=0, motR=0;
double kL=2.70;  //Linear gain <> Ganho linear
int vel=60;

void setup() 
{
    one.spiConnect(SSPIN);   // start SPI communication module
    one.stop();              // stop motors
    one.obstacleEmitters(ON);// activate IR emitters
    one.lcd1("Obstacles  Range");
    one.lcd2("Press a Button!!");
    while(one.readButton()==0);//Wait a button to be pressed <> Espera que pressione um botão
    while(one.readButton()!=0);//Wait for button release <> Espera que largue o botão
}

void loop()
{
    byte rangeL=one.readRangeL(); // read left obstacle sensor range 
    byte rangeR=one.readRangeR(); // read right obstacle sensor range
    one.lcd1("Range Left : ",rangeL);
    one.lcd2("Range Right: ",rangeR);

    if(rangeL>rangeR)
    {
      motR=(vel-(rangeL*kL));
      motL=vel;
    }
    else if(rangeL<rangeR)
    {
      motR=vel;
      motL=(vel-(rangeR*kL));      
    }
    else if(rangeL==rangeR)
    {
      motR=vel;
      motL=vel;
    }
    one.move(motL,motR);
    delay(25); //The robot has new readings every 25ms (40 readings per second)
}