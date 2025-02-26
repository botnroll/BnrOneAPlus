/*
 * This example was created by José Cruz on January 2025
 *
 * This example aquires motor and encoder information to be sent to PIC18F45K22
 * for PID control of the movement. The robot wheels must rotate freely and
 * should not touch the floor. Motors must have encoders. This process is done
 * in 3 steps: Step 1: PWM will increase from 0 until movement is detected from
 * the encoders. Setp 2: with motors at maximum power, encoders counting will be
 * aqquired every 25ms. Step 3: send data to PIC to be stored in EEPROM.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
#include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp

BnrOneAPlus one;  // object to control the Bot'n Roll ONE A+

// constants definition
#define SSPIN 2                 // Slave Select (SS) pin for SPI communication
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

int g_motor_power = 40;
int g_left_enc_max = 0;
int g_right_enc_max = 0;
bool g_error_flag = false;

void printDebugEncoderMax() {
  Serial.print("  g_left_enc_max:");
  Serial.print(g_left_enc_max);
  Serial.print("  g_right_enc_max:");
  Serial.print(g_right_enc_max);
}

void printDebugEncodersMaxPowerAndError() {
  Serial.print("  g_motor_power:");
  Serial.print(g_motor_power);
  printDebugEncoderMax();
  Serial.print("  g_error_flag:");
  Serial.print(g_error_flag);
  Serial.println();
}

void printDebugInfo(const int left_enc, const int right_enc) {
  Serial.print("  left_enc:");
  Serial.print(left_enc);
  Serial.print("  right_enc:");
  Serial.print(right_enc);
  Serial.println();
}

void startMoveDetection() {
  bool exit_flag = false;
  int left_enc = one.readAndResetLeftEncoder();    // Clear encoder count
  int right_enc = one.readAndResetRightEncoder();  // Clear encoder count
  unsigned long t1sec = millis() + 1000;
  while (!exit_flag) {
    if (millis() >= t1sec) {
      t1sec += 500;  // Every second
      one.moveRAW(g_motor_power, g_motor_power);
      left_enc = one.readAndResetLeftEncoder();
      right_enc = one.readAndResetRightEncoder();
      one.lcd2(g_motor_power, left_enc, right_enc);
      printDebugEncodersMaxPowerAndError();
      if (abs(left_enc) < 100 || abs(right_enc) < 100)
        g_motor_power++;  // if motors are not moving
      else {
        if (left_enc < 0) {  // if encoderL is Not ok
          one.stop();
          one.lcd1("Motor 1 -> ERROR");
          Serial.println("ERROR: Motor 1 encoder is counting in reverse!!");
          g_error_flag = true;
        } else if (right_enc < 0) {  // if encoderR is Not ok
          one.stop();
          one.lcd2("Motor 2 -> ERROR");
          Serial.println("ERROR: Motor 2 encoder is counting in reverse!!");
          g_error_flag = true;
        }
        exit_flag = true;
      }
    }
  }
}

void maxPulsesDetection() {
  unsigned long t_cycle;
  unsigned long end_time;
  if (!g_error_flag) {
    one.lcd2(100, 0, 0);
    one.moveRAW(100, 100);
    delay(1500);
    t_cycle = millis();
    end_time = millis() + 5000;
    int left_enc = one.readAndResetLeftEncoder();    // Clear encoder count
    int right_enc = one.readAndResetRightEncoder();  // Clear encoder count
    while (millis() < end_time) {
      if (millis() >= t_cycle) {
        t_cycle += 25;
        left_enc = one.readAndResetLeftEncoder();
        right_enc = one.readAndResetRightEncoder();
        if (left_enc > g_left_enc_max) g_left_enc_max = left_enc;
        if (right_enc > g_right_enc_max) g_right_enc_max = right_enc;
        printDebugInfo(left_enc, right_enc);
      }
    }
    one.stop();
    g_left_enc_max = (int)((float)g_left_enc_max);
    g_right_enc_max = (int)((float)g_right_enc_max);
    one.lcd2(0, g_left_enc_max, g_right_enc_max);
    printDebugEncoderMax();
    delay(2000);
  }
}

void sendValues() {
  int enc_min = 30000;  // Find minimum encoder value
  if (!g_error_flag) {
    if (g_left_enc_max < enc_min) enc_min = g_left_enc_max;
    if (g_right_enc_max < enc_min) enc_min = g_right_enc_max;
    one.setMotors(g_motor_power, enc_min);
    Serial.println();
    Serial.println(
        "  Save values for void setMotors(int Smotor_power, int "
        "ctrlPulses);");
    Serial.print("  Smotor_power:");
    Serial.println(g_motor_power);
    Serial.print("  ctrlPulses:");
    Serial.println(enc_min);
    Serial.println("  Calibrate Finished!!");
    while (1) {
      one.lcd1("Smotor_power: ", g_motor_power);
      one.lcd2("ctrlPulses: ", enc_min);
      delay(2500);
      one.lcd1("Save values for ");
      one.lcd2("  setMotors();  ");
      delay(2500);
      one.lcd1("  Calibration   ");
      one.lcd2("   finished!    ");
      delay(2500);
    }
  }
  while (1);
}

void setup() {
  Serial.begin(115200);   // set baud rate to 115200bps for printing values at
                          // serial monitor.
  one.spiConnect(SSPIN);  // start SPI communication module
  one.stop();             // stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  one.setPid(2200, 245, 60);  // set PID parameters for robot movement

  one.lcd1(" Press a button ");
  one.lcd2("   to start!    ");
  while (one.readButton() == 0);
  one.lcd1("Motors Calibrate");
  one.lcd2(" !!Attention!!  ");
  delay(2000);
  one.lcd1("Wheels must not ");
  one.lcd2("touch the Floor!");
  delay(2000);
  one.lcd1(" Press a button ");
  one.lcd2("   to start!    ");
  while (one.readButton() == 0);
  one.lcd1("Power EncL EncR ");
}

void loop() {
  // 1-Detect Start Moving Power
  startMoveDetection();
  // 2-Detect control pulses at max speed every 25ms
  maxPulsesDetection();
  // 3-Send values to PIC18F45K22
  sendValues();
}