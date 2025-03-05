/**
 * This example was created by the Laboratory of Automation and Robotics for the
 * RoboParty Fun Challenge (https://lar.dei.uminho.pt/) on 18 December 2024
 *
 * Updated on February 2025 by José Cruz
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * This program detects automatic start and does the automatic end on the
 * RoboParty Fun Challenge.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define OFF 0
#define ON 1
#define CHALLENGE_TIME 90  // challenge time in seconds
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

int counter = 0;
int estado = 0;
float media = 0.0;
int sensor[8];

void setup() {
  one.spiConnect(SSPIN);                  // start SPI communication module
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.stop();                             // stop motors
  one.lcd1(" FUN CHALLENGE  ");           // print on LCD line 1
  one.lcd2(" Press a button ");           // print on LCD line 2
  while (one.readButton() == 0)
    ;
}

void loop() {
  media = 0;
  // para ler os sensores de linha
  for (int i = 0; i < 8; i++) {
    sensor[i] = one.readAdc(i);
    media += sensor[i] / 8;
  }

  // sensor[0] é o valor do sensor 0
  // sensor[1] é o valor do sensor 1
  // ...
  // media é o valor médio dos 8 sensores de linha

  // Este exemplo é baseado numa máquina de estados.
  // Cada estado simboliza uma tarefa diferente que o robô tem que fazer,
  // seja andar para a frente ou andar para trás.
  // Os 'if' no fim de cada estado simbolizam as condições
  // que fazem com que o robô altere a tarefa que está a fazer.
  // Por exemplo se o robô estiver na tarefa de ir em frente e se detectar um
  // obstáculo, altera o seu estado/tarefa para andar para trás

  switch (estado) {
    case 0:  // anda para a frente até detetar linha de meio campo ou sensores
             // obstaculos
      one.move(80, 80);

      // se detetar algum obstáculo, altera a tarefa para andar para trás
      if (one.readObstacleSensors() > 0) {
        estado = 1;
      }

      // se detetar uma media dos sensores acima de 900 (todos os sensores a
      // preto), altera a tarefa para andar para trás
      if (media > 900) {
        estado = 1;
      }

      break;

    case 1:  // anda para tras até os sensores estarem na parte branca
      one.move(-80, -80);

      // se detetar uma media dos sensores abaixo de 100 (todos os sensores a
      // branca), altera a tarefa para andar para trás mas fica à espera que o
      // robô seja levantado
      if (media < 100) {
        estado = 2;
      }

      break;

    case 2:  // anda para trás até ser levantado
      one.move(-80, -80);

      // se detetar uma media dos sensores acima de 900 (todos os sensores a
      // preto), altera a tarefa para andar para a frente o que significa que o
      // robô foi levantado
      if (media > 900) {
        estado = 3;
      }

      break;

    case 3:  // anda para a frente até os sensores estarem na parte branca (robô
             // ser pousado na pista)
      one.move(80, 80);

      // se detetar uma media dos sensores abaixo de 100 (todos os sensores a
      // branca), altera a tarefa para andar para a frente mas fica à espera que
      // o robô detete um obstáculo ou a linha de meio campo
      if (media < 100) {
        estado = 0;
      }

      break;
  }
}
