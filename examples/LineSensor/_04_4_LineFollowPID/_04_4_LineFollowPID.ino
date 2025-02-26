/**
 *  This example was created by José Cruz on October 2016
 *  Updated on February 2025 by José Cruz
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
 * Adjustable gains g_kp, g_ki and g_kd.
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
 * Ajuste dos ganhos g_kp, g_ki, e g_kd.
 * Ajuste do limite de velocidade da roda que está no exterior da curva.
 *
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A library
#include <EEPROM.h>       // EEPROM reading and writing
#include <SPI.h>          // SPI communication library required by BnrOne.cpp
BnrOneAPlus one;          // object to control the Bot'n Roll ONE A

// constants definitions
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define M1 1                    // Motor1
#define M2 2                    // Motor2
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery
#define LIMITS 100.0            // line value limit

// PID control gains <> Ganhos do controlo PID
float g_kp = 1.3;
float g_ki = 0.2;
float g_kd = 0.3;
int g_speed = 50;       // Max Speed <> Velocidade Máxima dos motores
int g_extra_speed = 3;  // Curve outside wheel max speed limit <> Limite de
                        // velocidade da roda exterior na curva

void menu() {
  int temp_var = 0;
  int button = 0;
  double temp = 0.0;
  one.stop();
  one.lcd1("  Menu Config:");
  one.lcd2("PB1+ PB2-  PB3ok");
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (one.readButton() == 3);
  while (one.readButton() == 0);
  while (one.readButton() == 3);

  //***** Maximum speed <> Velocidade Maxima ******
  temp_var = g_speed;
  while (button != 3) {
    one.lcd2("   VelMax:", temp_var);
    button = one.readButton();
    if (button == 1) {
      temp_var++;
      delay(150);
    }
    if (button == 2) {
      temp_var--;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (button != 0) {
    button = one.readButton();
  }
  g_speed = temp_var;

  //**** Outside wheel speed boost <> Incremento de velocidade da roda de fora
  //****
  temp_var = g_extra_speed;
  while (button != 3) {
    one.lcd2("  Curve Boost:", temp_var);
    button = one.readButton();
    if (button == 1) {
      temp_var++;
      delay(150);
    }
    if (button == 2) {
      temp_var--;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (button != 0) {
    button = one.readButton();
  }
  g_extra_speed = temp_var;

  //**** Proportional gain g_kp <> Ganho proporcional g_kp ****
  temp = g_kp * 1000;
  temp_var = (int)temp;
  while (button != 3) {
    one.lcd2("    g_kp:", temp_var);
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
  while (button != 0) {
    button = one.readButton();
  }
  g_kp = (double)temp_var / 1000;

  //**** Integral gain g_ki <> Ganho integral g_ki ****
  temp = g_ki * 10000.0;
  temp_var = (int)temp;
  while (button != 3) {
    one.lcd2("     g_ki:", temp_var);
    button = one.readButton();
    if (button == 1) {
      temp_var += 1;
      delay(150);
    }
    if (button == 2) {
      temp_var -= 1;
      delay(150);
    }
  }
  // Wait PB3 to be released <> Espera que se largue o botão 3
  while (button != 0) {
    button = one.readButton();
  }
  g_ki = (double)temp_var / 10000.0;

  //**** Differential gain g_kd <> Ganho diferencial g_kd ****
  temp = g_kd * 1000;
  temp_var = (int)temp;
  while (button != 3) {
    one.lcd2("     g_kd:", temp_var);
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
  while (button != 0) {
    button = one.readButton();
  }
  g_kd = (double)temp_var / 1000;

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
  byte eeprom_address = 10;

  eeprom_address = writeByteToEEPROM(eeprom_address, g_speed);
  eeprom_address = writeByteToEEProm(eeprom_address, g_extra_speed);

  int temp_var = (int)(g_kp * 1000.0);
  eeprom_address = writeIntToEEPROM(eeprom_address, temp_var);
  temp_var = (int)(g_ki * 10000.0);
  eeprom_address = writeIntToEEPROM(eeprom_address, temp_var);
  temp_var = (int)(g_kd * 1000.0);
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
  byte eeprom_address = 10;

  eeprom_address = readByteFromEEPROM(eeprom_address, g_speed);
  eeprom_address = readByteFromEEPROM(eeprom_address, g_extra_speed);

  int temp_var = 0;
  eeprom_address = readIntFromEEPROM(eeprom_address, temp_var);
  g_kp = (double)temp_var / 1000.0;
  eeprom_address = readIntFromEEPROM(eeprom_address, temp_var);
  g_ki = (double)temp_var / 10000.0;
  eeprom_address = readIntFromEEPROM(eeprom_address, temp_var);
  g_kd = (double)temp_var / 1000.0;

  // Test if values are within limits <> Testa se os valores estão dentro dos
  // limites
  if (!isWithinLimits<byte>(g_speed, 0, 100)) g_speed = 50;
  if (!isWithinLimits<byte>(g_extra_speed, 0, 100)) g_extra_speed = 3;
  if (!isWithinLimits<float>(g_kp, 0.0, 10.0)) g_kp = 1.3;
  if (!isWithinLimits<float>(g_ki, 0.0, 20.0)) g_ki = 0.2;
  if (!isWithinLimits<float>(g_kd, 0.0, 10.0)) g_kd = 0.3;
}

void setup() {
  Serial.begin(115200);   // sets baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.setPid(2200, 245, 60);  // set PID parameters for robot movement

  one.stop();                 // stop motors
  if (one.readButton() == 0)  // Skip read EEPROM is necessary
    readMenuEEPROM();  // read control values from EEPROM <> Ler valores de
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

  // Increment integral error if output is below limits <>
  // Incrementa erro integral se a saída está dentro dos limites
  if (output > LIMITS) {
    output = LIMITS;  // Limit the output value <> Limitar o valor de saída
  } else if (output < (-LIMITS)) {
    output = (-LIMITS);
  } else {
    // Incrementa erro integral
    i_error += (double)p_error;
  }
  // Clean integral error if line value is zero or if line signal has changed
  // Limpar o erro integral se o valor da linha é zero ou se o sinal da linha
  // mudou
  if ((p_error * previous_p_error) <= 0) {
    i_error = 0.0;
  }

  // Differential error <> Erro diferencial
  d_error = p_error - previous_p_error;
  previous_p_error = p_error;

  output =
      (g_kp * (double)p_error) + (g_ki * i_error) + (g_kd * (double)d_error);

  // Limit motors maximum and minimum speed <> Limitar mínimos e máximos da
  // velocidade dos motores
  m1_speed = g_speed - (int)output;
  m2_speed = g_speed + (int)output;
  if (m1_speed < -1) {
    // Minimum speed -1 causes motor to brake <> Velocidade mínima -1 faz o
    // motor travar
    m1_speed = -1;
  }
  if (m2_speed < -1) {
    m2_speed = -1;
  }
  if (m1_speed > g_speed + g_extra_speed) {
    // Maximum speed limit <> Limite da velocidade máxima
    m1_speed = g_speed + g_extra_speed;
  }
  if (m2_speed > g_speed + g_extra_speed) {
    m2_speed = g_speed + g_extra_speed;
  }

  one.move(m1_speed, m2_speed);

  // Configuration menu <> Menu de configuração
  if (one.readButton() == 3) {
    menu();
  }
}