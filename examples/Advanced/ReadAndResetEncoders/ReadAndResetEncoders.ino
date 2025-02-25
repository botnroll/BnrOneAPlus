/**
 * This example was created by Nino Pereira on February 2025
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot moves by taking in speed values in rpm (rotations per minute)
 * It then reads both encoders in one go and resets them.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A

// constants definition
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

void testMoveRpmRequestEncoders(BnrOneAPlus& one) {
  int left_encoder = 0;
  int right_encoder = 0;
  one.readAndResetEncoders(left_encoder, right_encoder);
  one.lcd1("Left: ", left_encoder);
  one.lcd2("Right: ", right_encoder);
  Serial.print("Left: ");
  Serial.print(left_encoder);
  Serial.print(" Right: ");
  Serial.println(right_encoder);
}

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.setPid(2200, 245, 60);  // set PID parameters for robot movement

  one.lcd1(" Read Reset Enc ");
  one.lcd2(" Press a button ");
  // Wait a button to be pushed <> Espera que pressione um botão
  while (one.readButton() == 0)
    ;

  for (int i = -200; i < 200; i += 2) {
    one.moveRpm(i, i);
    testMoveRpmRequestEncoders(one);
    delay(100);
  }
  for (int i = 200; i > -200; i -= 2) {
    one.moveRpm(i, i);
    testMoveRpmRequestEncoders(one);
    delay(100);
  }
  one.stop();  // stop motors
}

void loop() {}
