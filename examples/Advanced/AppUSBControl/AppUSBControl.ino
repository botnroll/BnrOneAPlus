/* 
 This example was created by José Cruz (www.botnroll.com)
 on 18 December 2024
 
 This code example is in the public domain. 
 http://www.botnroll.com

 Description:
 This program allows a complete diagnosis of the Bot'n Roll ONE A+ hardware
 This program must be uploaded to the robot when using the Windows APP to control Bot'n Roll ONE A+
*/

#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <EEPROM.h>        // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp
#include <Servo.h>     // Gripper

BnrOneAPlus one;           // declaration of object variable to control the Bot'n Roll ONE A+
BnrColorSensor colorsensor;         // declaration of object variable to control Bot'n Roll Color Sensor
BnrCompass compass;        // declaration of object variable to control the CMPS11 compass

//constants definition
#define SSPIN  2       // Slave Select (SS) pin for SPI communication
#define COLOR_SENSOR_ADDRESS 0x2C        //default factory address
#define COMPASS_ADDRESS 0x60   // Defines address of CMPS11
#define CONTROL        1  // Delay Time


// TO CLEANUP VARIABLES
byte firmware[2]={0,0};
byte sonarL=0, sonarC=0, sonarR=0;
byte rgbL[3]={0,0,0};
byte rgbR[3]={0,0,0};
char str[50];
int *tempint;
byte buttons;
float battery;

float bearing;
char roll, pitch;
int encL, encR;
int b0, b1, b2, b3, b4, b5, b6, b7;
byte rangeL, rangeR;


Servo gripper1;
Servo gripper2;

int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };



struct TRAMA
{
  byte AA;
  byte address;
  byte command;
  signed char data[16]; 
} trama;

void read_serial_packet()
{
 int i;


 if (Serial.available() > 0) // Don't read unless
 {
    trama.AA = Serial.read(); // Read a character
    delay(CONTROL);
    trama.address = Serial.read(); // Read a character
    delay(CONTROL);
    trama.command = Serial.read(); // Read a character
    delay(CONTROL);
    i=0;
    while (i<16)
    {
      trama.data[i++] = Serial.read(); // Read a character    
      delay(CONTROL);
    }
 }
}

void setup() 
{
    Serial.begin(115200);     // sets baud rate to 57600bps for printing values at serial monitor.
    one.spiConnect(SSPIN);   // starts the SPI communication module
    one.stop();              // stops motors

    colorsensor.i2cConnect(COLOR_SENSOR_ADDRESS);    //Enable I2C communication
    colorsensor.setRgbStatus(ENABLE);     //Enable/Disable RGB scanning

    compass.i2cConnect(COMPASS_ADDRESS);    //Enable I2C communication

    gripper1.attach(5);
    gripper2.attach(6);
    
    one.lcd1("Bot'n Roll ONE A");

    delay(1000);     // wait 1 second
}


void loop()
{
  if (Serial.available() > 0) {
    read_serial_packet();
    if (trama.AA==0xAA) {
      switch(trama.command) {
        // ----------- READ ALL SENSOR VALUES ----------------
        case COMMAND_READ_ALL_SENSORS:
            battery=one.readBattery();         

            b0 = one.readAdc(0); 
            b1 = one.readAdc(1);
            b2 = one.readAdc(2);
            b3 = one.readAdc(3);
            b4 = one.readAdc(4);
            b5 = one.readAdc(5);
            b6 = one.readAdc(6);
            b7 = one.readAdc(7);
            
            buttons=one.readButton();
            
            encL = one.readAndIncrementLeftEncoder();
            encR = one.readAndIncrementRightEncoder();

            rangeL = one.readLeftRangeSensor(); // read left obstacle sensor range 
            rangeR = one.readRightRangeSensor(); // read right obstacle sensor range

            sprintf(str, "AL%d.%d;%d;%d,%d;%d,%d;%d,%d,%d,%d,%d,%d,%d,%d", (int)battery, (int)((battery-(int)battery)*100), buttons, encL, encR, rangeL, rangeR, b0, b1, b2, b3, b4, b5, b6, b7);
            Serial.write(str);
            break;

        // ----------- COMMAND_LED ----------------
        case COMMAND_LED:
            one.setLed(trama.data[0]);
            break;
        
        // ----------- COMMAND_LCD_L1 ----------------
        case COMMAND_LCD_L1:
            sprintf(str, "%s", trama.data);
            str[16] = 0;
            one.lcd1(str);
            break;


        // ----------- COMMAND_LCD_L2 ----------------
        case COMMAND_LCD_L2:
            sprintf(str, "%s", trama.data);
            str[16] = 0;
            one.lcd2(str);
            break;
            
        // ----------- COMMAND_MOVE ----------------
        case COMMAND_MOVE:
            if (trama.data[0] == 0 && trama.data[1] == 0) {
                one.stop();
            } else {
                one.move((signed char)trama.data[0], (signed char)trama.data[1]);
            }
            sprintf(str, "%d,%d    ", (signed char)trama.data[0], (signed char)trama.data[1]);
            one.lcd2(str);
            break;


        // ----------- COMMAND_READ_LINE_SENSOR ---------------- 
        case COMMAND_ADC0:
        case COMMAND_ADC1:
        case COMMAND_ADC2:
        case COMMAND_ADC3:
        case COMMAND_ADC4:
        case COMMAND_ADC5:
        case COMMAND_ADC6:
        case COMMAND_ADC7:
            b0 = one.readAdc(0); 
            b1 = one.readAdc(1);
            b2 = one.readAdc(2);
            b3 = one.readAdc(3);
            b4 = one.readAdc(4);
            b5 = one.readAdc(5);
            b6 = one.readAdc(6);
            b7 = one.readAdc(7);
            sprintf(str, "AD%d,%d,%d,%d,%d,%d,%d,%d", b0, b1, b2, b3, b4, b5, b6, b7);
            Serial.write(str);
            one.lcd2(str+2);
            break;

        // ----------- COMMAND_BAT_READ ----------------
        case COMMAND_BAT_READ:
            battery = one.readBattery();                              
            sprintf(str, "BV%d.%d", (int)battery, (int)((battery-(int)battery)*100));
            Serial.write(str);
            one.lcd2(str+2);
            break;

        // ----------- COMMAND_BUT_READ ----------------
        case COMMAND_BUT_READ:
            buttons = one.readButton();
            sprintf(str, "BU%d", buttons);
            Serial.write(str);
            one.lcd2(str+2);
            break;

        // ----------- COMMAND_ENCODERS ----------------
        case COMMAND_ENCL:
        case COMMAND_ENCR:
            encL = one.readAndResetLeftEncoder();
            encR = one.readAndResetRightEncoder();
            sprintf(str, "EN%d,%d", encL, encR);
            Serial.write(str);
            one.lcd2(str+2);
            break;

        // ----------- COMMAND_OBSTACLES ----------------
        case COMMAND_RANGE_LEFT:
        case COMMAND_RANGE_RIGHT:
            rangeL = one.readLeftRangeSensor(); // read left obstacle sensor range 
            rangeR = one.readRightRangeSensor(); // read right obstacle sensor range
            sprintf(str, "RI%d,%d", rangeL, rangeR);
            Serial.write(str);
            one.lcd2(str+2);
            break;
  
    /* Arduino Write Commands->Don't require response from Bot'n Roll ONE A+ */
        // ----------- COMMAND_SERVO1 ----------------
        case COMMAND_ARDUINO_GRP1: 
            gripper1.write((unsigned char)trama.data[0]); 
            break;

        // ----------- COMMAND_SERVO2 ----------------
        case COMMAND_ARDUINO_GRP2: 
            gripper2.write((unsigned char)trama.data[0]); 
            break;

        // ----------- COMMAND_ARDUINO_BUZZER ----------------
        case COMMAND_ARDUINO_BUZ:  
            tone(9, int((unsigned char)trama.data[0]*8), 25);
            one.lcd2(int((unsigned char)trama.data[0]*8));
            break;
                              
        // ----------- COMMAND_ARDUINO_COMPASS ----------------
        case COMMAND_ARDUINO_CMP:  
            one.lcd2(str+2);
            bearing = compass.read_bearing();
            roll = compass.read_roll();
            pitch = compass.read_pitch();
                
            sprintf(str,"CP%d.%d,%d,%d", (int)bearing, (int)((bearing-(int)bearing)*100), roll, pitch);
            Serial.write(str);
            break;
        
        // ----------- COMMAND_READ_COLOR_SENSOR ---------------- 
        case COMMAND_READ_COLOR_SENSOR:
            colorsensor.readRgbL(&rgbL[0],&rgbL[1],&rgbL[2]);      //Read Left RGB sensor
            colorsensor.readRgbR(&rgbR[0],&rgbR[1],&rgbR[2]);      //Read Right RGB sensor     //  0xF1 //Save calibration data
            sprintf(str, "CS%d,%d,%d;%d,%d,%d", (int)rgbL[0], (int)rgbL[1], (int)rgbL[2], (int)rgbR[0], (int)rgbR[1], (int)rgbR[2]);
            Serial.write(str);
            one.lcd2(str+2);
            break;
      }
    }
  }
}