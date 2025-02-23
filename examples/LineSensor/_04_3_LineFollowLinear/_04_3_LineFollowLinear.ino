/**
 *  This example was created by José Cruz on October 2016
 *  Updated on January 2019 by José Cruz
 *
 *  This code example is in the public domain.
 *  http://www.botnroll.com
 *
 * IMPORTANT!!!!
 * Before you use this example you MUST calibrate the line sensor. Use example
 * CalibrateLineSensor first!!!
 *
 * Line reading provides a linear value between -100 to 100
 * Line follow:
 * Motors speed varies according to a linear function.
 * Linear Gain must be adjusted.
 * You can adjust the speed limit of the wheel that is outside the curve.
 * Press push button 3 (PB3) to enter control configuration menu.
 *
 * <>
 *
 * IMPORTANTE!!!!
 * Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar
 * exemplo CalibrateLineSensor antes deste!!!
 *
 * Leitura da linha que devolve um valor linear entre -100 e +100 da posição da
 * linha. Seguimento de linha: Os motores variam com a linha de acordo com uma
 * função linear. Ajuste de ganho linear. Ajuste do limite de velocidade da roda
 * que está no exterior da curva. Botão 3 (PB3) entra no menu de configuração do
 * controlo.
 */

#include <BnrOneA.h>  // Bot'n Roll ONE A library
#include <EEPROM.h>   // EEPROM reading and writing
#include <SPI.h>      // SPI communication library required by BnrOne.cpp
BnrOneA one;  // declaration of object variable to control the Bot'n Roll ONE A

// constants definitions
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define M1 1                    // Motor1
#define M2 2                    // Motor2
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

// variables definitions
int speed = 60;
double linear_gain = 1.10;  // Linear gain <> Ganho linear
int extra_speed = 8;        // Curve outside wheel max speed limit <> Limite de
                            // velocidade da roda exterior na curva

void setup() {
  Serial.begin(115200);    // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  // safety voltage for discharging the battery
  one.setMinBatteryV(MINIMUM_BATTERY_V);
  one.stop();        // stop motors
  readMenuEEPROM();  // read control values from EEPROM <> Ler valores de
                     // controlo da EEPROM
  one.lcd1("Line Follow Lin.");
  one.lcd2(" Press a button ");
  // Wait a button to be pushed <> Espera que pressione um botão
  while (one.readButton() == 0);
  // Wait for button release <> Espera que largue o botão
  while (one.readButton() != 0);
}

void loop() {
  int line = one.readLine();
  // Linear function for Motor1 <> Função linear para o Motor1
  int velM1 = (int)((double)speed + ((double)line * linear_gain));
  // Linear function for Motor2 <> Função linear para o Motor2
  int velM2 = (int)((double)speed - ((double)line * linear_gain));

  // Limit motors maximum and minimum speed <> Limitar mínimos e máximos da
  // velocidade dos motores
  // Minimum speed -1 causes motor to brake <> Velocidade mínima -1 faz o motor
  // travar
  if (velM1 < -1) velM1 = -1;
  if (velM2 < -1) velM2 = -1;
  // Maximum speed limit <> Limite da velocidade máxima
  if (velM1 > speed + extra_speed) velM1 = speed + extra_speed;
  if (velM2 > speed + extra_speed) velM2 = speed + extra_speed;

  one.move(velM1, velM2);

  // Configuration menu <> Menu de configuração
  // PB3 to enter menu <> PB3 para entrar no menu
  if (one.readButton() == 3) menu();
}

void menu() {
  int temp_var = 0;
  static int button = 0;
  float temp = 0.0;
  one.stop();
  one.lcd1("  Menu Config:");
  one.lcd2("PB1+ PB2- PB3ok");
  delay(250);
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) {
    delay(150);
  }

  //***** Maximum speed <> Velocidade Maxima ******
  temp_var = speed;
  while (button != 3) {
    one.lcd2("   VelMax:", temp_var);
    button = one.readButton();
    if (button == 1) {
      ++temp_var;
      delay(150);
    }
    if (button == 2) {
      --temp_var;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (button == 3) {
    button = one.readButton();
  }
  speed = temp_var;

  //**** Outside wheel speed boost <> Incremento de velocidade da roda de fora
  //****
  temp_var = extra_speed;
  while (button != 3) {
    one.lcd2("  Curve Boost:", temp_var);
    button = one.readButton();
    if (button == 1) {
      ++temp_var;
      delay(150);
    }
    if (button == 2) {
      --temp_var;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (button == 3) {
    button = one.readButton();
  }
  extra_speed = temp_var;

  //**** Linear gain KLine <> Ganho linear KLine ****
  temp = linear_gain * 1000.0;
  temp_var = (int)temp;
  while (button != 3) {
    one.lcd2(" Line Gain:", temp_var);
    button = one.readButton();
    if (button == 1) {
      temp_var += 10;
      delay(150);
    }
    if (button == 2) {
      temp_var -= 10;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (button == 3) {
    button = one.readButton();
  }
  linear_gain = (float)temp_var / 1000.0;

  //**** Configuration end <> Termina Configuração *****
  writeMenuEEPROM();  // Write control values to EEPROM <> Escrever valores de
                      // controlo na EEPROM
  one.lcd1("Line  Following!");
  one.lcd2("www.botnroll.com");
  delay(250);
}

// Write Menu values on EEPROM <> Escrever valores na EEPROM
void writeMenuEEPROM() {
  byte eeprom_address = 10;
  int temp_var = 0;

  temp_var = speed;
  EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;
  temp_var = extra_speed;
  EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;
  temp_var = (int)(linear_gain * 1000.0);
  EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;
  EEPROM.write(eeprom_address, lowByte(temp_var));
  ++eeprom_address;
}

// Read Menu values from EEPROM <> Ler valores da EEPROM
void readMenuEEPROM() {
  byte eeprom_address = 10;
  int temp_var = 0;

  speed = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  extra_speed = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  temp_var = 0;
  temp_var = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  temp_var = temp_var << 8;
  temp_var += (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  linear_gain = (double)temp_var / 1000.0;
  if (speed == 255) speed = 60;
  if (extra_speed == 255) extra_speed = 3;
  if (linear_gain < 0) linear_gain = 1.3;
}
