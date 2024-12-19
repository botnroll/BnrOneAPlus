/* 
 This example was created by José Cruz (www.botnroll.com)
 on the 18th December 2024
 
 This code example is in the public domain. 
 http://www.botnroll.com

Description: 
 Configure the necessary power in order to move the motors for the lowest possible speed. 
 Use a fully charged battery for the configuration.
 Information is printed in the robot LCD and computer Serial monitor.
 Place the robot on a flat surface.
 Use PB1 to increase the power until the robot moves forward.
 Use PB3 to store the data in EEPROM. 
 Use PB2 to decrease the power if necessary.

Motors Calibration.
*/

#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;           // declaration of object variable to control the Bot'n Roll ONE A+

//constants definition
#define SSPIN  2       // Slave Select (SS) pin for SPI communication

int powerL=40, powerR=40,button=0;
float battery=0.0;
unsigned long t1sec=1000;

void setup() 
{
    Serial.begin(57600);     // set baud rate to 57600bps for printing values at serial monitor.
    one.spiConnect(SSPIN);   // start SPI communication module
    one.stop();              // stop motors
    one.minBat(9.5);
    battery=one.readBattery();
}

void loop()
{
  if(millis()>t1sec) //Every 250ms
  {
    t1sec+=250;
    battery=one.readBattery();
    one.moveCalibrate(powerL,powerR);
    one.lcd1("Bateria V: ",battery);
    one.lcd2(powerL,powerR);
  }
  button=one.readButton();
  if(button==1)
  {
    powerL++;
    powerR++;
    Serial.print("PowerL:");Serial.print(powerL);
    Serial.print("  PowerR:");Serial.print(powerR);
    Serial.print("  Battery:");Serial.print(battery);
    Serial.println();      
    while(one.readButton()!=0)
    {
      delay(50);
    }
  }
  else if(button==2)
  {
    powerL--;
    powerR--;
    Serial.print("PowerL:");Serial.print(powerL);
    Serial.print("  PowerR:");Serial.print(powerR);
    Serial.print("  Battery:");Serial.print(battery);
    Serial.println();      
    while(one.readButton()!=0)
    {
      delay(50);
    }
  }  
  else if(button==3)
  {
    one.saveCalibrate(battery,(byte)powerL,(byte)powerR);
    Serial.print("Calibration data saved:");
    one.lcd1("Calibration data");
    one.lcd2("    Saved!!!    ");
    delay(2000);
    Serial.print("  Battery:");Serial.print(battery);
    Serial.print("  PowerL:");Serial.print(powerL);
    Serial.print("  PowerR:");Serial.print(powerR);
    Serial.println();      
    while(one.readButton()!=0)
    {
      delay(50);
    }
  }
}