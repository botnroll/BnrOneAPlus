/*
 This example was created by José Cruz (www.botnroll.com)
 on 18 December 2024

 This code example is in the public domain.
 http://www.botnroll.com

 Description:
 This program allows a complete diagnosis of the Bot'n Roll ONE A+ hardware
 This program must be uploaded to the robot when using the Windows APP to
 control Bot'n Roll ONE A+
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>          // SPI communication library required by BnrOneAPlus.cpp
#include <Servo.h>        // Gripper

BnrOneAPlus one;             // declaration of object variable to control the Bot'n Roll ONE A+
BnrColorSensor colorsensor;  // declaration of object variable to control Bot'n Roll Color Sensor
BnrCompass compass;          // declaration of object variable to control the CMPS11 compass

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define COLOR_SENSOR_ADDRESS 0x2C  // default factory address
#define COMPASS_ADDRESS 0x60  // Defines address of CMPS11
#define CONTROL 1  // Delay Time

// TO CLEANUP VARIABLES
float g_battery;
float g_bearing;
char g_roll, g_pitch;
char g_str[50];
int g_encL, g_encR;
int g_b0, g_b1, g_b2, g_b3, g_b4, g_b5, g_b6, g_b7;
byte g_rangeL, g_rangeR;
byte g_buttons;

byte g_rgbL[3] = {0, 0, 0};
byte g_rgbR[3] = {0, 0, 0};


Servo g_gripper1;
Servo g_gripper2;

struct Packet {
  byte AA;
  byte address;
  byte command;
  signed char data[16];
} packet;


void readSerialPacket() {
  int i;

  if (Serial.available() > 0)  // Don't read unless
  {
    packet.AA = Serial.read();  // Read a character
    delay(CONTROL);
    packet.address = Serial.read();  // Read a character
    delay(CONTROL);
    packet.command = Serial.read();  // Read a character
    delay(CONTROL);
    i = 0;
    while (i < 16) {
      packet.data[i++] = Serial.read();  // Read a character
      delay(CONTROL);
    }
  }
}

void setup() {
  Serial.begin(115200);   // sets baud rate to 57600bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.stop();             // stops motors

  colorsensor.i2cConnect(COLOR_SENSOR_ADDRESS);  // Enable I2C communication
  colorsensor.setRgbStatus(ENABLE);              // Enable/Disable RGB scanning

  compass.i2cConnect(COMPASS_ADDRESS);  // Enable I2C communication

  g_gripper1.attach(5);
  g_gripper2.attach(6);

  one.lcd1("Bot'n Roll ONE A");

  delay(1000);  // wait 1 second
}

void loop() {
  if (Serial.available() > 0) {
    readSerialPacket();
    if (packet.AA == 0xAA) {
      switch (packet.command) {
        // ----------- READ ALL SENSOR VALUES ----------------
        case COMMAND_READ_ALL_SENSORS:
          g_battery = one.readBattery();

          g_b0 = one.readAdc(0);
          g_b1 = one.readAdc(1);
          g_b2 = one.readAdc(2);
          g_b3 = one.readAdc(3);
          g_b4 = one.readAdc(4);
          g_b5 = one.readAdc(5);
          g_b6 = one.readAdc(6);
          g_b7 = one.readAdc(7);

          g_buttons = one.readButton();

          g_encL = one.readIncrementalLeftEncoder();
          g_encR = one.readIncrementalRightEncoder();

          g_rangeL = one.readLeftRangeSensor();   // read left obstacle sensor range
          g_rangeR = one.readRightRangeSensor();  // read right obstacle sensor range

          sprintf(g_str,
                  "AL%d.%d;%d;%d,%d;%d,%d;%d,%d,%d,%d,%d,%d,%d,%d",
                  (int)g_battery,
                  (int)((g_battery - (int)g_battery) * 100),
                  g_buttons,
                  g_encL,
                  g_encR,
                  g_rangeL,
                  g_rangeR,
                  g_b0,
                  g_b1,
                  g_b2,
                  g_b3,
                  g_b4,
                  g_b5,
                  g_b6,
                  g_b7);
          Serial.write(g_str);
          break;

        // ----------- COMMAND_LED ----------------
        case COMMAND_LED:
          one.setLed(packet.data[0]);
          break;

        // ----------- COMMAND_LCD_L1 ----------------
        case COMMAND_LCD_L1:
          sprintf(g_str, "%s", packet.data);
          g_str[16] = 0;
          one.lcd1(g_str);
          break;


        // ----------- COMMAND_LCD_L2 ----------------
        case COMMAND_LCD_L2:
          sprintf(g_str, "%s", packet.data);
          g_str[16] = 0;
          one.lcd2(g_str);
          break;

        // ----------- COMMAND_MOVE ----------------
        case COMMAND_MOVE:
          if (packet.data[0] == 0 && packet.data[1] == 0) {
            one.stop();
          } else {
            one.move((signed char)packet.data[0], (signed char)packet.data[1]);
          }
          sprintf(g_str, "%d,%d    ", (signed char)packet.data[0], (signed char)packet.data[1]);
          one.lcd2(g_str);
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
          g_b0 = one.readAdc(0);
          g_b1 = one.readAdc(1);
          g_b2 = one.readAdc(2);
          g_b3 = one.readAdc(3);
          g_b4 = one.readAdc(4);
          g_b5 = one.readAdc(5);
          g_b6 = one.readAdc(6);
          g_b7 = one.readAdc(7);
          sprintf(g_str, "AD%d,%d,%d,%d,%d,%d,%d,%d", g_b0, g_b1, g_b2, g_b3, g_b4, g_b5, g_b6, g_b7);
          Serial.write(g_str);
          one.lcd2(g_str + 2);
          break;

        // ----------- COMMAND_BAT_READ ----------------
        case COMMAND_BAT_READ:
          g_battery = one.readBattery();
          sprintf(g_str, "BV%d.%d", (int)g_battery, (int)((g_battery - (int)g_battery) * 100));
          Serial.write(g_str);
          one.lcd2(g_str + 2);
          break;

        // ----------- COMMAND_BUT_READ ----------------
        case COMMAND_BUT_READ:
          g_buttons = one.readButton();
          sprintf(g_str, "BU%d", g_buttons);
          Serial.write(g_str);
          one.lcd2(g_str + 2);
          break;

        // ----------- COMMAND_ENCODERS ----------------
        case COMMAND_ENCL:
        case COMMAND_ENCR:
          g_encL = one.readAndResetLeftEncoder();
          g_encR = one.readAndResetRightEncoder();
          sprintf(g_str, "EN%d,%d", g_encL, g_encR);
          Serial.write(g_str);
          one.lcd2(g_str + 2);
          break;

        // ----------- COMMAND_OBSTACLES ----------------
        case COMMAND_RANGE_LEFT:
        case COMMAND_RANGE_RIGHT:
          g_rangeL = one.readLeftRangeSensor();   // read left obstacle sensor range
          g_rangeR = one.readRightRangeSensor();  // read right obstacle sensor range
          sprintf(g_str, "RI%d,%d", g_rangeL, g_rangeR);
          Serial.write(g_str);
          one.lcd2(g_str + 2);
          break;

          /* Arduino Write Commands->Don't require response from Bot'n Roll ONE
           * A+ */
        // ----------- COMMAND_SERVO1 ----------------
        case COMMAND_ARDUINO_GRP1:
          g_gripper1.write((unsigned char)packet.data[0]);
          break;

        // ----------- COMMAND_SERVO2 ----------------
        case COMMAND_ARDUINO_GRP2:
          g_gripper2.write((unsigned char)packet.data[0]);
          break;

        // ----------- COMMAND_ARDUINO_BUZZER ----------------

        case COMMAND_ARDUINO_BUZ:
          tone(9, int((unsigned char)packet.data[0] * 8), 25);
          one.lcd2(int((unsigned char)packet.data[0] * 8));
          break;

        // ----------- COMMAND_ARDUINO_COMPASS ----------------
        case COMMAND_ARDUINO_CMP:
          one.lcd2(g_str + 2);
          g_bearing = compass.read_bearing();
          g_roll = compass.read_roll();
          g_pitch = compass.read_pitch();

          sprintf(g_str, "CP%d.%d,%d,%d", (int)g_bearing, (int)((g_bearing - (int)g_bearing) * 100), g_roll, g_pitch);
          Serial.write(g_str);
          break;

        // ----------- COMMAND_READ_COLOR_SENSOR ----------------
        case COMMAND_READ_COLOR_SENSOR:
          colorsensor.readRGBL(&g_rgbL[0], &g_rgbL[1], &g_rgbL[2]);  // Read Left RGB sensor
          colorsensor.readRGBR(&g_rgbR[0], &g_rgbR[1], &g_rgbR[2]);  // Read Right RGB sensor     //
                                                                     // 0xF1 //Save calibration data
          sprintf(g_str,
                  "CS%d,%d,%d;%d,%d,%d",
                  (int)g_rgbL[0],
                  (int)g_rgbL[1],
                  (int)g_rgbL[2],
                  (int)g_rgbR[0],
                  (int)g_rgbR[1],
                  (int)g_rgbR[2]);
          Serial.write(g_str);
          one.lcd2(g_str + 2);
          break;
      }
    }
  }
}