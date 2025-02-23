/*
 This example was created by José Cruz (www.botnroll.com)
 on the 10th February 2025

 This code example is in the public domain.
 http://www.botnroll.com

Description:
Read obstacle sensors range distance for left and right IR sensors.
Range varies from 0 to 20:
 -> 0 means no obstacle is detected
 -> 20 means obstacle is very close to the sensor
The robot has new readings every 25ms (40 readings per second)
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus
    one;  // declaration of object variable to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2          // Slave Select (SS) pin for SPI communication
int motL = 0, motR = 0;  // Motor speed
int speed = 10;          // Max speed
unsigned long tcycle;    // Control cycle time in loop
float rangeGain = 2.5;  // Range Linear gain <> Ganho linear do obstáculo (2500)
byte rangeL;            // Left obstacle sensor range
byte rangeR;            // Right obstacle sensor range
byte maxRange = 19;     // Maximum range value for obstacle
byte minRange = 1;      // Minimum range value for obstacle

void setup() {
  one.spiConnect(SSPIN);            // start SPI communication module
  one.stop();                       // stop motors
  readMenuEEPROM();                 // Read configuration values from EEPROM
  one.setMinBatteryV(9.5);          // Set the protection battery voltage.
  one.setPid(1200, 400, 200);       // Set PID parameters for movement
  one.obstacleSensorsEmitters(ON);  // activate IR emitters
  one.lcd1("Avoid Obstacles ");
  one.lcd2("Press a Button!!");
  while (one.readButton() ==
         0);  // Wait a button to be pressed <> Espera que pressione um botão
  while (one.readButton() !=
         0);  // Wait for button release <> Espera que largue o botão
  one.lcd1(" RgL SpL RgR SpR");
  tcycle = millis();  // Set start value for tcycle
}

void loop() {
  if (millis() > tcycle) {
    tcycle += 25;      // Loop every 25ms
    readAndProcess();  // Read obstacles and decide how to move
    one.move(motL, motR);
    one.lcd2(rangeL, motL, rangeR, motR);

    // Configuration menu <> Menu de configuração
    if (one.readButton() == 3)
      menu();  // PB3 to enter menu <> PB3 para entrar no menu
  }
}

void readAndProcess() {
  rangeL = one.readLeftRangeSensor();   // read left obstacle sensor range
  rangeR = one.readRightRangeSensor();  // read right obstacle sensor range

  // if obstacle is near the left sensor - Upper limit range
  if (rangeL >= maxRange) {
    motR = -speed;
    motL = speed;
    //      delay(rotTime);
  }
  // if obstacle is near the right sensor - Upper limit range
  else if (rangeR >= maxRange) {
    motR = speed;
    motL = -speed;
    //      delay(rotTime);
  }
  // If obstacle is far away it is ignored - Lower limit range
  if (rangeL <= minRange && rangeR <= minRange) {
    motR = speed;
    motL = speed;
  }
  // If obstacle is in the active range
  else {
    // if obstacle is closer to the left sensor
    if (rangeL > rangeR) {
      // motL=speed;
      motL = (speed + (int)((float)rangeL / 2.0));
      motR = (speed - (int)((float)rangeL * (float)rangeGain));
      if (motR < -speed) motR = -speed;
    }
    // if obstacle is closer to the right sensor
    else {
      // motR=speed;
      motR = (speed + (int)((float)rangeR / 2.0));
      motL = (speed - (int)((float)rangeR * (float)rangeGain));
      if (motL < -speed) motL = -speed;
    }
  }
}

void menu() {
  int temp_var = 0;
  one.stop();
  one.lcd1("  Menu Config:  ");
  one.lcd2("PB1+ PB2- PB3ok");
  delay(500);
  while (one.readButton() ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    delay(150);
  }
  //*****  Adjust sensor distance <> Ajustar a distância dos sensores  ******
  while (one.readButton() != 3) {
    readAndProcess();
    one.lcd1(" RgL SpL RgR SpR");
    one.lcd2(rangeL, motL, rangeR, motR);
  }
  while (one.readButton() ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
    one.lcd1("  Menu Config:  ");

  /****** Range mínimo para iniciar o desvio de obstáculos *****/
  temp_var = (int)minRange;
  while (one.readButton() != 3) {
    one.lcd2("  minRange: ", temp_var);
    if (one.readButton() == 1) {
      ++temp_var;
      delay(150);
    }
    if (one.readButton() == 2) {
      --temp_var;
      delay(150);
    }
  }
  while (one.readButton() ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
    minRange = (byte)temp_var;

  /****** Range máximo a partir do qual faz rotação ***********/
  temp_var = (int)maxRange;
  while (one.readButton() != 3) {
    one.lcd2("  maxRange: ", temp_var);
    if (one.readButton() == 1) {
      ++temp_var;
      delay(150);
    }
    if (one.readButton() == 2) {
      --temp_var;
      delay(150);
    }
  }
  while (one.readButton() ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
    maxRange = (byte)temp_var;

  /****** Maximum speed <> velObst Maxima ******************/
  temp_var = speed;
  while (one.readButton() != 3) {
    one.lcd2(" velObstMax: ", temp_var);
    if (one.readButton() == 1) {
      ++temp_var;
      delay(150);
    }
    if (one.readButton() == 2) {
      --temp_var;
      delay(150);
    }
  }
  while (one.readButton() ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
    speed = temp_var;

  //**** Linear gain kLinear <> Ganho linear kLinear ****
  temp_var = (int)(rangeGain * 1000.0);
  while (one.readButton() != 3) {
    one.lcd2(" DistGain: ", temp_var);
    if (one.readButton() == 1) {
      temp_var += 50;
      delay(150);
    }
    if (one.readButton() == 2) {
      temp_var -= 50;
      delay(150);
    }
  }
  while (one.readButton() ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
    rangeGain = (float)temp_var / 1000.0;

  /**** Configuration end <> Termina Configuração *****/
  writeMenuEEPROM();  // Write control values to EEPROM <> Escrever valores de
                      // controlo na EEPROM
  one.lcd1(" EEPROM WRITE...");
  delay(250);
  one.lcd2("   Written!     ");
  delay(1500);
  one.lcd1("RgL SpL RgR SpR");
  tcycle = millis();  // Set start value for tcycle
}

// Write Menu values on EEPROM <> Escrever valores na EEPROM
void writeMenuEEPROM() {
  byte eeprom_address = 30;
  int temp_var = 0;

  temp_var = speed;
  EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;

  temp_var = (int)(rangeGain * 1000.0);
  EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;
  EEPROM.write(eeprom_address, lowByte(temp_var));
  ++eeprom_address;

  temp_var = minRange;
  EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;

  temp_var = maxRange;
  EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;
}

// Read Menu values from EEPROM <> Ler valores da EEPROM
void readMenuEEPROM() {
  byte eeprom_address = 30;
  int temp_var = 0;

  speed = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;

  temp_var = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  temp_var = temp_var << 8;
  temp_var += (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  rangeGain = (float)temp_var / 1000.0;

  minRange = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;

  maxRange = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;

  if (speed == 255) speed = 10;
  if (rangeGain < 0) rangeGain = 2.5;
  if (minRange == 255) minRange = 1;
  if (maxRange == 255) maxRange = 19;
}