/* 
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 6 
 Trig to Arduino pin 7
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl

 This example was created by José Cruz (www.botnroll.com)
 on 18 December 2024
 
 This code example is in the public domain. 
 http://www.botnroll.com
*/
 
#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <SPI.h>            // required by BnrOneAPlus.cpp
#include <EEPROM.h>         // EEPROM reading and writing

//constants definitions
#define SSPIN  2      //Slave Select (SS) pin for SPI communication

#define echoPin 6    // Echo Pin
#define trigPin 7    // Trigger Pin

BnrOneAPlus one;         //declaration of object variable to control the Bot'n Roll ONE A+

#define maximumRange 200   // Maximum range (200cm)
#define minimumRange   0   // Minimum range

int Sonar()
{
    unsigned long duration;  // Duration used to calculate distance
    int distance;
    unsigned long tempo=micros();
  
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
  
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
  
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 11640);
    delayMicroseconds(16000 - (micros()-tempo));  // this routine has fixed time (16 milliseconds)
  
    //Calculate the distance (in cm) based on the speed of sound
    distance = (int)(duration/58.2);
    if (distance >= maximumRange || distance <= minimumRange)
      distance=-1;
  
    return distance;
}

void setup()
{
    //setup routines
    Serial.begin(57600);    // set baud rate to 115200bps for printing values in serial monitor.
    one.spiConnect(SSPIN);  // start SPI module
    one.stop();             // stop motors

    one.lcd1("www.botnroll.com");
    one.lcd2(" ");
 
    // SONAR setup
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop()
{
    long distance;
  
    distance=Sonar();
    one.lcd2("distance: ", distance);
    Serial.println(distance);
    delay(50);
}

