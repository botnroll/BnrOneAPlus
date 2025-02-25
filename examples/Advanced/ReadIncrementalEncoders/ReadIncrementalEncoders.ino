/**
 * This example was created by Nino Pereira on February 2025
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot moves by taking in speed values in rpm (rotations per minute)
 * It then reads both incremental encoders and stop the motors when the desired
 * value is reached. Incremental encoder counting will overflow when value is
 * higher than +32767 ou lower than -32768.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;  // object to control the Bot'n Roll ONE A

// constants definition
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.setPid(2200, 245, 60);  // set PID parameters for robot movement
  one.lcd1(" Incremental Enc ");
  one.lcd2(" Press a button ");
  // Wait a button to be pushed <> Espera que pressione um botão
  one.resetLeftEncoder();   // Reset encoder value to 0 <> Ler valor incremental
                            // do encoder
  one.resetRightEncoder();  /// Reset encoder value to 0
  while (one.readButton() == 0);
}

void printDebugInfo(const int left_encoder, const int right_encoder) {
  Serial.print("Left: ");
  Serial.print(left_encoder);
  Serial.print(" Right: ");
  Serial.println(right_encoder);
}

void resetEncoders() {
  one.resetLeftEncoder();   // Reset encoder value to 0
  one.resetRightEncoder();  // Reset encoder value to 0
}

void readAndPrintEncoders() {
  int left_encoder =
      one.readIncrementalLeftEncoder();  // Read incremental encoder
  int right_encoder =
      one.readIncrementalRightEncoder();  // Read incremental encoder
  one.lcd1("Left: ", left_encoder);       // Print the encoder value
  one.lcd2("Right: ", right_encoder);
  printDebugInfo(left_encoder, right_encoder);
}

void brakeResetAndPrintEncoders() {
  one.brake();  // Brake motors
  delay(1500);
  resetEncoders();
  readAndPrintEncoders();
  delay(1500);
}

void loop() {
  int left_encoder = 0;
  int right_encoder = 0;

  one.moveRpm(120, 120);  // Move motors in rpm

  while (left_encoder < 1000) {
    readAndPrintEncoders();
  }
  brakeResetAndPrintEncoders();

  one.moveRpm(-60, -60);  // Move motors in rpm

  while (left_encoder > -500) {
    readAndPrintEncoders();
  }
  brakeResetAndPrintEncoders();
}
