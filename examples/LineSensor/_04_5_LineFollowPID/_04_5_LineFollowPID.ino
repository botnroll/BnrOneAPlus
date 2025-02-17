/*
 This example was created by José Cruz on December 2024

 This code example is in the public domain.
 http://www.botnroll.com

IMPORTANT!!!!
Before you use this example you MUST calibrate the line sensor. Use example
_04_1_Calibrate.ino first!!! Line reading provides a linear value between -100
to 100

Line follow:
Motors speed varies using PID control.
Adjustable gains kp, ki and kd.
You can adjust the speed limit of the wheel that is outside the curve.
Press push button 3 (PB3) to enter control configuration menu.

<>

IMPORTANTE!!!!
Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar exemplo
_04_1_Calibrate.ino antes deste!!! Leitura da linha que devolve um valor linear
entre -100 e +100 da posição da linha.

Seguimento de linha:
Os motores variam com a linha com controlo PID
Ajuste dos ganhos kp, ki, e kd.
Ajuste do limite de velocidade da roda que está no exterior da curva.

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

float batmin = 10.5;  // safety voltage for discharging the battery

#define LIMITS 100.0
double kp = 1.3, ki = 0.0013,
       kd = 0.35;  // PID control gains <> Ganhos do controlo PID
int vel = 60;      // Max Speed <> Velocidade Máxima dos motores
int vCurve = 3;  // Curve outside wheel max speed limit <> Limite de velocidade
                 // da roda exterior na curva

void setup() {
  Serial.begin(57600);    // sets baud rate to 57600bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // starts the SPI communication module
  one.setMinBatteryV(batmin);  // safety voltage for discharging the battery
  one.stop();                  // stop motors
  readMenuEEPROM();  // read control values from EEPROM <> Ler valores de
                     // controlo da EEPROM
  one.lcd1("Line Follow PID");
  one.lcd2(" Press a button ");
  while (one.readButton() ==
         0);  // Wait a button to be pressed <> Espera que pressione um botão
  while (one.readButton() !=
         0);  // Wait for button release <> Espera que largue o botão
}

void loop() {
  int line = 0;     // The line sensor value <> Valor da linha no sensor
  int lineRef = 0;  // Reference line value <> Valor de referência da linha
  int erroP = 0;    // Proportional error <> Erro proporcional
  static double erroI = 0.0;  // Integral error <> Erro Integral
  static int erroD = 0.0;     // Differential error <> Erro diferencial
  static int erroPAnt =
      0;  // Previous proportional eror <> Erro proporcional anterior
  double output = 0.0;  // PID control output <> Resultado do controlo PID

  int velM1 = 0, velM2 = 0;  // Motor speeds <> Velocidade dos motores

  line = one.readLine();  // Read the line sensor value -100 to +100 <> Leitura
                          // do valor da linha -100 a +100
  // Serial.print(" Line:");Serial.print(line);

  erroP = lineRef - line;    // Proportional error <> Erro proporcional
  erroD = erroP - erroPAnt;  // Differential error <> Erro diferencial
  output = kp * (double)erroP + ki * erroI + kd * (double)erroD;
  // Clean integral error if line value is zero or if line signal has changed
  // Limpar o erro integral se o valor da linha é zero ou se o sinal da linha
  // mudou
  if (erroP * erroPAnt <= 0) erroI = 0.0;
  if (output > LIMITS)
    output = LIMITS;  // Limit the output value <> Limitar o valor de saída
  else if (output < -LIMITS)
    output = -LIMITS;
  else
    erroI += (double)
        erroP;  // Increment integral error if output is below limits <>
                // Incrementa o erro integral se a saída está dentro dos limites
  erroPAnt = erroP;
  // Serial.print("  Out:"); Serial.print(output);

  // Limit motors maximum and minimum speed <> Limitar mínimos e máximos da
  // velocidade dos motores
  velM1 = vel - (int)output;
  velM2 = vel + (int)output;
  if (velM1 < -1)
    velM1 = -1;  // Minimum speed -1 causes motor to brake <> Velocidade mínima
                 // -1 faz o motor travar
  if (velM2 < -1) velM2 = -1;
  if (velM1 > vel + vCurve)
    velM1 = vel + vCurve;  // Maximum speed limit <> Limite da velocidade máxima
  if (velM2 > vel + vCurve) velM2 = vel + vCurve;

  // Serial.print("   M1:");  Serial.print(velM1); Serial.print("   M2:");
  // Serial.println(velM2);
  one.move(velM1, velM2);

  // Configuration menu <> Menu de configuração
  if (one.readButton() == 3) menu();  // Entra no menu
}

void menu() {
  int var = 0;
  int butt = 0;
  double temp = 0.0;
  one.stop();
  one.lcd1("  Menu Config:");
  one.lcd2("PB1+ PB2- PB3ok");
  delay(150);
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
  while (butt !=
         0)  // Wait PB3 to be released <> Espera que se largue o botão 3
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
  while (butt !=
         0)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    butt = one.readButton();
  }
  vCurve = var;

  //**** Proportional gain kp <> Ganho proporcional kp ****
  temp = kp * 1000;
  var = (int)temp;
  while (butt != 3) {
    one.lcd2("    kp:", var);
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
  while (butt !=
         0)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    butt = one.readButton();
  }
  kp = (double)var / 1000;

  //**** Integral gain ki <> Ganho integral ki ****
  temp = ki * 10000.0;
  var = (int)temp;
  while (butt != 3) {
    one.lcd2("     ki:", var);
    butt = one.readButton();
    if (butt == 1) {
      var += 1;
      delay(150);
    }
    if (butt == 2) {
      var -= 1;
      delay(150);
    }
  }
  while (butt !=
         0)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    butt = one.readButton();
  }
  ki = (double)var / 10000.0;

  //**** Differential gain kd <> Ganho diferencial kd ****
  temp = kd * 1000;
  var = (int)temp;
  while (butt != 3) {
    one.lcd2("     kd:", var);
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
  while (butt !=
         0)  // Wait PB3 to be released <> Espera que se largue o botão 3
  {
    butt = one.readButton();
  }
  kd = (double)var / 1000;

  //**** Configuration end <> Termina Configuração *****
  writeMenuEEPROM();  // Write control values to EEPROM <> Escrever valores de
                      // controlo na EEPROM
  one.lcd1("Line  Following!");
  one.lcd2("www.botnroll.com");
  delay(250);
}

// Write values on EEPROM <> Escrever valores na EEPROM
void writeMenuEEPROM() {
  byte eepromADD = 10;
  int var = 0;

  var = vel;
  EEPROM.write(eepromADD, lowByte(var));  // Guardar em EEPROM
  eepromADD++;
  var = vCurve;
  EEPROM.write(eepromADD, lowByte(var));  // Guardar em EEPROM
  eepromADD++;
  var = (int)(kp * 1000.0);
  EEPROM.write(eepromADD, highByte(var));  // Guardar em EEPROM
  eepromADD++;
  EEPROM.write(eepromADD, lowByte(var));
  eepromADD++;
  var = (int)(ki * 10000.0);
  EEPROM.write(eepromADD, highByte(var));  // Guardar em EEPROM
  eepromADD++;
  EEPROM.write(eepromADD, lowByte(var));
  eepromADD++;
  var = (int)(kd * 1000.0);
  EEPROM.write(eepromADD, highByte(var));  // Guardar em EEPROM
  eepromADD++;
  EEPROM.write(eepromADD, lowByte(var));
  eepromADD++;
}

// Read values from EEPROM <> Ler valores da EEPROM
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
  kp = (double)var / 1000.0;
  var = 0;
  var = (int)EEPROM.read(eepromADD);
  eepromADD++;
  var = var << 8;
  var += (int)EEPROM.read(eepromADD);
  eepromADD++;
  ki = (double)var / 10000.0;
  var = 0;
  var = (int)EEPROM.read(eepromADD);
  eepromADD++;
  var = var << 8;
  var += (int)EEPROM.read(eepromADD);
  eepromADD++;
  kd = (double)var / 1000.0;
  if (vel == 255) vel = 60;
  if (vCurve == 255) vCurve = 3;
  if (kp < 0) kp = 1.3;
  if (ki < 0) ki = 0.0013;
  if (kd < 0) kd = 0.35;
}
