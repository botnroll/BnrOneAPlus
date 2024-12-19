/* 
 This example was created by the Laboratory of Automation and Robotics for the RoboParty Fun Challenge (https://lar.dei.uminho.pt/)
 on 18 December 2024
  
 This code example is in the public domain. 
 http://www.botnroll.com

Description:  
This program detects automatic start and does the automatic end on the RoboParty Fun Challenge.
*/

#include <BnrOneAPlus.h>   // Bot'n Roll ONE A+ library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOneAPlus.cpp
BnrOneAPlus one;           // declaration of object variable to control the Bot'n Roll ONE A+

//constants definition
#define SSPIN  2       // Slave Select (SS) pin for SPI communication

int counter = 0; 
int CHALLENGE_TIME = 90;   // challenge time

void setup()
{
  
  one.spiConnect(SSPIN);  // start SPI communication module
	one.stop();             // stop motors
  initialize_timer();     // configures the interrupt timer for the end of the challenge
	one.lcd1("FUN CHALLENGE"); // print on LCD line 1
	one.lcd2("READY TO START..");       // print on LCD line 2
	one.obstacleEmitters(OFF); // deactivate obstacles IR emitters
	delay(4000);            // time to stabilize IR sensors (DO NOT REMOVE!!!)
        static byte start=0;
 	while(!start)
        {
          start=automatic_start();
        }
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  one.obstacleEmitters(ON); // deactivate obstacles IR emitters

}


bool automatic_start()
{ 
	bool actstate=one.readIRSensors(); // read actual IR sensors state
	
	if(!actstate) // If state is LOW
	{
		unsigned long int tempo_A=millis(); // read time
		while(!actstate) // while state is LOW
		{
			actstate=one.readIRSensors(); // read actual IR sensors state
			if((millis()-tempo_A)>50) // if time is low for more than 50ms
			{
				return true; // start Race
			}
		}
	}
	return false;
}

void initialize_timer()
{
    
  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  

  sei();//allow interrupts
  
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz
  
  if (counter >= CHALLENGE_TIME){
    one.lcd2("END OF CHALLENGE");     // print on LCD line 2
    while(1)              // does not allow anything else to be done after the challenge ends
    {
      one.brake(100, 100);  // Stop motors with torque

      // place code here, to stop any additional actuators... 
    
    }
  }
  else
  {
    one.lcd2(counter);    // print the challenge time on LCD line 2
  }
  counter++;
  
}


void loop()
{

  // place code here, to control the robot ...

}
