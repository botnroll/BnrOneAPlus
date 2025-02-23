/**
 *  This example was created by José Cruz on October 2016
 *  Updated on January 2019 by José Cruz
 *
 *  This code example is in the public domain.
 *  http://www.botnroll.com
 *
 * IMPORTANT!!!!
 * Before you use this example you MUST calibrate the line sensor. Use example
 * CalibrateLineSensor first!!! Line reading provides a linear value between
 * -100 to 100
 *
 * Line follow:
 * Motors speed varies using PID control.
 * Adjustable gains kp, ki and kd.
 * You can adjust the speed limit of the wheel that is outside the curve.
 * Press push button 3 (PB3) to enter control configuration menu.
 *
 * <>
 *
 * IMPORTANTE!!!!
 * Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar
 * exemplo CalibrateLineSensor antes deste!!! Leitura da linha que devolve um
 * valor linear entre -100 e +100 da posição da linha.
 *
 * Seguimento de linha:
 * Os motores variam com a linha com controlo PID
 * Ajuste dos ganhos kp, ki, e kd.
 * Ajuste do limite de velocidade da roda que está no exterior da curva.
 *
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
#define LIMITS 100.0            // line value limit

// PID control gains <> Ganhos do controlo PID
double kp = 1.3;
double ki = 0.0013;
double kd = 0.35;
int vel = 60;         // Max Speed <> Velocidade Máxima dos motores
int extra_speed = 3;  // Curve outside wheel max speed limit <> Limite de
                      // velocidade da roda exterior na curva

void menu() {
  int temp_var = 0;
  int butt = 0;
  double temp = 0.0;
  one.stop();
  one.lcd1("  Menu Config:");
  one.lcd2("PB1+ PB2- PB3ok");
  delay(150);
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3) {
    delay(150);
  }

  //***** Maximum speed <> Velocidade Maxima ******
  temp_var = vel;
  while (butt != 3) {
    one.lcd2("   VelMax:", temp_var);
    butt = one.readButton();
    if (butt == 1) {
      temp_var++;
      delay(150);
    }
    if (butt == 2) {
      temp_var--;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (butt != 0) {
    butt = one.readButton();
  }
  vel = temp_var;

  //**** Outside wheel speed boost <> Incremento de velocidade da roda de fora
  //****
  temp_var = extra_speed;
  while (butt != 3) {
    one.lcd2("  Curve Boost:", temp_var);
    butt = one.readButton();
    if (butt == 1) {
      temp_var++;
      delay(150);
    }
    if (butt == 2) {
      temp_var--;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (butt != 0) {
    butt = one.readButton();
  }
  extra_speed = temp_var;

  //**** Proportional gain kp <> Ganho proporcional kp ****
  temp = kp * 1000;
  temp_var = (int)temp;
  while (butt != 3) {
    one.lcd2("    kp:", temp_var);
    butt = one.readButton();
    if (butt == 1) {
      temp_var += 10;
      delay(150);
    }
    if (butt == 2) {
      temp_var -= 10;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (butt != 0) {
    butt = one.readButton();
  }
  kp = (double)temp_var / 1000;

  //**** Integral gain ki <> Ganho integral ki ****
  temp = ki * 10000.0;
  temp_var = (int)temp;
  while (butt != 3) {
    one.lcd2("     ki:", temp_var);
    butt = one.readButton();
    if (butt == 1) {
      temp_var += 1;
      delay(150);
    }
    if (butt == 2) {
      temp_var -= 1;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (butt != 0) {
    butt = one.readButton();
  }
  ki = (double)temp_var / 10000.0;

  //**** Differential gain kd <> Ganho diferencial kd ****
  temp = kd * 1000;
  temp_var = (int)temp;
  while (butt != 3) {
    one.lcd2("     kd:", temp_var);
    butt = one.readButton();
    if (butt == 1) {
      temp_var += 10;
      delay(150);
    }
    if (butt == 2) {
      temp_var -= 10;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (butt != 0) {
    butt = one.readButton();
  }
  kd = (double)temp_var / 1000;

  //**** Configuration end <> Termina Configuração *****
  writeMenuEEPROM();  // Write control values to EEPROM <> Escrever valores de
                      // controlo na EEPROM
  one.lcd1("Line  Following!");
  one.lcd2("www.botnroll.com");
  delay(250);
}

// Write values on EEPROM <> Escrever valores na EEPROM
void writeMenuEEPROM() {
  byte eeprom_address = 10;
  int temp_var = 0;

  temp_var = vel;
  EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
  eeprom_address++;
  temp_var = extra_speed;
  EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
  eeprom_address++;
  temp_var = (int)(kp * 1000.0);
  EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
  eeprom_address++;
  EEPROM.write(eeprom_address, lowByte(temp_var));
  eeprom_address++;
  temp_var = (int)(ki * 10000.0);
  EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
  eeprom_address++;
  EEPROM.write(eeprom_address, lowByte(temp_var));
  eeprom_address++;
  temp_var = (int)(kd * 1000.0);
  EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
  eeprom_address++;
  EEPROM.write(eeprom_address, lowByte(temp_var));
  eeprom_address++;
}

// Read values from EEPROM <> Ler valores da EEPROM
void readMenuEEPROM() {
  byte eeprom_address = 10;
  int temp_var = 0;

  vel = (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  extra_speed = (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  temp_var = 0;
  temp_var = (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  temp_var = temp_var << 8;
  temp_var += (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  kp = (double)temp_var / 1000.0;
  temp_var = 0;
  temp_var = (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  temp_var = temp_var << 8;
  temp_var += (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  ki = (double)temp_var / 10000.0;
  temp_var = 0;
  temp_var = (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  temp_var = temp_var << 8;
  temp_var += (int)EEPROM.read(eeprom_address);
  eeprom_address++;
  kd = (double)temp_var / 1000.0;
  if (vel == 255) vel = 60;
  if (extra_speed == 255) extra_speed = 3;
  if (kp < 0) kp = 1.3;
  if (ki < 0) ki = 0.0013;
  if (kd < 0) kd = 0.35;
}

void setup() {
  Serial.begin(115200);    // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.setMinimumBatteryV(
      MINIMUM_BATTERY_V);  // safety voltage for discharging the battery
  one.stop();              // stop motors
  readMenuEEPROM();        // read control values from EEPROM <> Ler valores de
                           // controlo da EEPROM
  one.lcd1("Line Follow PID");
  one.lcd2(" Press a button ");
  // Wait a button to be pressed <> Espera que pressione um botão
  while (one.readButton() == 0);
  // Wait for button release <> Espera que largue o botão
  while (one.readButton() != 0);
}

void loop() {
  int line = 0;      // The line sensor value <> Valor da linha no sensor
  int line_ref = 0;  // Reference line value <> Valor de referência da linha
  int p_error = 0;   // Proportional error <> Erro proporcional
  static double i_error = 0.0;  // Integral error <> Erro Integral
  static int d_error = 0.0;     // Differential error <> Erro diferencial

  // Previous proportional eror <> Erro proporcional anterior
  static int previous_p_error = 0;
  double output = 0.0;  // PID control output <> Resultado do controlo PID

  // Motor speeds <> Velocidade dos motores
  int m1_speed = 0;
  int m2_speed = 0;

  line = one.readLine();  // Read the line sensor value -100 to +100 <> Leitura
                          // do valor da linha -100 a +100

  p_error = line_ref - line;  // Proportional error <> Erro proporcional
  // Differential error <> Erro diferencial
  d_error = p_error - previous_p_error;
  output = (kp * (double)p_error) + (ki * i_error) + (kd * (double)d_error);
  // Clean integral error if line value is zero or if line signal has changed
  // Limpar o erro integral se o valor da linha é zero ou se o sinal da linha
  // mudou
  if ((p_error * previous_p_error) <= 0) {
    i_error = 0.0;
  }
  if (output > LIMITS) {
    output = LIMITS;  // Limit the output value <> Limitar o valor de saída
  } else if (output < (-LIMITS)) {
    output = (-LIMITS);
  } else {
    // Increment integral error if output is below limits <> Incrementa o erro
    // integral se a saída está dentro dos limites
    i_error += (double)p_error;
  }
  previous_p_error = p_error;

  // Limit motors maximum and minimum speed <> Limitar mínimos e máximos da
  // velocidade dos motores
  m1_speed = vel - (int)output;
  m2_speed = vel + (int)output;
  if (m1_speed < -1) {
    // Minimum speed -1 causes motor to brake <> Velocidade mínima -1 faz o
    // motor travar
    m1_speed = -1;
  }
  if (m2_speed < -1) {
    m2_speed = -1;
  }
  if (m1_speed > vel + extra_speed) {
    // Maximum speed limit <> Limite da velocidade máxima
    m1_speed = vel + extra_speed;
  }
  if (m2_speed > vel + extra_speed) {
    m2_speed = vel + extra_speed;
  }

  one.move(m1_speed, m2_speed);

  // Configuration menu <> Menu de configuração
  if (one.readButton() == 3) {
    menu();
  }
}