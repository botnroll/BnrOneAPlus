/**
 * This example was created by José Cruz on December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Line Following:
 * 15 possible values for line position:-100 -87 -75 -62 -50 -37 -25 0 +25 +37
 * +50 +62 +75 +87 +100 The speed of the motors is adjusted using a linear
 * approach and gain (k)
 * <>
 * Seguimento de linha:
 * 15 valores possiveis para a posiçao da linha: -100 -87 -75 -62 -50 -37 -25 0
 * +25 +37 +50 +62 +75 +87 +100 A velocidade dos motores é ajustada numa
 * abordagem linear (reta) e ganho (k = declive da reta)
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definitions
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define M1 1     // Motor1
#define M2 2     // Motor2

#define BW_THRESHOLD 300    // Line follower limit between white and black
#define MIN_BATTERY_V 10.5  // Safety voltage for discharging the battery
#define SPEED 40            // Default speed

int readLine() {
  int lineValue = 0;
  int sensorCount = 0;
  if (one.readAdc(0) > BW_THRESHOLD)  // Test Sensor1  <>  Testa o sensor1
  {
    lineValue -= 100;
    ++sensorCount;
  }
  if (one.readAdc(1) > BW_THRESHOLD)  // Test Sensor2  <>  Testa o sensor2
  {
    lineValue -= 75;
    ++sensorCount;
  }
  if (one.readAdc(2) > BW_THRESHOLD) {
    lineValue -= 50;
    ++sensorCount;
  }
  if (one.readAdc(3) > BW_THRESHOLD) {
    lineValue -= 25;
    ++sensorCount;
  }
  if (one.readAdc(4) > BW_THRESHOLD) {
    lineValue += 25;
    ++sensorCount;
  }
  if (one.readAdc(5) > BW_THRESHOLD) {
    lineValue += 50;
    ++sensorCount;
  }
  if (one.readAdc(6) > BW_THRESHOLD) {
    lineValue += 75;
    ++sensorCount;
  }
  if (one.readAdc(7) > BW_THRESHOLD)  // Test Sensor8  <>  Testa o sensor8
  {
    lineValue += 100;
    ++sensorCount;
  }

  if (sensorCount > 2)
    lineValue = -1;
  else if (sensorCount > 0)
    lineValue = lineValue / sensorCount;

  return lineValue;
}

void setup() {
  Serial.begin(115200);    // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.stop();             // stop motors
  // safety voltage for discharging the battery
  one.setMinBatteryV(MIN_BATTERY_V);
  delay(1000);
}

void loop() {
  int line = readLine();

  double k = 0.45;  // Linear gain to control motor speed <> Ganho linear para
                    // controlar a velocidade dos motores

  double m1_speed = (double)SPEED + ((double)line * k);
  double m2_speed = (double)SPEED - ((double)line * k);

  one.move((int)m1_speed, (int)m2_speed);
}
