/**
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Description:
 * The robot moves by taking in speed values in rpm (rotations per minute)
 * and receives back the encoder values.
 */

 #include <BnrOneAPlus.h>  // Bot'n Roll ONE A+ library
 #include <SPI.h>  // SPI communication library required by BnrOneAPlus.cpp
 BnrOneAPlus one;  // object to control the Bot'n Roll ONE A

 // constants definition
 #define SSPIN 2  // Slave Select (SS) pin for SPI communication

 void testMoveRpmRequestEncoders(int increment) {
    int left_encoder = 0;
    int right_encoder = 0;
    one.moveRpmGetEncoders(200 + increment, 200 + increment, left_encoder, right_encoder);
    one.lcd1("Left: ", left_encoder);
    one.lcd2("Right: ", right_encoder);
    Serial.print("Left: ");
    Serial.print(left_encoder);
    Serial.print(" Right: ");
    Serial.println(right_encoder);
}

 void setup() {
    Serial.begin(115200);    // set baud rate to 115200bps for printing values at
                            // serial monitor.
    one.spiConnect(SSPIN);  // start SPI communication module
    one.stop();             // stop motors
    for (int i=0; i < 50; ++i) {
        testMoveRpmRequestEncoders(i);
        delay(100);
    }
    one.stop();             // stop motors
}

void loop(){

}

