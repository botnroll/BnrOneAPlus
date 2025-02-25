/**
 * This example was created by José Cruz (www.botnroll.com) on October 2016
 *  Updated on February 2025 by José Cruz
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * IMPORTANT!!!!
 * Before you use this example you MUST calibrate the line sensor. Use example
 * CalibrateLineSensor first!!!
 *
 * Line reading provides a linear value between -100 to 100
 * Line in the sensor varies from 0 to 9000
 * Reads the 8 sensors and stores the highest value sensor.
 * The nearest higher value sensor defines the line position between these two
 * sensors. Maximum and highest neighbour.
 *
 * <>
 * IMPORTANTE!!!!
 * Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar
 * exemplo CalibrateLineSensor antes deste!!!
 *
 * Leitura da linha que devolve um valor linear entre -100 e +100 da posição da
 * linha. A linha no sensor varia de 0 a 9000. Efetua a leitura dos 8 sensores e
 * regista o sensor com o valor máximo. O sensor vizinho com maior valor define
 * a posição da linha entre os dois sensores: Maximo e vizinho com maior valor.
 *
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>          // SPI communication library required by BnrOne.cpp
BnrOneAPlus
    one;  // declaration of object variable to control the Bot'n Roll ONE A

// constants definitions
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define M1 1  // Motor1
#define M2 2  // Motor2

#define MAX_VALUE 1000  // maximum value for a sensor reading
#define MIN_BATTERY_V 10.5  // safety voltage for discharging the battery

int max_value[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
int min_value[8] = {0, 0, 0, 0, 0, 0, 0, 0};
double correction_factor[8];
int bw_threshold = 50;  // Line follower limit between white and black

void setupLine() {
  // Read EEPROM values <> Ler valores da EEPROM
  byte eeprom_address = 100;
  Serial.println("Setup:");
  Serial.print("Max: ");
  for (int i = 0; i < 8; i++) {
    max_value[i] = (int)EEPROM.read(eeprom_address);
    max_value[i] = max_value[i] << 8;
    ++eeprom_address;
    max_value[i] += (int)EEPROM.read(eeprom_address);
    ++eeprom_address;
    Serial.print(max_value[i]);
    Serial.print("  ");
  }
  Serial.println();
  Serial.print("Min: ");
  for (int i = 0; i < 8; i++) {
    min_value[i] = (int)EEPROM.read(eeprom_address);
    min_value[i] = min_value[i] << 8;
    ++eeprom_address;
    min_value[i] += (int)EEPROM.read(eeprom_address);
    ++eeprom_address;
    Serial.print(min_value[i]);
    Serial.print("  ");
  }
  bw_threshold = (int)EEPROM.read(eeprom_address);
  bw_threshold = bw_threshold << 8;
  ++eeprom_address;
  bw_threshold += (int)EEPROM.read(eeprom_address);
  Serial.println();
  Serial.print("bw_threshold: ");
  Serial.print(bw_threshold);
  Serial.println();

  for (int i = 0; i < 8; i++) {
    // Calculate factor for each sensor <> Calcular fator de cada sensor
    correction_factor[i] =
        (double)MAX_VALUE / (double)(max_value[i] - min_value[i]);
  }
}

int readLine() {
  int virtual_sensor_1 = min_value[1] * 2;
  int virtual_sensor_2 = min_value[6] * 2;
  int sensor_value[8];
  int normalised_sensor_value[10] = {
      virtual_sensor_1, 0, 0, 0, 0, 0, 0, 0, 0, virtual_sensor_2};
  int id_max = -1, max_sensor = -1;
  int line_value = -1;
  int flag = -1;
  static int previous_line_value = 0;

  // Read the 8 sensor values <> Leitura dos valores dos 8 sensores
  for (int i = 0; i < 8; i++) {
    sensor_value[i] = one.readAdc(i);
  }

  // Normalize values between 0 and 1000 <> Normalizar valores entre 0 e 1000
  for (int i = 1; i < 9; i++) {
    // Registar o valor efetivo máximo de cada sensor
    normalised_sensor_value[i] =
        (int)((double)((sensor_value[i - 1] - min_value[i - 1])) *
              correction_factor[i - 1]);
    // Identify the sensor with the highest value <> Identifica o sensor com
    // valor efectivo máximo
    if (normalised_sensor_value[i] > max_sensor) {
      max_sensor = normalised_sensor_value[i];
      id_max = i;  // Store the sensor index <> Registar o indice do sensor
    }
  }
  // If previous is bigger than the next <> Se o anterior for maior que o
  // seguinte
  if (max_sensor > bw_threshold && normalised_sensor_value[id_max - 1] >=
                                       normalised_sensor_value[id_max + 1]) {
    line_value = MAX_VALUE * (id_max - 1) + normalised_sensor_value[id_max];
    flag = 0;
  }
  // If previous is smaller than the next <> Se o anterior for menor que o
  // seguinte
  else if (max_sensor > bw_threshold && normalised_sensor_value[id_max - 1] <
                                            normalised_sensor_value[id_max + 1])

  {
    if (id_max != 8)  // If not the last sensor <> Se não é o último sensor
    {
      line_value = MAX_VALUE * id_max + normalised_sensor_value[id_max + 1];
      flag = 1;
    } else  // If this is the last sensor <> Se é o último sensor
    {
      line_value =
          MAX_VALUE * id_max + MAX_VALUE - normalised_sensor_value[id_max];
      flag = 2;
    }
  }
  // Lost the line -> all white <> Saiu da linha -> tudo branco
  if (line_value == -1) {
    if (previous_line_value > 4500) {
      line_value = 9000;
    } else {
      line_value = 0;
    }
  }
  // Possible reading errors <> Possiveis erros de leitura
  else if (line_value < -1 || line_value > 9000) {
    line_value = previous_line_value;
  } else  // If normal values <> Se valores normais
  {
    previous_line_value = line_value;
  }

  // Values from -100 to 100 <> Valores de -100 a 100
  return (int)((double)(line_value + 1) * 0.022222) - 100;
}

void setup() {
  Serial.begin(115200);   // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.stop();             // stop motors
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