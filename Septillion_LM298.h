#ifndef Septillion_LM298_h
#define Septillion_LM298_h

#include "Arduino.h"

class Septillion_LM298 {
	public:
		Septillion_LM298(byte pinPwm, byte pinIn1, byte pinIn2);
		
		void speed(int speed);
		int getSpeed();
		void update();
		void setDelay(unsigned int vertraging);
		unsigned int getDelay();
		void emergencyStop();
		void start(byte direction);
		void stop();

	protected:
	
		void doSpeed(int speed);
	
		int setSpeed; 	//Snelheid die we willen gaan rijden
		int curSpeed;	//Snelheid waarop de PWM werkelijk is ingesteld.
		unsigned int delay;
		unsigned long millisLast;
		
		byte pinPwm;
		byte pinIn1;
		byte pinIn2;
		
		 
};

#endif