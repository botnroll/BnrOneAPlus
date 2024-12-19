/* 
 This example was created by José Cruz on December 2024
 
 This code example is in the public domain. 
 http://www.botnroll.com

Line Following:
15 possible values for line position:-100 -87 -75 -62 -50 -37 -25 0 +25 +37 +50 +62 +75 +87 +100
The speed of the motors is set for every possible case.
The RGB LED allows identifying the line position in every moment.
<>
Seguimento de linha:
15 valores possiveis para a posiçao da linha: -100 -87 -75 -62 -50 -37 -25 0 +25 +37 +50 +62 +75 +87 +100
A velocidade dos motores é ajustada para cada um dos casos.
O LED RGB permite identificar que caso se verifica a cada momento.
*/
 
#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;           // declaration of object variable to control the Bot'n Roll ONE A+

//constants definitions
#define SSPIN  2    //Slave Select (SS) pin for SPI communication
#define M1  1       //Motor1
#define M2  2       //Motor2

#define Vtrans 300 //Line follower limit between white and black 

float batmin=10.5; // safety voltage for discharging the battery

int vel=50;


void setup() 
{  
  Serial.begin(57600);     // sets baud rate to 57600bps for printing values at serial monitor.
  one.spiConnect(SSPIN);   // starts the SPI communication module
  one.stop();              // stop motors
  one.minBat(batmin); 	   // safety voltage for discharging the battery
  delay(1000);
}


void loop() 
{
  int v25=10; //Speed for line value 25 <> Velocidade para o valor da linha 25
  int v37=15; 
  int v50=20; 
  int v62=25; 
  int v75=30; 
  int v87=30; 
  int v100=35;

  int line=readLine();
  Serial.print("  Line:");Serial.println(line);  

  switch(line)
  {
      case -100:
        one.move(-1,vel+v100);        
        break;
        
      case -87:
        one.move(-1,vel+v87);
        break;

      case -75:
        one.move(vel-v75,vel+v75);
        break;

      case -62:
        one.move(vel-v62,vel+v62);
        break;

      case -50:
        one.move(vel-v50,vel+v50);
        break;

      case -37:
        one.move(vel-v37,vel+v37);
        break;
  
      case -25:
        one.move(vel-v25,vel+v25);
        break;

      case 0:
        one.move(vel,vel);
        break;
  
      case 25:
        one.move(vel+v25,vel-v25);
        break;

      case 37:
        one.move(vel+v37,vel-v37);
        break;

      case 50:
        one.move(vel+v50,vel-v50);
        break;

      case 62:
        one.move(vel+v62,vel-v62);
        break;

      case 75:
        one.move(vel+v75,vel-v75);
        break;

      case 87:
        one.move(vel+v87,-1);
        break;

      case 100:
        one.move(vel+v100,-1);
        break;
  }
}


int readLine()
{
    int lineValue=0;
    int sensorCount=0;
    if(one.readAdc(0)>Vtrans) //Test Sensor1  <>  Testa o sensor1
      {                       
        lineValue-=100;
        sensorCount++;
      }
    if(one.readAdc(1)>Vtrans) //Test Sensor2  <>  Testa o sensor2
      {                       
        lineValue-=75;
        sensorCount++;
      }
    if(one.readAdc(2)>Vtrans)
      {
        lineValue-=50;
        sensorCount++;
      }
    if(one.readAdc(3)>Vtrans)
      {
        lineValue-=25;
        sensorCount++;
      }
    if(one.readAdc(4)>Vtrans)
      {
        lineValue+=25;
        sensorCount++;
      }
    if(one.readAdc(5)>Vtrans)
      {
        lineValue+=50;
        sensorCount++;
      }
    if(one.readAdc(6)>Vtrans)
      {
        lineValue+=75;
        sensorCount++;
      }
    if(one.readAdc(7)>Vtrans) //Test Sensor8  <>  Testa o sensor8
      {                       
        lineValue+=100;
        sensorCount++;
      }
    if(sensorCount>2)
        lineValue=-1;
    else if(sensorCount>0)
        lineValue=lineValue/sensorCount;
    return lineValue;
}



