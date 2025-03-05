/**
 * This example was created by José Cruz on December 2024
 *  Updated on February 2025 by José Cruz
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Line Following:
 * Test which sensor detects the line by comparing sensor values.
 * The motors speed is set for each sensor detecting the line.
 * <>
 * Seguimento de linha:
 * Testa qual o sensor que deteta a linha comparando os valores dos sensores.
 * A velocidade dos motores é definida para cada uma das 8 posições da linha no
 * sensor.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// Constants definitions
// Definir constantes
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define M1 1  // Motor1
#define M2 2  // Motor2
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

// Transition value between white and black
// Valor de transição entre branco e preto
#define BW_THRESHOLD 300

// Speed for the robot movement
// Velocidade do robô
int speed = 40;

void setup() {
  Serial.begin(115200);   // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection

  one.lcd1(" Bot'n Roll ONE");
  one.lcd2("Press a button!");
  // Wait for a button to be pressed to move motors
  // Espera pressionar um botão para mover motores
  while (one.readButton() == 0)
    ;
  one.lcd2("Line Following!");
}

void loop() {
  // Read the 8 sensors
  // Ler os 8 sensores
  int sensor0 = one.readAdc(0);
  int sensor1 = one.readAdc(1);
  int sensor2 = one.readAdc(2);
  int sensor3 = one.readAdc(3);
  int sensor4 = one.readAdc(4);
  int sensor5 = one.readAdc(5);
  int sensor6 = one.readAdc(6);
  int sensor7 = one.readAdc(7);

  // From left to centre
  // Lado esquerdo do exterior para o centro
  if (sensor0 > BW_THRESHOLD)  // 10000000
  {
    one.move(-7, 40);
  } else if (sensor1 > BW_THRESHOLD)  // 01000000
  {
    one.move(5, 40);
  } else if (sensor2 > BW_THRESHOLD)  // 00100000
  {
    one.move(20, 40);
  } else if (sensor3 > BW_THRESHOLD)  // 00010000
  {
    one.move(speed, speed);
  }
  // From right to centre
  // Lado direito do exterior para o centro
  else if (sensor7 > BW_THRESHOLD)  // 00000001
  {
    one.move(40, -7);
  } else if (sensor6 > BW_THRESHOLD)  // 00000010
  {
    one.move(40, 5);
  } else if (sensor5 > BW_THRESHOLD)  // 00000100
  {
    one.move(40, 20);
  } else if (sensor4 > BW_THRESHOLD)  // 00001000
  {
    one.move(speed, speed);
  }
}
