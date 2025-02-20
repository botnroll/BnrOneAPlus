/**
 * This example was created by José Cruz (www.botnroll.com)
 * on the 18th December 2024
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot moves and messages are printed on the LCD identifying the
 * movements.
 */

 #include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
 #include <EEPROM.h>       // EEPROM reading and writing
 #include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
 BnrOneAPlus one;  // object to control the Bot'n Roll ONE A
 
 // constants definition
 #define SSPIN 2  // Slave Select (SS) pin for SPI communication
 
 void setup() {
   Serial.begin(57600);    // set baud rate to 57600bps for printing values at
                           // serial monitor.
   one.spiConnect(SSPIN);  // start SPI communication module
   one.stop();             // stop motors
 }
 
 void loop() {
   one.lcd2("    Forward ");  // print data on LCD line 2
   one.move(50, 50);          // Forward
   delay(1000);               // wait 1 second
   one.lcd2("     Stop   ");
   one.stop();  // Stop Motors
   delay(500);
   one.lcd2("   Backwards ");
   one.move(-50, -50);  // Backwards
   delay(1000);
   one.lcd2("     Stop   ");
   one.move(0, 0);  // Stop Motors
   delay(500);
   one.lcd2("  Rotate Right ");
   one.move(50, -50);  // Rotate Right
   delay(500);
   one.lcd2("     Stop   ");
   one.stop();  // Stop
   delay(500);
   one.lcd2("  Rotate Left ");
   one.move(-50, 50);  // Rotate Left
   delay(500);
   one.lcd2("     Stop   ");
   one.stop();  // Stop Motors
   delay(500);
   one.lcd2("    Forward ");
   one.move(100, 100);  // Forward
   delay(1000);
   one.lcd2("     Brake    ");
   one.brake(100, 100);  // Stop motors with torque
   delay(800);
   one.lcd2("   Backwards ");
   one.move(-100, -100);  // Backwards
   delay(1000);
   one.lcd2("     Brake    ");
   one.brake(100, 100);  // Stop motors with torque
   delay(800);
   one.lcd2("     Stop   ");
   one.stop();  // Stop Motors
   delay(1500);
 }
 