/*
 This example was created by José Cruz (www.botnroll.com) on December 2024

 This code example is in the public domain.
 http://www.botnroll.com

Every sensor have a specific weigh:
  S1  S2  S3  S4  S5  S6  S7  S8
-100 -75 -50 -25 +25 +50 +75 +100
Verifies which sensors detect the line (black>Vtrans) and the result is the
average weigh of these sensors. Note: Only 1 or 2 sensors are expected to detect
the line at the same time. The 15 possible results for the line position are:
-100 -87 -75 -62 -50 -37 -25 0 +25 +37 +50 +62 +75 +87 +100
<>
Cada sensor tem um peso específico:
  S1  S2  S3  S4  S5  S6  S7  S8
-100 -75 -50 -25 +25 +50 +75 +100
Verifica quais os sensores que detectam a linha (preto > Vtrans) e o resusltado
é a média do peso destes sensores. Nota: É esperado que somente 1 ou 2 sensores
detetem a linha ao mesmo tempo. Os 15 resultados possíveis para a posição da
linha no sensor são: -100 -87 -75 -62 -50 -37 -25 0 +25 +37 +50 +62 +75 +87 +100
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus
    one;  // declaration of object variable to control the Bot'n Roll ONE A+

// constants definitions
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define M1 1     // Motor1
#define M2 2     // Motor2

#define Vtrans 300  // Line follower limit between white and black

void setup() {
  Serial.begin(57600);    // sets baud rate to 57600bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(10.5);  // safety voltage for discharging the battery
  one.lcd1(" Bot'n Roll ONE");
  one.lcd2("  Line Reading");
  delay(1000);
}

void loop() {
  int line = readLine();  // Read line <>  Ler a linha
  Serial.print("Line:");
  Serial.println(
      line);  // Print on Serial Monitor  <>  Imprime no Serial Monitor
  one.lcd2("  Line:", line);  // Print on LCD  <>  Imprime no LCD
  delay(50);  // Remove delay to follow the line  <>  Remover a pausa para
              // seguir a linha
}

int readLine() {
  int lineValue = 0;
  int sensorCount = 0;
  if (one.readAdc(0) > Vtrans)  // Test Sensor1
  {                             // Testa o sensor1
    lineValue -= 100;
    sensorCount++;
  }
  if (one.readAdc(1) > Vtrans)  // Test Sensor2
  {                             // Testa o sensor2
    lineValue -= 75;
    sensorCount++;
  }
  if (one.readAdc(2) > Vtrans) {
    lineValue -= 50;
    sensorCount++;
  }
  if (one.readAdc(3) > Vtrans) {
    lineValue -= 25;
    sensorCount++;
  }
  if (one.readAdc(4) > Vtrans) {
    lineValue += 25;
    sensorCount++;
  }
  if (one.readAdc(5) > Vtrans) {
    lineValue += 50;
    sensorCount++;
  }
  if (one.readAdc(6) > Vtrans) {
    lineValue += 75;
    sensorCount++;
  }
  if (one.readAdc(7) > Vtrans)  // Test Sensor8
  {                             // Testa o sensor8
    lineValue += 100;
    sensorCount++;
  }
  //      Serial.print("Val:");Serial.print(lineValue);
  //      Serial.print("  NrSen:");Serial.print(sensorCount);
  if (sensorCount > 2)
    lineValue = -1;
  else if (sensorCount > 0)
    lineValue = lineValue / sensorCount;
  return lineValue;
}
