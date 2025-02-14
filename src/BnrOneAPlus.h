/*
  BnrOneA.h - Library for interfacing with Bot'n Roll ONE A+ Arduino Compatible from www.botnroll.com
  Created by Jos� Cruz, November 28, 2013.
  Updated June 06, 2018. -> String type variable for printing on LCD
  Updated January 04, 2019. -> Improved SPI communication. New line read and calibrate routines.
  Released into the public domain.
*/

#ifndef BnrOneAPlus_h
#define BnrOneAPlus_h

#include <string.h>
#include "Arduino.h"
#include "utility/Config.h"
#include "utility/LineDetector.h"
#include "include/SpiCommands.h"
#include "include/ArduinoCommands.h"

class BnrOneAPlus
{
  public:
        //setup routines
        void spiConnect(byte sspin);
        void setBatMin(float batmin);
        void saveCalibrate(float bat,byte speedL,byte speedR);
        void setPid(int Kp, int Ki, int Kd);  //Set kp,ki,kd values for PID control
        void setMotors(int SMPow, int Ks, int ctrlPulses); //Set Start Moving power, ks gain, pulses every 25ms at max speed.
        void obstacleEmitters(boolean state);// ON/OFF

        //reading routines
        byte obstacleSensors(); //RobertaLab compatibility
        byte readRangeL();
        byte readRangeR();
        int readAdc(byte);
        byte readButton();
        float readBattery();
        int readLine();
        int readLineGaussian();
        int* readLineSensor();
        int readEncL();
        int readEncR();
        int readEncLInc();
        int readEncRInc();
        int readDBG(byte);
        float readDBGf();
        void readFirmware(byte*,byte*,byte*);

        //write routines
        void led(boolean state);// ON/OFF
        void move(int speedL,int speedR);
        void moveRAW(int powerL,int powerR);
        void move1m(byte motor, int speed);
//        void movePID(int speedL,int speedR);
        void stop();
        void brake();                           //Brake with MAX torque
        void brake(byte torqueL,byte torqueR);  //Brake setting the torque
        void stop1m(byte motor);
        void brake1m(byte motor, byte torque);
        void resetEncL();
        void resetEncR();


        //LCD Line 1 write routines
        void lcd1(String string);
        void lcd1(byte string[]);
        void lcd1(const char string[]);
        void lcd1(int number);
        void lcd1(unsigned int number);
        void lcd1(long int number);
        void lcd1(double number);
        void lcd1(const char string[],int number);
        void lcd1(const char string[],unsigned int number);
        void lcd1(const char string[],long int number);
        void lcd1(const char string[],double number);
        void lcd1(unsigned char stringA[8],unsigned char stringB[8]);
        void lcd1(int num1, int num2);
        void lcd1(unsigned int num1, unsigned int num2);
        void lcd1(int num1, int num2, int num3);
        void lcd1(int num1, int num2, int num3, int num4);
        void lcd1(unsigned int num1, unsigned int num2, unsigned int num3);
        void lcd1(unsigned int num1, unsigned int num2, unsigned int num3, unsigned int num4);
        //LCD Line 2 write routines
        void lcd2(String string);
        void lcd2(byte string[]);
        void lcd2(const char string[]);
        void lcd2(int number);
        void lcd2(unsigned int number);
        void lcd2(long int number);
        void lcd2(double number);
        void lcd2(const char string[],int number);
        void lcd2(const char string[],unsigned int number);
        void lcd2(const char string[],long int number);
        void lcd2(const char string[],double number);
        void lcd2(unsigned char stringA[8],unsigned char stringB[8]);
        void lcd2(int num1, int num2);
        void lcd2(unsigned int num1, unsigned int num2);
        void lcd2(int num1, int num2, int num3);
        void lcd2(int num1, int num2, int num3, int num4);
        void lcd2(unsigned int num1, unsigned int num2, unsigned int num3);
        void lcd2(unsigned int num1, unsigned int num2, unsigned int num3, unsigned int num4);


  private:
        byte spiRequestByte(byte command);
        int  spiRequestWord(byte command);
        float spiRequestFloat(byte command);
        void spiSendData(byte command, byte buffer[], byte numBytes);
        byte _sspin;
        byte fmw1=0, fmw2=0, fmw3=0;
        LineDetector _lineDetector;
};
#endif

