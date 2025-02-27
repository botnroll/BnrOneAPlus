#pragma once

/*Read Commands-> Computer to Bot'n Roll ONE A+*/
#define COMMAND_ARDUINO_ANA0 0xBF  // Read analog0 value
#define COMMAND_ARDUINO_ANA1 0xBE  // Read analog1 value
#define COMMAND_ARDUINO_ANA2 0xBD  // Read analog2 value
#define COMMAND_ARDUINO_ANA3 0xBC  // Read analog3 value
#define COMMAND_ARDUINO_DIG0 0xBB  // Read digital0 value
#define COMMAND_ARDUINO_DIG1 0xBA  // Read digital1 value
//#define COMMAND_ARDUINO_DIG2	0xB9 //Read digital2 value

#define COMMAND_ARDUINO_DIG3 0xB8   // Read digital3 value
#define COMMAND_ARDUINO_DIG4 0xB7   // Read digital4 value
#define COMMAND_ARDUINO_DIG5 0xB6   // Read digital5 value
#define COMMAND_ARDUINO_DIG6 0xB5   // Read digital6 value
#define COMMAND_ARDUINO_DIG7 0xB4   // Read digital7 value
#define COMMAND_ARDUINO_DIG8 0xB3   // Read digital8 value
#define COMMAND_ARDUINO_DIG9 0xB2   // Read digital9 value
#define COMMAND_ARDUINO_DIG10 0xB1  // Read digital10 value
#define COMMAND_ARDUINO_DIG11 0xB0  // Read digital11 value
#define COMMAND_ARDUINO_DIG12 0xAF  // Read digital12 value
#define COMMAND_ARDUINO_DIG13 0xAE  // Read digital13 value
#define COMMAND_ARDUINO_BUZ 0xAD    // Read Buzzer
#define COMMAND_ARDUINO_CMP 0xAC    // Read Compass
#define COMMAND_ARDUINO_SNR 0xAB    // Read Sonar
#define COMMAND_ARDUINO_GRP1 0xAA   // Read gripper1
#define COMMAND_ARDUINO_GRP2 0x9F   // Read gripper2

#define COMMAND_READ_ALL_SENSORS 0xFF   // Read All sensors
#define COMMAND_READ_COLOR_SENSOR 0xC0  // Read Color sensor
