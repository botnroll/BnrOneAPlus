#pragma once

#define KEY1 0xAA  // key used in critical commands
#define KEY2 0x55  // key used in critical commands
// #define BRAKE_TORQUE 100
#define OFF False
#define ON True
#define AN0 0
#define AN1 1
#define AN2 2
#define AN3 3
#define AN4 4
#define AN5 5
#define AN6 6
#define AN7 7

/*User Commands*/
/*Read Firmware version*/
#define COMMAND_FIRMWARE 0xFE  // Read firmware value (integer value)
/*Write Commands->Don't require response from Bot'n Roll ONE A+ */
#define COMMAND_LED 0xFD  // LED
// #define COMMAND_SERVO1          0xFC //Move Servo1
// #define COMMAND_SERVO2          0xFB //Move Servo2
#define COMMAND_LCD_L1 0xFA       // Write LCD line1
#define COMMAND_LCD_L2 0xF9       // Write LCD line2
#define COMMAND_IR_EMITTERS 0xF8  // IR Emmiters ON/OFF
#define COMMAND_STOP 0xF7         // Stop motors freeley
#define COMMAND_MOVE 0xF6         // Move motors
#define COMMAND_BRAKE_SET_T 0xF5  // Stop motors setting brake torque
#define COMMAND_SET_BAT_MIN 0xF4  // Set low battery voltage
#define COMMAND_SET_PID 0xF3      // Set kp, ki, kd values for PID control
// Set motors calibration parameters (SPow, ctrl_pulses)
#define COMMAND_MOVE_RAW 0xF2
#define COMMAND_SET_MOTORS 0xF1  // Save calibration data
#define COMMAND_ENCL_RESET 0xF0  // Preset the value of encoder1
#define COMMAND_ENCR_RESET 0xEF  // Preset the value of encoder2
#define COMMAND_MOVE_RPM 0xEE    // Sends the speed of the motors in RPM
// Sends the speed of the motors in RPM and requests the encoders readings
#define COMMAND_MOVE_RPM_R_ENC 0xED
#define COMMAND_FUTURE_USE4 0xEC
#define COMMAND_BRAKE_MAX_T 0xEB  // Stop motors with MAX brake torque
#define COMMAND_MOVE_1M 0xEA      // Move 1 motor
#define COMMAND_STOP_1M 0xE9      // Stop 1 motor
#define COMMAND_BRAKE_1M 0xE8     // Brake 1 motor
#define COMMAND_FUTURE_USE5 0xE7

/*Read Commands-> requests to Bot'n Roll ONE A+ */
#define COMMAND_ADC0 0xDF      // Read ADC0
#define COMMAND_ADC1 0xDE      // Read ADC1
#define COMMAND_ADC2 0xDD      // Read ADC2
#define COMMAND_ADC3 0xDC      // Read ADC3
#define COMMAND_ADC4 0xDB      // Read ADC4
#define COMMAND_ADC5 0xDA      // Read ADC5
#define COMMAND_ADC6 0xD9      // Read ADC6
#define COMMAND_ADC7 0xD8      // Read ADC7
#define COMMAND_BAT_READ 0xD7  // Read ADC battery
#define COMMAND_BUT_READ 0xD6  // Read ADC button
// Read IR obstacle sensors -> RobertaLab compatibility
#define COMMAND_OBSTACLES 0xD5
#define COMMAND_FUTURE_USE7 0xD4
#define COMMAND_ENCL 0xD3           // Read Encoder1 position
#define COMMAND_ENCR 0xD2           // Read Encoder2 position
#define COMMAND_ENCL_INC 0xD1       // Read Encoder1 Incremental value
#define COMMAND_ENCR_INC 0xD0       // Read Encoder2 Incremental value
#define COMMAND_LINE_READ 0xCF      // Read 16bytes line buffer at once
#define COMMAND_RANGE_LEFT 0xCE     // Read IR obstacles distance range
#define COMMAND_RANGE_RIGHT 0xCD    // Read IR obstacles distance range
#define COMMAND_ENCODERS_READ 0XCC  // Read EncoderL + EncoderR in 1 request
#define COMMAND_RANGES_READ 0XCB    // Read RangeL + RangeR in 1 request
