/**
 * This example was created by José Cruz on February 2025
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * IMPORTANT!!!!
 * Before you use this example you MUST calibrate the line sensor. Use example
 * CalibrateLineSensor first!!!
 *
 * Line reading provides a linear value between -100 to 100
 * Line follow:
 * Motors speed varies according to a cosine function of the line value.
 * Function Gain can be adjusted.
 * You can adjust the speed limit of the wheel that is outside the curve.
 * Press push button 3 (PB3) to enter control configuration menu.
 *
 * <>
 *
 * IMPORTANTE!!!!+
 * Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar
 * exemplo CalibrateLineSensor antes deste!!!
 *
 * Leitura da linha que devolve um valor linear entre -100 e +100 da posição da
 * linha. Seguimento de linha: Os motores variam com a linha de acordo com a
 * função coseno do valor da linha. O ganho da função pode ser ajustado. Ajuste
 * do limite de velocidade da roda que está no exterior da curva. Botão 3 (PB3)
 * entra no menu de configuração do controlo.
 *
 * https://www.desmos.com/calculator
 *
 */

#include <AngleConvertor.h>  // Angle conversion functions
#include <BnrOneAPlus.h>     // Bot'n Roll ONE A library
#include <EEPROM.h>          // EEPROM reading and writing
#include <SPI.h>             // SPI communication library required by BnrOne.cpp

BnrOneAPlus one;  // object to control the Bot'n Roll ONE A

// constants definitions
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define ML 1                    // Left Motor
#define MR 2                    // Right Motor
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

int g_speed = 55;  // Robot overall speed <> Velocidade do robô para seguir a
                   // linha
int g_left_speed = 0;
int g_right_speed = 0;

// g_line_gain behaviour -> Lower Gain, higher output
// Comportamento do g_line_gain -> Baixando o ganho aumenta a saida
float g_line_gain = 2.0;  // Line gain <> Ganho da linha

// g_wheel_boost_factor behaviour ->  Lower Gain, higher output
// Comportamento do g_wheel_boost_factor -> Baixando o ganho aumenta a saida

// Speed increase factor for outside wheel <>
// Fator para o incremento da velocidade da roda exterior
float g_wheel_boost_factor = 5.0;
int g_wheel_boost = 4;     // Outside wheel maximum speed on a curve
                           // Velocidade máxima da roda exterior numa curva
int g_min_speed_lim = -1;  // Inside wheel minimum speed on a curve
                           // Velocidade minima da roda interior numa curva

/****** Line Following with Cosine Function ************
******* Seguimento de linha com função coseno **********
********************************************************/
void lineFollowCosine() {
  // Read the line value from the line sensor <> Ler o valor da linha do sensor
  // de linha
  const int line = one.readLine();

  // Apply line gain to the line value <> Aplicar o ganho da linha ao valor da
  // linha
  float angle_degrees = (float)line * g_line_gain;

  // Angle cannnot exceed 180º <> O angulo não pode exceder 180º
  angle_degrees = constrain(angle_degrees, -180, 180);

  // Conversion factor from degree to radians <> Factor de conversão de graus
  // para radianos
  const float angle_radians = AngleConvertor::toRadians(angle_degrees);

  // If line is on the left side of the sensor <> Se a linha está do lado
  // esquerdo do sensor
  if (line <= 0) {
    // Inside wheel decreases speed according to cosine function
    // Roda interior diminui a velocidade de acordo com a função coseno
    g_left_speed = (int)(cos(angle_radians) * g_speed);

    // Outside wheel maintains or increases speed according to
    // g_wheel_boost_factor Roda exterior mantém ou aumenta a velocidade de
    // acordo com o factor g_wheel_boost_factor
    g_right_speed = g_speed + (int)(((float)(g_speed - g_left_speed)) /
                                    g_wheel_boost_factor);

    // Limit the outside wheel boost <> Limitar o aumento de velocidade da roda
    // exterior
    if (g_right_speed > g_speed + g_wheel_boost) {
      g_right_speed = g_speed + g_wheel_boost;
    }

    // Limit inside wheel minimum speed <> Limitar velocidade mínima da roda
    // interior
    if (g_left_speed < g_min_speed_lim) {
      g_left_speed = g_min_speed_lim;
    }
  }

  // If line is on the right side of the sensor <> Se a linha está do lado
  // direito do sensor
  else if (line > 0) {
    g_right_speed = (int)(cos(angle_radians) * g_speed);
    g_left_speed = g_speed + (int)(((float)(g_speed - g_right_speed)) /
                                   g_wheel_boost_factor);
    if (g_left_speed > g_speed + g_wheel_boost) {
      g_left_speed = g_speed + g_wheel_boost;
    }
    if (g_right_speed < g_min_speed_lim) {
      g_right_speed = g_min_speed_lim;
    }
  }
}

void waitButton3Push() {
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3);
  while (one.readButton() == 0);
  while (one.readButton() == 3);
}

int getUserInput(const char text[], const int value) {
  int temp_var = value;
  while (one.readButton() != 3) {
    one.lcd2(text, temp_var);
    delay(125);

    if (one.readButton() == 1) {
      ++temp_var;
    }
    if (one.readButton() == 2) {
      --temp_var;
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3);

  return temp_var;
}

void menu() {
  one.stop();
  one.lcd1("  Menu Config:");
  one.lcd2("PB1+ PB2-  PB3ok");
  waitButton3Push();

  //***** Overall speed <> Velocidade para seguir a linha ******
  g_speed = getUserInput("   speed:", g_speed);

  //**** Outside wheel maximum speed <> Velocidade maxima da roda exterior
  g_wheel_boost = getUserInput(" Wheel Boost:", g_wheel_boost);

  //**** Line gain <> Ganho da linha  ****
  auto temp_var = (int)(g_wheel_boost_factor * 1000.0);
  temp_var = getUserInput("BoostFactor:", temp_var);
  g_wheel_boost_factor = (float)temp_var / 1000.0;

  //**** Inside wheel minimum speed on a curve <> Velocidade minima da roda
  // interior numa curva
  g_min_speed_lim = getUserInput("g_min_speed_lim:", g_min_speed_lim);

  //**** Line gain <> Ganho da linha  ****
  temp_var = (int)(g_line_gain * 1000.0);
  temp_var = getUserInput("BoostFactor:", temp_var);
  g_line_gain = (float)temp_var / 1000.0;

  //**** Configuration end <> Termina Configuração *****
  writeMenuEEPROM();  // Write control values to EEPROM <> Escrever valores de
                      // controlo na EEPROM
  one.lcd1("Line  Following!");
  one.lcd2("www.botnroll.com");
  delay(250);
}

byte writeByteToEEPROM(const byte eeprom_address, const int temp_var) {
  EEPROM.write(eeprom_address, low_byte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;

  return eeprom_address;
}

byte writeIntToEEPROM(const byte eeprom_address, const int temp_var) {
  EEPROM.write(eeprom_address, high_byte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;
  EEPROM.write(eeprom_address, low_byte(temp_var));  // Guardar em EEPROM
  ++eeprom_address;

  return eeprom_address;
}

byte readByteFromEEPROM(const byte eeprom_address, int& temp_var) {
  temp_var = (int)EEPROM.read(eeprom_address);  // Guardar em EEPROM
  ++eeprom_address;

  return eeprom_address;
}

byte readIntFromEEPROM(const byte eeprom_address, int& temp_var) {
  temp_var = (int)EEPROM.read(eeprom_address);
  ++eeprom_address;
  temp_var = temp_var << 8;
  temp_var += (int)EEPROM.read(eeprom_address);
  ++eeprom_address;

  return eeprom_address;
}

// Write values on EEPROM <> Escrever valores na EEPROM
void writeMenuEEPROM() {
  byte eeprom_address = 20;
  eeprom_address = writeByteToEEPROM(eeprom_address, low_byte(g_speed));

  eeprom_address = writeByteToEEPROM(eeprom_address, low_byte(g_wheel_boost));

  int temp_var = (int)(g_line_gain * 1000.0);
  eeprom_address = writeIntToEEPROM(eeprom_address, temp_var);

  eeprom_address = writeIntToEEPROM(eeprom_address, g_min_speed_lim);

  temp_var = (int)(g_wheel_boost_factor * 1000.0);
  eeprom_address = writeIntToEEPROM(eeprom_address, temp_var);
}

// Test if value is withn limits <> Testa se o valor está dentro dos limites
template <typename T>
boolean isWithinLimits(const T valor, const T min, const T max) {
  if (valor > max) return false;
  if (valor < min) return false;
  return true;
}

// Read values from EEPROM <> Ler valores da EEPROM
void readMenuEEPROM() {
  byte eeprom_address = 20;

  eeprom_address = readByteFromEEPROM(eeprom_address, g_speed);

  eeprom_address = readByteFromEEPROM(eeprom_address, g_wheel_boost);

  eeprom_address = readIntFromEEPROM(eeprom_address, g_line_gain);
  g_line_gain = (float)g_line_gain / 1000.0;

  eeprom_address = readIntFromEEPROM(eeprom_address, g_min_speed_lim);

  eeprom_address = readIntFromEEPROM(eeprom_address, g_wheel_boost_factor);
  g_wheel_boost_factor = (float)g_wheel_boost_factor / 1000.0;

  if (!isWithinLimits<byte>(g_speed, 0, 100)) g_speed = 50;
  if (!isWithinLimits<byte>(g_wheel_boost, 0, 100)) g_wheel_boost = 4;
  if (!isWithinLimits<float>(g_line_gain, 0.0, 10.0)) g_line_gain = 2.0;
  if (!isWithinLimits<int>(g_min_speed_lim, -100, 100)) g_min_speed_lim = -2;
  if (!isWithinLimits<float>(g_wheel_boost_factor, 0.0, 20.0)) {
    g_wheel_boost_factor = 4.0;
  }
}

void loop() {
  // Call follow line function <> Chama a função de seguir linha
  lineFollowCosine();

  // Move motors <> Mover motores
  one.move(g_left_speed, g_right_speed);

  // Configuration menu <> Menu de configuração
  if (one.readButton() == 3)
    menu();  // PB3 to enter menu <> PB3 para entrar no menu
}

void setup() {
  Serial.begin(115200);   // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.stop();                             // stop motors
  one.lcd1("Line Follow COS ");
  one.lcd2(" Press a button ");
  if (one.readButton() == 0)  // Skip read EEPROM is necessary
    readMenuEEPROM();  // read control values from EEPROM <> Ler valores de
                       // controlo da EEPROM
  // Wait a button to be pressed <> Espera que pressione um botão
  while (one.readButton() == 0);
  // Wait for button release <> Espera que largue o botão
  while (one.readButton() != 0);
  delay(125);  // Debounce delay
  one.lcd2("www.botnroll.com");
}