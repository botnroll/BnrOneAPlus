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

#define KEY1 0xAA // key used in critical commands
#define KEY2 0x55 // key used in critical commands
//#define BRAKE_TORQUE 100
#define OFF 0
#define ON  1
#define AN0 0
#define AN1 1
#define AN2 2
#define AN3 3
#define AN4 4
#define AN5 5
#define AN6 6
#define AN7 7


/*User Commands*/
/*Read Firmware version*/
#define COMMAND_FIRMWARE        0xFE //Read firmware value (integer value)
/*Write Commands->Don't require response from Bot'n Roll ONE A+ */
#define COMMAND_LED             0xFD //LED
//#define COMMAND_SERVO1          0xFC //Move Servo1
//#define COMMAND_SERVO2          0xFB //Move Servo2
#define COMMAND_LCD_L1          0xFA //Write LCD line1
#define COMMAND_LCD_L2          0xF9 //Write LCD line2
#define COMMAND_IR_EMITTERS     0xF8 //IR Emmiters ON/OFF
#define COMMAND_STOP            0xF7 //Stop motors freeley
#define COMMAND_MOVE            0xF6 //Move motors
#define COMMAND_BRAKE_SET_T     0xF5 //Stop motors setting brake torque
#define COMMAND_SET_BAT_MIN     0xF4 //Set low battery voltage
#define COMMAND_SET_PID  	      0xF3 //Set kp,ki,kd values for PID control
#define COMMAND_MOVE_RAW        0xF2 //Set motors calibration parameters (SPow, Ks, ctrlPulses)
#define COMMAND_SET_MOTORS      0xF1 //Save calibration data
#define COMMAND_ENCL_RESET      0xF0 //Preset the value of encoder1
#define COMMAND_ENCR_RESET      0xEF //Preset the value of encoder2
#define COMMAND_FUTURE_USE2     0xEE   
#define COMMAND_FUTURE_USE3     0xED 
#define COMMAND_FUTURE_USE4     0xEC 
#define COMMAND_BRAKE_MAX_T     0xEB //Stop motors with MAX brake torque
#define COMMAND_MOVE_1M         0xEA //Move 1 motor
#define COMMAND_STOP_1M         0xE9 //Stop 1 motor
#define COMMAND_BRAKE_1M        0xE8 //Brake 1 motor
#define COMMAND_FUTURE_USE5	    0xE7 

/*Read Commands-> requests to Bot'n Roll ONE A+ */
#define COMMAND_ADC0            0xDF //Read ADC0
#define COMMAND_ADC1            0xDE //Read ADC1
#define COMMAND_ADC2            0xDD //Read ADC2
#define COMMAND_ADC3            0xDC //Read ADC3
#define COMMAND_ADC4            0xDB //Read ADC4
#define COMMAND_ADC5            0xDA //Read ADC5
#define COMMAND_ADC6            0xD9 //Read ADC6
#define COMMAND_ADC7            0xD8 //Read ADC7
#define COMMAND_BAT_READ	0xD7 //Read ADC battery
#define COMMAND_BUT_READ	0xD6 //Read ADC button
#define COMMAND_OBSTACLES       0xD5 //Read IR obstacle sensors -> RobertaLab compatibility
#define COMMAND_FUTURE_USE7     0xD4 
#define COMMAND_ENCL            0xD3 //Read Encoder1 position
#define COMMAND_ENCR            0xD2 //Read Encoder2 position
#define COMMAND_ENCL_INC	0xD1 //Read Encoder1 Incremental value
#define COMMAND_ENCR_INC	0xD0 //Read Encoder2 Incremental value
#define COMMAND_LINE_READ	0xCF //Read 16bytes line buffer at once
#define COMMAND_RANGE_LEFT      0xCE //Read IR obstacles distance range
#define COMMAND_RANGE_RIGHT     0xCD //Read IR obstacles distance range

/*Read Commands-> Computer to Bot'n Roll ONE A+*/
#define COMMAND_ARDUINO_ANA0	0xBF //Read analog0 value
#define COMMAND_ARDUINO_ANA1	0xBE //Read analog1 value
#define COMMAND_ARDUINO_ANA2	0xBD //Read analog2 value
#define COMMAND_ARDUINO_ANA3	0xBC //Read analog3 value
#define COMMAND_ARDUINO_DIG0	0xBB //Read digital0 value
#define COMMAND_ARDUINO_DIG1	0xBA //Read digital1 value
//#define COMMAND_ARDUINO_DIG2	0xB9 //Read digital2 value
#define COMMAND_ARDUINO_DIG3	0xB8 //Read digital3 value
#define COMMAND_ARDUINO_DIG4	0xB7 //Read digital4 value
#define COMMAND_ARDUINO_DIG5	0xB6 //Read digital5 value
#define COMMAND_ARDUINO_DIG6	0xB5 //Read digital6 value
#define COMMAND_ARDUINO_DIG7	0xB4 //Read digital7 value
#define COMMAND_ARDUINO_DIG8	0xB3 //Read digital8 value
#define COMMAND_ARDUINO_DIG9	0xB2 //Read digital9 value
#define COMMAND_ARDUINO_DIG10	0xB1 //Read digital10 value
#define COMMAND_ARDUINO_DIG11	0xB0 //Read digital11 value
#define COMMAND_ARDUINO_DIG12	0xAF //Read digital12 value
#define COMMAND_ARDUINO_DIG13	0xAE //Read digital13 value
#define COMMAND_ARDUINO_BUZ     0xAD //Read Buzzer
#define COMMAND_ARDUINO_CMP     0xAC //Read Compass
#define COMMAND_ARDUINO_SNR     0xAB //Read Sonar
#define COMMAND_ARDUINO_GRP1    0xAA //Read gripper1
#define COMMAND_ARDUINO_GRP2    0x9F //Read gripper2


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

