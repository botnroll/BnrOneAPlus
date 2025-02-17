/*
 This example was created by José Cruz (www.botnroll.com) on Deceember 2024

 This code example is in the public domain.
 http://www.botnroll.com

IMPORTANT!!!!
Before you use this example you MUST calibrate the line sensor. Use example
_04_1_Calibrate.ino first!!!

Line reading provides a linear value between -100 to 100
Line in the sensor varies from 0 to 9000
Reads the 8 sensors and stores the highest value sensor.
The nearest higher value sensor defines the line position between these two
sensors. Maximum and highest neighbour.

<>
IMPORTANTE!!!!
Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar exemplo
_04_1_Calibrate.ino antes deste!!!

Leitura da linha que devolve um valor linear entre -100 e +100 da posição da
linha. A linha no sensor varia de 0 a 9000. Efetua a leitura dos 8 sensores e
regista o sensor com o valor máximo. O sensor vizinho com maior valor define a
posição da linha entre os dois sensores: Maximo e vizinho com maior valor.

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

#define VMAX 1000
int SValMax[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
int SValMin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
double SFact[8];
int Vtrans = 50;      // Line follower limit between white and black
float batmin = 10.5;  // safety voltage for discharging the battery

void setup() {
  Serial.begin(57600);  // sets baud rate to 57600bps for printing values at
                        // serial monitor.
  one.spiConnect(SSPIN);       // starts the SPI communication module
  one.stop();                  // stop motors
  one.setMinBatteryV(batmin);  // safety voltage for discharging the battery
  setupLine();  // read line calibrate values from EEPROM <> Ler valores de
                // calibração da linha da EEPROM
}

void loop() {
  int line = readLine();  // read line value <> ler valor da linha (-100 to
                          // +100)
  Serial.print("Line: ");
  Serial.println(line);
  one.lcd2("   Line: ", line);
  delay(50);
}

void setupLine() {
  // Read EEPROM values <> Ler valores da EEPROM
  byte eepromADD = 100;
  Serial.println("Setup:");
  Serial.print("Max: ");
  for (int i = 0; i < 8; i++) {
    SValMax[i] = (int)EEPROM.read(eepromADD);
    SValMax[i] = SValMax[i] << 8;
    eepromADD++;
    SValMax[i] += (int)EEPROM.read(eepromADD);
    eepromADD++;
    Serial.print(SValMax[i]);
    Serial.print("  ");
  }
  Serial.println();
  Serial.print("Min: ");
  for (int i = 0; i < 8; i++) {
    SValMin[i] = (int)EEPROM.read(eepromADD);
    SValMin[i] = SValMin[i] << 8;
    eepromADD++;
    SValMin[i] += (int)EEPROM.read(eepromADD);
    eepromADD++;
    Serial.print(SValMin[i]);
    Serial.print("  ");
  }
  Vtrans = (int)EEPROM.read(eepromADD);
  Vtrans = Vtrans << 8;
  eepromADD++;
  Vtrans += (int)EEPROM.read(eepromADD);
  Serial.println();
  Serial.print("Vtrans: ");
  Serial.print(Vtrans);
  Serial.println();

  for (int i = 0; i < 8; i++) {
    SFact[i] =
        (double)VMAX /
        (double)(SValMax[i] - SValMin[i]);  // Calculate factor for each sensor
                                            // <> Calcular fator de cada sensor
  }
}

int readLine() {
  int Vrt1 = SValMin[1] * 2, Vrt2 = SValMin[6] * 2;
  int SValR[8];
  int SValN[10] = {Vrt1, 0, 0, 0, 0, 0, 0, 0, 0, Vrt2};
  int idMax = -1, SMax = -1;
  int lineValue = -1;
  int flag = -1;
  static int prevLineValue = 0;

  // Read the 8 sensor values <> Leitura dos valores dos 8 sensores
  for (int i = 0; i < 8; i++) {
    SValR[i] = one.readAdc(i);
  }

  // Normalize values between 0 and 1000 <> Normalizar valores entre 0 e 1000
  for (int i = 1; i < 9; i++) {
    SValN[i] =
        (int)((double)((SValR[i - 1] - SValMin[i - 1])) *
              SFact[i - 1]);  // Registar o valor efetivo máximo de cada sensor
    if (SValN[i] > SMax) {
      SMax = SValN[i];  // Identify the sensor with the highest value <>
                        // Identificar o sensor com valor efectivo máximo
      idMax = i;  // Store the sensor index <> Registar o indice do sensor
    }
  }

  if (SMax > Vtrans &&
      SValN[idMax - 1] >=
          SValN[idMax + 1])  // If previous is bigger than the next <> Se o
                             // anterior for maior que o seguinte
  {
    lineValue = VMAX * (idMax - 1) + SValN[idMax];
    flag = 0;
  } else if (SMax > Vtrans &&
             SValN[idMax - 1] <
                 SValN[idMax + 1])  // If previous is smaller than the next <>
                                    // Se o anterior for menor que o seguinte
  {
    if (idMax != 8)  // If not the last sensor <> Se não é o último sensor
    {
      lineValue = VMAX * idMax + SValN[idMax + 1];
      flag = 1;
    } else  // If this is the last sensor <> Se é o último sensor
    {
      lineValue = VMAX * idMax + VMAX - SValN[idMax];
      flag = 2;
    }
  }
  if (lineValue ==
      -1)  // Lost the line -> all white <> Saiu da linha -> tudo branco
  {
    if (prevLineValue > 4500) {
      lineValue = 9000;
    } else {
      lineValue = 0;
    }
  } else if (lineValue < -1 ||
             lineValue >
                 9000)  // Possible reading errors <> Possiveis erros de leitura
  {
    lineValue = prevLineValue;
  } else  // If normal values <> Se valores normais
  {
    prevLineValue = lineValue;
  }
  // return lineValue;  //Values from 0 to 9000 <> Valores de 0 a 9000
  return (int)((double)(lineValue + 1) * 0.022222) -
         100;  // Values from -100 to 100 <> Valores de -100 a 100
}
