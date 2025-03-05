/*
 This example was created by José Cruz (www.botnroll.com)
 on the 10th February 2025

 This code example is in the public domain.
 http://www.botnroll.com

Description:
Read obstacle sensors range distance for left and right IR sensors.
Range varies from 0 to 20:
 -> 0 means no obstacle is detected
 -> 20 means obstacle is very close to the sensor
The robot has new readings every 25ms (40 readings per second)
*/

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define OFF 0
#define ON 1
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

int left_speed = 0;
int right_speed = 0;
int g_speed = 20;      // Max speed
unsigned long tcycle;  // Control cycle time in loop
// Range Linear gain <> Ganho linear do obstáculo (2000)
float g_obstacle_gain = 2.0;
byte left_range;      // Left obstacle sensor range
byte right_range;     // Right obstacle sensor range
byte max_range = 18;  // Maximum range value for obstacle
byte min_range = 1;   // Minimum range value for obstacle

float g_line_gain = 1.0;  // Line linear gain <> Ganho linear da linha
int g_extra_speed = 8;    // Curve outside wheel max g_speed limit <> Limite de

void setup() {
  one.spiConnect(SSPIN);                  // start SPI communication module
  one.stop();                             // stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.obstacleSensorsEmitters(ON);        // activate IR emitters
  one.lcd1("   Crazy Race   ");
  one.lcd2("Press a Button!!");
  // Wait a button to be pressed <> Espera que pressione um botão
  while (one.readButton() == 0)
    ;
  // Wait for button release <> Espera que largue o botão
  while (one.readButton() != 0)
    ;

  tcycle = millis();  // Set start value for tcycle
}

void loop() {
  if (millis() > tcycle) {
    tcycle += 25;  // Loop every 25ms

    if (isAllDark(one.readLineSensor(), 450)) {
      readAndProcessObstacles();  // Read obstacles and decide how to move
    } else {
      readAndProcessLine();  // Follow line
    }
    one.move(left_speed, right_speed);
  }
}

void readAndProcessLine() {
  const int line = one.readLine();
  // Linear function for Motor1 <> Função linear para o Motor1
  left_speed = (int)((float)g_speed + ((float)line * g_line_gain));
  // Linear function for Motor2 <> Função linear para o Motor2
  right_speed = (int)((float)g_speed - ((float)line * g_line_gain));

  // Limit motors maximum and minimum g_speed <> Limitar mínimos e máximos
  // da velocidade dos motores
  left_speed = constrain(left_speed, -15, g_speed + g_extra_speed);
  right_speed = constrain(right_speed, -15, g_speed + g_extra_speed);
}

void readAndProcessObstacles() {
  left_range = one.readLeftRangeSensor();    // read left obstacle sensor range
  right_range = one.readRightRangeSensor();  // read right obstacle sensor range

  // if obstacle is near the left sensor - Upper limit range
  if (left_range >= max_range) {
    right_speed = -g_speed;
    left_speed = g_speed;
  }
  // if obstacle is near the right sensor - Upper limit range
  else if (right_range >= max_range) {
    right_speed = g_speed;
    left_speed = -g_speed;
  }
  // If obstacle is far away it is ignored - Lower limit range
  if ((left_range <= min_range) && (right_range <= min_range)) {
    right_speed = g_speed;
    left_speed = g_speed;
  }
  // If obstacle is in the active range
  else {
    // if obstacle is closer to the left sensor
    if (left_range > right_range) {
      left_speed = (g_speed + (int)((float)left_range / 2.0));
      right_speed =
          (g_speed - (int)((float)left_range * (float)g_obstacle_gain));
      if (right_speed < -g_speed) {
        right_speed = -g_speed;
      }
    }
    // if obstacle is closer to the right sensor
    else {
      right_speed = (g_speed + (int)((float)right_range / 2.0));
      left_speed =
          (g_speed - (int)((float)right_range * (float)g_obstacle_gain));
      if (left_speed < -g_speed) {
        left_speed = -g_speed;
      }
    }
  }
}

// Check if the floor is all dark
boolean isAllDark(int readings[8], int threshold) {
  for (int i = 0; i < 8; i++) {
    if (readings[i] < threshold) {
      return false;
    }
  }
  return true;
}
