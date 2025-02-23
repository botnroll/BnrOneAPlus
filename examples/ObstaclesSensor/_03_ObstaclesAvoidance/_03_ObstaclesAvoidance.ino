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
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
int left_speed = 0;
int right_speed = 0;
int speed = 10;        // Max speed
unsigned long tcycle;  // Control cycle time in loop
// Range Linear gain <> Ganho linear do obstáculo (2500)
float linear_gain = 2.5;
byte left_range;      // Left obstacle sensor range
byte right_range;     // Right obstacle sensor range
byte max_range = 19;  // Maximum range value for obstacle
byte min_range = 1;   // Minimum range value for obstacle

void setup() {
  one.spiConnect(SSPIN);            // start SPI communication module
  one.stop();                       // stop motors
  readMenuEEPROM();                 // Read configuration values from EEPROM
  one.setMinBatteryV(9.5);          // Set the protection battery voltage.
  one.setPid(1200, 400, 200);       // Set PID parameters for movement
  one.obstacleSensorsEmitters(ON);  // activate IR emitters
  one.lcd1("Avoid Obstacles ");
  one.lcd2("Press a Button!!");
  // Wait a button to be pressed <> Espera que pressione um botão
  while (one.readButton() == 0);
  // Wait for button release <> Espera que largue o botão
  while (one.readButton() != 0);

  one.lcd1(" RgL SpL RgR SpR");
  tcycle = millis();  // Set start value for tcycle
}

void loop() {
  if (millis() > tcycle) {
    tcycle += 25;      // Loop every 25ms
    readAndProcess();  // Read obstacles and decide how to move
    one.move(left_speed, right_speed);
    one.lcd2(left_range, left_speed, right_range, right_speed);

    // Configuration menu <> Menu de configuração
    if (one.readButton() == 3)
      menu();  // PB3 to enter menu <> PB3 para entrar no menu
  }
}

void readAndProcess() {
  left_range = one.readLeftRangeSensor();    // read left obstacle sensor range
  right_range = one.readRightRangeSensor();  // read right obstacle sensor range

  // if obstacle is near the left sensor - Upper limit range
  if (left_range >= max_range) {
    right_speed = -speed;
    left_speed = speed;
  }
  // if obstacle is near the right sensor - Upper limit range
  else if (right_range >= max_range) {
    right_speed = speed;
    left_speed = -speed;
  }
  // If obstacle is far away it is ignored - Lower limit range
  if ((left_range <= min_range) && (right_range <= min_range)) {
    right_speed = speed;
    left_speed = speed;
  }
  // If obstacle is in the active range
  else {
    // if obstacle is closer to the left sensor
    if (left_range > right_range) {
      left_speed = (speed + (int)((float)left_range / 2.0));
      right_speed = (speed - (int)((float)left_range * (float)linear_gain));
      if (right_speed < -speed) {
        right_speed = -speed;
      }
    }
    // if obstacle is closer to the right sensor
    else {
      right_speed = (speed + (int)((float)right_range / 2.0));
      left_speed = (speed - (int)((float)right_range * (float)linear_gain));
      if (left_speed < -speed) {
        left_speed = -speed;
      }
    }
  }
}

void menu() {
  int temp_var = 0;
  one.stop();
  one.lcd1("  Menu Config:  ");
  one.lcd2("PB1+ PB2- PB3ok");
  delay(500);
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) {
    delay(150);
  }
  //*****  Adjust sensor distance <> Ajustar a distância dos sensores  ******
  while (one.readButton() != 3) {
    readAndProcess();
    one.lcd1(" RgL SpL RgR SpR");
    one.lcd2(left_range, left_speed, right_range, right_speed);
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) one.lcd1("  Menu Config:  ");

  /****** Range mínimo para iniciar o desvio de obstáculos *****/
  var = (int)min_range;
  while (one.readButton() != 3) {
    one.lcd2("  min_range: ", var);
    if (one.readButton() == 1) {
      ++var;
      delay(150);
    }
    if (one.readButton() == 2) {
      --var;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) min_range = (byte)var;

  /****** Range máximo a partir do qual faz rotação ***********/
  var = (int)max_range;
  while (one.readButton() != 3) {
    one.lcd2("  max_range: ", var);
    if (one.readButton() == 1) {
      ++var;
      delay(150);
    }
    if (one.readButton() == 2) {
      --var;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) max_range = (byte)var;

  /****** Maximum speed <> velObst Maxima ******************/
  temp_var = speed;
  while (one.readButton() != 3) {
    one.lcd2(" velObstMax: ", temp_var);
    if (one.readButton() == 1) {
      ++var;
      delay(150);
    }
    if (one.readButton() == 2) {
      --var;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) speed = var;

  //**** Linear gain kLinear <> Ganho linear kLinear ****
  var = (int)(linear_gain * 1000.0);

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
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) linear_gain = (float)var / 1000.0;

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
  int var = 0;

  var = speed;
  EEPROM.write(eeprom_address, lowByte(var));  // Guardar em EEPROM
  ++eeprom_address;

  var = (int)(linear_gain * 1000.0);
  EEPROM.write(eeprom_address, highByte(var));  // Guardar em EEPROM
  ++eeprom_address;
  EEPROM.write(eeprom_address, lowByte(var));
  ++eeprom_address;

  var = min_range;
  EEPROM.write(eeprom_address, lowByte(var));  // Guardar em EEPROM
  ++eeprom_address;

  var = max_range;
  EEPROM.write(eeprom_address, lowByte(var));  // Guardar em EEPROM

  ++eeprom_address;
}

// Read Menu values from EEPROM <> Ler valores da EEPROM
void readMenuEEPROM() {
  byte eeprom_address = 30;
  int var = 0;

  speed = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;

  var = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  var = var << 8;
  var += (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  linear_gain = (float)var / 1000.0;

  min_range = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;

  max_range = (int)EEPROM.read(eeprom_address);

  ++eeprom_address;

  if (speed == 255) {
    speed = 10;
  }
  if (linear_gain < 0) {
    linear_gain = 2.5;
  }
  if (min_range == 255) {
    min_range = 1;
  }
  if (max_range == 255) {
    max_range = 19;
  }
}