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
#include <SPI.h>    // SPI communication library required by BnrOneAPlus.cpp
#include <Servo.h>  // Gripper
#include <Wire.h>   // Compass

BnrOneAPlus
    one;  // declaration of object variable to control the Bot'n Roll ONE A+
Servo gripper1;
Servo gripper2;

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication

#define ADDRESS 0x60  // Defines address of CMPS11

#define echoPin 6         // Echo Pin
#define trigPin 7         // Trigger Pin
#define maximumRange 200  // Maximum range needed (200cm)
#define minimumRange 0    // Minimum range needed

#define CONTROL 5  // Delay Time

struct TRAMA {
  byte AA;
  byte address;
  byte command;
  signed char data[16];
} trama;

float read_bearing() {
  byte highByte, lowByte;  // highByte and lowByte store the bearing and fine
                           // stores decimal place of bearing

  Wire.beginTransmission(ADDRESS);  // starts communication with CMPS11
  Wire.write(2);  // Sends the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(ADDRESS, 2);  // Request 4 bytes from CMPS11
  while (Wire.available() < 2);  // Wait for bytes to become available
  highByte = Wire.read();
  lowByte = Wire.read();

  return (float)((highByte << 8) + lowByte) / 10;
}

char read_roll() {
  char roll;  // Stores  roll values of CMPS11, chars are used because they
              // support signed value

  Wire.beginTransmission(ADDRESS);  // starts communication with CMPS11
  Wire.write(5);  // Sends the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(ADDRESS, 1);  // Request 4 bytes from CMPS11
  while (Wire.available() < 1);  // Wait for bytes to become available
  roll = Wire.read();
  return roll;
}

char read_pitch() {
  char pitch;  // Stores pitch values of CMPS11, chars are used because they
               // support signed value

  Wire.beginTransmission(ADDRESS);  // starts communication with CMPS11
  Wire.write(4);  // Sends the register we wish to start reading from
  Wire.endTransmission();

  Wire.requestFrom(ADDRESS, 1);  // Request 4 bytes from CMPS11
  while (Wire.available() < 1);  // Wait for bytes to become available
  pitch = Wire.read();

  return pitch;
}

void le_trama() {
  int i;

  //  if (Serial.available() > 0) // Don't read unless
  //  {
  trama.AA = Serial.read();  // Read a character
  delay(CONTROL);
  trama.address = Serial.read();  // Read a character
  delay(CONTROL);
  trama.command = Serial.read();  // Read a character
  delay(CONTROL);
  i = 0;
  while (i < 16) {
    trama.data[i++] = Serial.read();  // Read a character
    delay(CONTROL);
  }
  //  }
}

void setup() {
  Serial.begin(115200);    // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.stop();             // stops motors

  Wire.begin();  // Start I2C BUS

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  gripper1.attach(3);
  gripper2.attach(5);

  one.lcd1("Bot'n Roll ONE A");

  delay(1000);  // wait 1 second
}

void loop() {
  static int opcao = 0;

  // Temporary variables;
  char str[50];
  float *tempfloatp;
  int *tempint;
  byte my_byte;
  float battery;

  if (Serial.available() > 0) {
    le_trama();
    if (trama.AA == 0xAA) {
      switch (trama.command) {
        case COMMAND_FIRMWARE:  //  0xFE // Read firmware value (integer value)
          break;

          /* Write Commands->Don't require response from Bot'n Roll ONE A+ */
        case COMMAND_LED:  //  0xFD // Debug LED
          one.setLed(trama.data[0]);
          break;
        case COMMAND_SERVO1:  //  0xFC // Move Servo1
          one.servo1(trama.data[0]);
          break;
        case COMMAND_SERVO2:  //  0xFB // Move Servo2
          one.servo2(trama.data[0]);
          break;
        case COMMAND_LCD_L1:  //  0xFA // Write LCD line1
          sprintf(str, "%s", trama.data);
          str[16] = 0;
          one.lcd1(str);
          break;
        case COMMAND_LCD_L2:  //  0xF9 // Write LCD line2
          sprintf(str, "%s", trama.data);
          str[16] = 0;
          one.lcd2(str);
          break;
        case COMMAND_IR_EMITTERS:  //  0xF8 // IR Emitters ON/OFF
          one.obstacleSensorsEmitters(trama.data[0]);
          break;
        case COMMAND_STOP:  //  0xF7 // Stop motors freely
          one.stop();
          break;
        case COMMAND_MOVE:  //  0xF6 // Move motors with no PID control
          one.move((signed char)trama.data[0], (signed char)trama.data[1]);
          sprintf(str,
                  "%d,%d    ",
                  (signed char)trama.data[0],
                  (signed char)trama.data[1]);
          one.lcd2(str);
          break;
        case COMMAND_BRAKE:  //  0xF5 // Stop motors with brake torque
          one.brake((byte)trama.data[0], (byte)trama.data[1]);
          sprintf(str,
                  "%d,%d    ",
                  (signed char)trama.data[0],
                  (signed char)trama.data[1]);
          one.lcd2(str);
          break;
        case COMMAND_BAT_MIN:  //  0xF4 // Configure low battery level
          tempfloatp = (float *)trama.data;
          one.setMinBatteryV(*tempfloatp);
          break;
        case COMMAND_MOVE_PID:  //  0xF3 // Move motor with PID control
          break;
        case COMMAND_MOVE_CALIBRATE:  //  0xF2 //Move motors for calibration
          break;
        case COMMAND_SAVE_CALIBRATE:  //  0xF1 //Save calibration data
          break;
        case COMMAND_ENCL_RESET:  //  0xF0 // Preset the value of encoder1
          break;
        case COMMAND_ENCR_RESET:  //  0xEF // Preset the value of encoder2
          break;

          /* Read Commands-> requests to Bot'n Roll ONE A+ */
        case COMMAND_ADC0:  //  0xDF // Read ADC0
        case COMMAND_ADC1:  //  0xDE // Read ADC1
        case COMMAND_ADC2:  //  0xDD // Read ADC2
        case COMMAND_ADC3:  //  0xDC // Read ADC3
        case COMMAND_ADC4:  //  0xDB // Read ADC4
        case COMMAND_ADC5:  //  0xDA // Read ADC5
        case COMMAND_ADC6:  //  0xD9 // Read ADC6
        case COMMAND_ADC7:  //  0xD8 // Read ADC7
        {
          int b0, b1, b2, b3, b4, b5, b6, b7;
          b0 = one.readAdc(0);
          b1 = one.readAdc(1);
          b2 = one.readAdc(2);
          b3 = one.readAdc(3);
          b4 = one.readAdc(4);
          b5 = one.readAdc(5);
          b6 = one.readAdc(6);
          b7 = one.readAdc(7);
          sprintf(
              str, "AD%d,%d,%d,%d,%d,%d,%d,%d", b0, b1, b2, b3, b4, b5, b6, b7);
          Serial.write(str);
          one.lcd2(str + 2);
        } break;
        case COMMAND_BAT_READ:  //  0xD7 // Read ADC battery
          battery = one.readBattery();
          sprintf(str,
                  "BV%d.%d",
                  (int)battery,
                  (int)((battery - (int)battery) * 100));
          Serial.write(str);
          one.lcd2(str + 2);
          break;
        case COMMAND_BUT_READ:  //  0xD6 // Read ADC button
          my_byte = one.readButton();
          sprintf(str, "BU%d", my_byte);
          Serial.write(str);
          one.lcd2(str + 2);
          break;
        case COMMAND_OBSTACLES:  //  0xD5 // Read IR obstacle sensors
          my_byte = one.readObstacleSensors();
          sprintf(str, "OB%d", my_byte);
          Serial.write(str);
          one.lcd2(str + 2);
          break;
        case COMMAND_IR_SENSORS:  //  0xD4 // Read IR sensors instant value
          my_byte = one.readIRSensors();
          sprintf(str, "IR%d", my_byte);
          Serial.write(str);
          one.lcd2(str + 2);
          break;
        case COMMAND_ENCL:  //  0xD3 // Read Encoder1 position
          break;
        case COMMAND_ENCR:  //  0xD2 // Read Encoder2 position
          break;
        case COMMAND_ENCL_INC:  //  0xD1 // Read Encoder1 Incremental value
          break;
        case COMMAND_ENCR_INC:  //  0xD0 // Read Encoder2 Incremental value
          break;
        case COMMAND_LINE_READ:  //  0xCF //Read Line Value (-100 +100)
          break;
        case COMMAND_RANGE_LEFT:  //  0xCE //Read IR obstacles distance range
          break;
        case COMMAND_RANGE_RIGHT:  //  0xCD //Read IR obstacles distance range

          break;

          /* Arduino Write Commands->Don't require response from Bot'n Roll ONE
           * A+ */

        case COMMAND_ARDUINO_BUZ:  //  0xad // buzzer
          sprintf(str,
                  "%c%c%c%c%c",
                  trama.data[0],
                  trama.data[1],
                  trama.data[2],
                  trama.data[3],
                  trama.data[4]);
          tone(9, atoi(str), 250);
          delay(250);
          noTone(9);
          one.lcd2("Play Music");
          break;
        case COMMAND_ARDUINO_CMP:  //  0xac // compass
        {
          float bearing;
          char roll, pitch;

          bearing = read_bearing();
          roll = read_roll();
          pitch = read_pitch();

          sprintf(str,
                  "CP%d.%d,%d,%d",
                  (int)bearing,
                  (int)((bearing - (int)bearing) * 100),
                  roll,
                  pitch);
          Serial.write(str);
          one.lcd2(str + 2);
        } break;
        case COMMAND_ARDUINO_SNR:  //  0xac // compass
        {
          long distance;
          unsigned long duration;  // Duration used to calculate distance
          unsigned long tempo = micros();

          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);

          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);

          digitalWrite(trigPin, LOW);
          duration = pulseIn(echoPin, HIGH, 11640);
          delayMicroseconds(
              16000 -
              (micros() -
               tempo));  // this routine has fixed time (16 milliseconds)

          // Calculate the distance (in cm) based on the speed of sound
          distance = (int)(duration / 58.2);
          if (distance >= maximumRange || distance <= minimumRange)
            distance = -1;

          sprintf(str, "SN%ld", distance);
          Serial.write(str);
          one.lcd2(str + 2);
        } break;
        case COMMAND_ARDUINO_GRP1:  //  0xac // compass
          gripper1.write((unsigned char)trama.data[0]);
          break;
        case COMMAND_ARDUINO_GRP2:  //  0xac // compass
          gripper2.write((unsigned char)trama.data[0]);
          break;
      }
    }
  }
}