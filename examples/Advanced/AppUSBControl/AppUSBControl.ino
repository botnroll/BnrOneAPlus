/**
 * This example was created by José Cruz (www.botnroll.com)
 * on 18 December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * This program allows a complete diagnosis of the Bot'n Roll ONE A+ hardware
 * This program must be uploaded to the robot when using the Windows APP to
 * control Bot'n Roll ONE A+
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>    // SPI communication library required by BnrOneAPlus.cpp
#include <Servo.h>  // Gripper
#include <Wire.h>   // Compass

BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+
Servo gripper1;
Servo gripper2;

// constants definition
#define ADDRESS 0x60   // Defines address of CMPS11
#define CONTROL 5      // Delay Time
#define ECHO_PIN 6     // Echo Pin
#define TRIGGER_PIN 7  // Trigger Pin
#define MAX_RANGE 200  // Maximum range needed (200cm)
#define MIN_RANGE 0    // Minimum range needed
#define SSPIN 2        // Slave Select (SS) pin for SPI communication

struct Message {
  byte AA;
  byte address;
  byte command;
  signed char data[16];
};

float readBearing() {
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

char readRoll() {
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

char readPitch() {
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

Message read_message() {
  int i;
  struct Message message;
  message.AA = Serial.read();  // Read a character
  delay(CONTROL);
  message.address = Serial.read();  // Read a character
  delay(CONTROL);
  message.command = Serial.read();  // Read a character
  delay(CONTROL);
  i = 0;
  while (i < 16) {
    message.data[i++] = Serial.read();  // Read a character
    delay(CONTROL);
  }

  return message;
}

void setup() {
  Serial.begin(57600);    // sets baud rate to 57600bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.stop();             // stops motors

  Wire.begin();  // Start I2C BUS

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  gripper1.attach(3);
  gripper2.attach(5);

  one.lcd1("Bot'n Roll ONE A");

  delay(1000);  // wait 1 second
}

void loop() {
  // Temporary variables;
  char str[50];
  float *tempFloatP;
  byte myByte;
  float battery;

  if (Serial.available() > 0) {
    Message message = read_message();
    if (message.AA == 0xAA) {
      switch (message.command) {
        case COMMAND_FIRMWARE:  // Read firmware value (integer value)
          break;

          /* Write Commands->Don't require response from Bot'n Roll ONE A+ */
        case COMMAND_LED:  // Debug LED
          one.setLed(message.data[0]);
          break;
        case COMMAND_LCD_L1:  // Write LCD line1
          sprintf(str, "%s", message.data);
          str[16] = 0;
          one.lcd1(str);
          break;
        case COMMAND_LCD_L2:  // Write LCD line2
          sprintf(str, "%s", message.data);
          str[16] = 0;
          one.lcd2(str);
          break;
        case COMMAND_IR_EMITTERS:  // IR Emitters ON/OFF
          one.obstacleSensorsEmitters(message.data[0]);
          break;
        case COMMAND_STOP:  // Stop motors freely
          one.stop();
          break;
        case COMMAND_MOVE:  //  Move motors with PID control
          one.move((signed char)message.data[0], (signed char)message.data[1]);
          sprintf(str,
                  "%d,%d    ",
                  (signed char)message.data[0],
                  (signed char)message.data[1]);
          one.lcd2(str);
          break;
        case COMMAND_BRAKE_SET_T:  // Stop motors with brake torque
          one.brake((byte)message.data[0], (byte)message.data[1]);
          sprintf(str,
                  "%d,%d    ",
                  (signed char)message.data[0],
                  (signed char)message.data[1]);
          one.lcd2(str);
          break;
        case COMMAND_SET_BAT_MIN:  // Configure low battery level
          tempFloatP = (float *)message.data;
          one.setMinBatteryV(*tempFloatP);
          break;
        case COMMAND_ENCL_RESET:  // Preset the value of encoder1
          break;
        case COMMAND_ENCR_RESET:  // Preset the value of encoder2
          break;

          /* Read Commands-> requests to Bot'n Roll ONE A+ */
        case COMMAND_ADC0:  // Read ADC0
        case COMMAND_ADC1:  // Read ADC1
        case COMMAND_ADC2:  // Read ADC2
        case COMMAND_ADC3:  // Read ADC3
        case COMMAND_ADC4:  // Read ADC4
        case COMMAND_ADC5:  // Read ADC5
        case COMMAND_ADC6:  // Read ADC6
        case COMMAND_ADC7:  // Read ADC7
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
        case COMMAND_BAT_READ:  // Read ADC battery
          battery = one.readBattery();
          sprintf(str,
                  "BV%d.%d",
                  (int)battery,
                  (int)((battery - (int)battery) * 100));
          Serial.write(str);
          one.lcd2(str + 2);
          break;
        case COMMAND_BUT_READ:  // Read ADC button
          myByte = one.readButton();
          sprintf(str, "BU%d", myByte);
          Serial.write(str);
          one.lcd2(str + 2);
          break;
        case COMMAND_OBSTACLES:  // Read IR obstacle sensors
          myByte = one.readObstacleSensors();
          sprintf(str, "OB%d", myByte);
          Serial.write(str);
          one.lcd2(str + 2);
          break;
        case COMMAND_ENCL:  // Read Encoder1 position
          break;
        case COMMAND_ENCR:  // Read Encoder2 position
          break;
        case COMMAND_ENCL_INC:  // Read Encoder1 Incremental value
          break;
        case COMMAND_ENCR_INC:  // Read Encoder2 Incremental value
          break;
        case COMMAND_LINE_READ:  // Read Line Value (-100 +100)
          break;
        case COMMAND_RANGE_LEFT:  //  0xCE //Read IR obstacles distance range
          break;
        case COMMAND_RANGE_RIGHT:  // Read IR obstacles distance range
          break;

          /* Arduino Write Commands->Don't require response from Bot'n Roll ONE
           * A+ */

        case COMMAND_ARDUINO_BUZ:  // buzzer
          sprintf(str,
                  "%c%c%c%c%c",
                  message.data[0],
                  message.data[1],
                  message.data[2],
                  message.data[3],
                  message.data[4]);
          tone(9, atoi(str), 250);
          delay(250);
          noTone(9);
          one.lcd2("Play Music");
          break;
        case COMMAND_ARDUINO_CMP:  // compass
        {
          float bearing;
          char roll, pitch;

          bearing = readBearing();
          roll = readRoll();
          pitch = readPitch();

          sprintf(str,
                  "CP%d.%d,%d,%d",
                  (int)bearing,
                  (int)((bearing - (int)bearing) * 100),
                  roll,
                  pitch);
          Serial.write(str);
          one.lcd2(str + 2);
        } break;
        case COMMAND_ARDUINO_SNR:  // compass
        {
          long distance;
          unsigned long duration;  // Duration used to calculate distance
          unsigned long tempo = micros();

          digitalWrite(TRIGGER_PIN, LOW);
          delayMicroseconds(2);

          digitalWrite(TRIGGER_PIN, HIGH);
          delayMicroseconds(10);

          digitalWrite(TRIGGER_PIN, LOW);
          duration = pulseIn(ECHO_PIN, HIGH, 11640);
          delayMicroseconds(
              16000 -
              (micros() -
               tempo));  // this routine has fixed time (16 milliseconds)

          // Calculate the distance (in cm) based on the speed of sound
          distance = (int)(duration / 58.2);
          if (distance >= MAX_RANGE || distance <= MIN_RANGE) distance = -1;

          sprintf(str, "SN%ld", distance);
          Serial.write(str);
          one.lcd2(str + 2);
        } break;
        case COMMAND_ARDUINO_GRP1:  // compass
          gripper1.write((unsigned char)message.data[0]);
          break;
        case COMMAND_ARDUINO_GRP2:  // compass
          gripper2.write((unsigned char)message.data[0]);
          break;
      }
    }
  }
}