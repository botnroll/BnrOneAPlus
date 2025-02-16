/*
 * This example aquires motor and encoder information to be sent to PIC18F45K22 for PID control of the movement.
 * The robot wheels must rotate freely and should not touch the floor. Motors must have encoders.
 * This process is done in 3 steps:
 * Step 1: PWM will increase from 0 until movement is detected from the encoders.
 * Setp 2: with motors at maximum power, encoders counting will be aqquired every 25ms.
 * Step 3: send data to PIC to be stored in EEPROM.
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>          // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;          // declaration of object variable to control the Bot'n Roll ONE A+

//constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication


int mPow=20;
int encLMax=0;
int encRMax=0;
bool errorFlag=false;
int ks=750;

void setup()
{
  Serial.begin(115200);   // set baud rate to 57600bps for printing values at serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(9.5);
  one.lcd1(" Press a button ");
  one.lcd2("   to start!    ");
  while(one.readButton()==0);
  one.lcd1("Motors Calibrate");
  one.lcd2(" !!Attention!!  ");
  delay(2000);
  one.lcd1("Wheels must not ");
  one.lcd2("touch the Floor!");
  delay(2000);
  one.lcd1(" Press a button ");
  one.lcd2("   to start!    ");
  while(one.readButton()==0);
  one.lcd1("Power EncL EncR ");
}

void loop()
{
  // 1-Detect Start Moving Power
  startMoveDetection();
  // 2-Detect control pulses at max speed every 25ms
  maxPulsesDetection();
  // 3-Send values to PIC18F45K22
  sendValues();
}


void startMoveDetection(){
  bool exitFlag=false;
  int encL=one.readAndResetLeftEncoder(); //Clear encoder count
  int encR=one.readAndResetRightEncoder(); //Clear encoder count
  unsigned long t1sec=millis()+1000;
  while(!exitFlag){

    if(millis()>=t1sec){
      t1sec+=1000;   //Every second
      one.moveRAW(mPow,mPow);
      encL=one.readAndResetLeftEncoder();
      encR=one.readAndResetRightEncoder();
      one.lcd2(mPow,encL,encR);
      Serial.print("  Pow:"); Serial.print(mPow);
      Serial.print("  EncL:"); Serial.print(encL);
      Serial.print("  EncR:"); Serial.print(encR);
      Serial.println();
      if(abs(encL)<100 || abs(encR)<100) mPow++; //if motors are not moving
      else{
        if(encL<0){ //if encoderL is Not ok
            one.lcd1("Motor 1 -> ERROR");
            Serial.println("ERROR: Motor 1 encoder is counting in reverse!!");
            errorFlag=true;
        }
        else if(encR<0){ //if encoderR is Not ok
            one.lcd2("Motor 2 -> ERROR");
            Serial.println("ERROR: Motor 2 encoder is counting in reverse!!");
            errorFlag=true;
        }
        exitFlag=true;
      }
    }
  }
}

void maxPulsesDetection(){
  unsigned long tCycle;
  unsigned long endTime;
  if(!errorFlag){
    one.lcd2(100,0,0);
    one.moveRAW(100,100);
    delay(1500);
    tCycle=millis();
    endTime=millis()+5000;
    int encL=one.readAndResetLeftEncoder(); //Clear encoder count
    int encR=one.readAndResetRightEncoder(); //Clear encoder count
    while(millis()<endTime){
    if(millis()>=tCycle){
      tCycle+=25;
      encL=one.readAndResetLeftEncoder();
      encR=one.readAndResetRightEncoder();
      if(encL>encLMax) encLMax=encL;
      if(encR>encRMax) encRMax=encR;
      Serial.print("  EncL:"); Serial.print(encL);
      Serial.print("  EncR:"); Serial.print(encR);
      Serial.println();
      }
    }
    one.stop();
    one.lcd2(0,encLMax,encRMax);
    Serial.print("  EncLMax:"); Serial.print(encLMax);
    Serial.print("  EncRMax:"); Serial.print(encRMax);
    delay(2000);
  }
}

void sendValues()
{
  int encMin=30000;  //Find minimum encoder value
  if(!errorFlag){
    if(encLMax<encMin) encMin=encLMax;
    if(encRMax<encMin) encMin=encRMax;
    one.setMotors(mPow,ks,encMin);
    Serial.println();
    Serial.println("  Save values for void setMotors(int SMPow, int Ks, int ctrlPulses);");
    Serial.print("  SMPow:"); Serial.println(mPow);
    Serial.print("  ctrlPulses:"); Serial.println(encMin);
    Serial.println("  Calibrate Finished!!");
    while(1){
        one.lcd1("     SMPow: ",mPow);
        one.lcd2("ctrlPulses: ",encMin);
        delay(2500);
        one.lcd1("Save values for ");
        one.lcd2("  setMotors();  ");
        delay(2500);
        one.lcd1("  Calibration   ");
        one.lcd2("   finished!    ");
        delay(2500);
    }
  }
  while(1);
}
