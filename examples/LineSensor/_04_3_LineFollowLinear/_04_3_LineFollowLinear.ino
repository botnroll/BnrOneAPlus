/*
 This example was created by José Cruz on December 2024

 This code example is in the public domain.
 http://www.botnroll.com

IMPORTANT!!!!
Before you use this example you MUST calibrate the line sensor. Use example
_04_1_Calibrate.ino first!!!

Line reading provides a linear value between -100 to 100
Line follow:
Motors speed varies according to a linear function.
Linear Gain must be adjusted.
You can adjust the speed limit of the wheel that is outside the curve.
Press push button 3 (PB3) to enter control configuration menu.

<>

IMPORTANTE!!!!
Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar exemplo
_04_1_Calibrate.ino antes deste!!!

Leitura da linha que devolve um valor linear entre -100 e +100 da posição da
linha. Seguimento de linha: Os motores variam com a linha de acordo com uma
função linear. Ajuste de ganho linear. Ajuste do limite de velocidade da roda
que está no exterior da curva. Botão 3 (PB3) entra no menu de configuração do
controlo.
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

// variables definitions
float batmin = 10.5;  // safety voltage for discharging the battery
int vel = 60;
double kLine = 1.10;  // Linear gain <> Ganho linear
int vCurve = 8;  // Curve outside wheel max speed limit <> Limite de velocidade
                 // da roda exterior na curva

void setup() {
  Serial.begin(57600);    // sets baud rate to 57600bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.setMinBatteryV(batmin);  // safety voltage for discharging the battery
  one.stop();                  // stop motors
  readMenuEEPROM();  // read control values from EEPROM <> Ler valores de
                     // controlo da EEPROM
  one.lcd1("Line Follow Lin.");
  one.lcd2(" Press a button ");
  while (one.readButton() ==
         0);  // Wait a button to be pressed <> Espera que pressione um botão
  while (one.readButton() !=
         0);  // Wait for button release <> Espera que largue o botão
}

void loop() {
  int line = one.readLine();

  int velM1 = (int)((double)vel +
                    ((double)line * kLine));  // Linear function for Motor1 <>
                                              // Função linear para o Motor1
  int velM2 = (int)((double)vel -
                    ((double)line * kLine));  // Linear function for Motor2 <>
                                              // Função linear para o Motor2

  // Limit motors maximum and minimum speed <> Limitar mínimos e máximos da
  // velocidade dos motores
  if (velM1 < -1)
    velM1 = -1;  // Minimum speed -1 causes motor to brake <> Velocidade mínima
                 // -1 faz o motor travar
  if (velM2 < -1) velM2 = -1;
  if (velM1 > vel + vCurve)
    velM1 = vel + vCurve;  // Maximum speed limit <> Limite da velocidade máxima
  if (velM2 > vel + vCurve) velM2 = vel + vCurve;

  // Serial.print("   M1:"); Serial.print(velM1); Serial.print("   M2:");
  // Serial.println(velM2);
  one.move(velM1, velM2);

  // Configuration menu <> Menu de configuração
  if (one.readButton() == 3)
    menu();  // PB3 to enter menu <> PB3 para entrar no menu
}

void menu() {
  int var = 0;
  static int butt = 0;
  float temp = 0.0;
  one.stop();
  one.lcd1("  Menu Config:");
  one.lcd2("PB1+ PB2- PB3ok");
  delay(250);
  while (one.readButton() ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    delay(150);
  }

  //***** Maximum speed <> Velocidade Maxima ******
  var = vel;
  while (butt != 3) {
    one.lcd2("   VelMax:", var);
    butt = one.readButton();
    if (butt == 1) {
      var++;
      delay(150);
    }
    if (butt == 2) {
      var--;
      delay(150);
    }
  }
  while (butt ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    butt = one.readButton();
  }
  vel = var;

  //**** Outside wheel speed boost <> Incremento de velocidade da roda de fora
  //****
  var = vCurve;
  while (butt != 3) {
    one.lcd2("  Curve Boost:", var);
    butt = one.readButton();
    if (butt == 1) {
      var++;
      delay(150);
    }
    if (butt == 2) {
      var--;
      delay(150);
    }
  }
  while (butt ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    butt = one.readButton();
  }
  vCurve = var;

  //**** Linear gain KLine <> Ganho linear KLine ****
  temp = kLine * 1000.0;
  var = (int)temp;
  while (butt != 3) {
    one.lcd2(" Line Gain:", var);
    butt = one.readButton();
    if (butt == 1) {
      var += 10;
      delay(150);
    }
    if (butt == 2) {
      var -= 10;
      delay(150);
    }
  }
  while (butt ==
         3)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    butt = one.readButton();
  }
  kLine = (float)var / 1000.0;

  //**** Configuration end <> Termina Configuração *****
  writeMenuEEPROM();  // Write control values to EEPROM <> Escrever valores de
                      // controlo na EEPROM
  one.lcd1("Line  Following!");
  one.lcd2("www.botnroll.com");
  delay(250);
}

// Write Menu values on EEPROM <> Escrever valores na EEPROM
void writeMenuEEPROM() {
  byte eepromADD = 10;
  int var = 0;

  var = vel;
  EEPROM.write(eepromADD, lowByte(var));  // Guardar em EEPROM
  eepromADD++;
  var = vCurve;
  EEPROM.write(eepromADD, lowByte(var));  // Guardar em EEPROM
  eepromADD++;
  var = (int)(kLine * 1000.0);
  EEPROM.write(eepromADD, highByte(var));  // Guardar em EEPROM
  eepromADD++;
  EEPROM.write(eepromADD, lowByte(var));
  eepromADD++;
}

// Read Menu values from EEPROM <> Ler valores da EEPROM
void readMenuEEPROM() {
  byte eepromADD = 10;
  int var = 0;

  vel = (int)EEPROM.read(eepromADD);
  eepromADD++;
  vCurve = (int)EEPROM.read(eepromADD);
  eepromADD++;
  var = 0;
  var = (int)EEPROM.read(eepromADD);
  eepromADD++;
  var = var << 8;
  var += (int)EEPROM.read(eepromADD);
  eepromADD++;
  kLine = (double)var / 1000.0;
  if (vel == 255) vel = 60;
  if (vCurve == 255) vCurve = 3;
  if (kLine < 0) kLine = 1.3;
}
