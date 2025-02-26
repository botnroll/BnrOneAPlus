/**
 * This example was created by José Cruz (www.botnroll.com) on December 2024
 *  Updated on February 2025 by José Cruz
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 */

#include <BnrOneAPlus.h>
#include <SPI.h>  // required by BnrOneAPlus.cpp

// Constants definitions
// Definir constantes
#define SSPIN 2   // Slave Select (SS) pin for SPI communication
#define M1 1      // Motor1
#define M2 2      // Motor2
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

void setup() {
  // setup routines
  Serial.begin(115200);  // set baud rate to 115200bps for printing values in
                         // serial monitor. Press (ctrl+shift+m) after uploading
  one.spiConnect(SSPIN);  // start the SPI module
  one.stop();             // stop motors
  one.lcd1(" Bot'n Roll ONE");
  one.lcd2(" Sensor Reading");
  delay(1000);
}

void loop() {
  // Read values from the 8 sensors
  // Ler o valor dos 8 sensores
  int sensor0 = one.readAdc(0);
  int sensor1 = one.readAdc(1);
  int sensor2 = one.readAdc(2);
  int sensor3 = one.readAdc(3);
  int sensor4 = one.readAdc(4);
  int sensor5 = one.readAdc(5);
  int sensor6 = one.readAdc(6);
  int sensor7 = one.readAdc(7);

  // Print values on LCD
  // Imprimir valores no LCD
  one.lcd1(sensor0, sensor1, sensor2, sensor3);
  one.lcd2(sensor4, sensor5, sensor6, sensor7);

  // Print values on Serial Monitor
  // Imprimir valores no Serial Monitor
  Serial.print(sensor0);
  Serial.print(" ");
  Serial.print(sensor1);
  Serial.print(" ");
  Serial.print(sensor2);
  Serial.print(" ");
  Serial.print(sensor3);
  Serial.print(" ");
  Serial.print(sensor4);
  Serial.print(" ");
  Serial.print(sensor5);
  Serial.print(" ");
  Serial.print(sensor6);
  Serial.print(" ");
  Serial.print(sensor7);
  Serial.print(" ");
  Serial.println();

  // This delay must be removed when the robot follows the line
  // Esta pausa deve ser removida quando o robô seguir a linha
  delay(50);
}
