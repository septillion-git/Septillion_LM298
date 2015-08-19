/*
	Septillion_LM298.cpp v2 - Library to drive a LM298 motor driver
	Written by Timo Engelgeer
*/

/*
	With this library you can drive one H-bridge of a LM298 motor driver. You need two objects to drive a full LM298.
  
  Object creation
  Septillion_LM298 myLM298(pinPwm, pinIn1, pinIn2);
  pinPwm is the enable pin of the driver.
  
  myLM298.update() needs to be called in the main loop(). It updates all the functions and is non blocking.
  
  myLM298.speed(speed) - Set speed between -255 (reverse) and 255 (forward)
  myLM298.getSpeed() - Returns last set speed
  myLM298.update() - Updates all the setting (including delay) in a non-blocking way
  myLM298.setDelay(delay) - Sets the delay between every step in speed (in ms)
  myLM298.getDelay() - Returns last set delay (in ms)
  myLM298.emergencyStop() - Activates fast motor braking
  myLM298.start(direction) - Short for myLM298.speed(+-120) with direction (0 = forward, 1 = reverse)
  myLM298.stop() - Sets speed to 0 (myLM298.speed(0))
 
*/


#include "Septillion_LM298.h"


Septillion_LM298::Septillion_LM298(byte pinPwm, byte pinIn1, byte pinIn2){
	//Save the pins to use
	this->pinPwm = pinPwm; 
	this->pinIn1 = pinIn1; 
	this->pinIn2 = pinIn2; 
	
	//Make them output
	pinMode(this->pinPwm, OUTPUT);
	pinMode(this->pinIn1, OUTPUT);
	pinMode(this->pinIn2, OUTPUT);
	
	this->delay = 0; //Standaard geen vertraging
	
}

void Septillion_LM298::speed(int speed){
	//Limit speed between -255 and 255
	if(speed > 255){
		speed = 255;
	}
	else if(speed < -255){
		speed = -255;
	}
	
	setSpeed = speed;
	
	if(delay == 0){
		this->doSpeed(setSpeed);
	}
}

//Returns set spees
int Septillion_LM298::getSpeed(){
	return this->setSpeed;
}


//Protected functie, niet van buiten de classe te bena  deren.
//Stelt werkelijk de hardware in op de juiste snelheid
void Septillion_LM298::doSpeed(int speed){
	if(curSpeed != speed){
		curSpeed = speed;
		
		if(speed < 0){
			digitalWrite(this->pinIn1, HIGH);
			digitalWrite(this->pinIn2, LOW);
			analogWrite(this-> pinPwm, -speed);
		}
		else{
			digitalWrite(this->pinIn1, LOW);
			digitalWrite(this->pinIn2, HIGH);
			analogWrite(this-> pinPwm, speed);
		}
	}
}

//Checkt of de snelheid aangepast moet worden (optrekken/afremmen) en doet dit
//Aanroepen in de loop
void Septillion_LM298::update(){
	//Kijken of het tijd is om een update te doen
  unsigned int millisNow = millis();
	if(millisNow - this->millisLast > this->delay) {
		this->millisLast = millis(); //opslaan dat we nu een update doen
		
		if(setSpeed > curSpeed){ //moeten dus sneller
			this->doSpeed(curSpeed + 1);
		}
		else if(setSpeed < curSpeed){	//moeten langzamer
			this->doSpeed(curSpeed - 1);
		}
	}
}

void Septillion_LM298::setDelay(unsigned int delay){
	this->delay = delay;
}


//Geeft de ingestelde vertraging
unsigned int Septillion_LM298::getDelay(){
	return this->delay;
}

//Triggert de noodstop
void Septillion_LM298::emergencyStop(){
	//Door beide in-pinnen laag te maken en de PWM pin hoog is er een "Fast Motor Stop"
	digitalWrite(this->pinIn1, LOW);
	digitalWrite(this->pinIn2, LOW);
	digitalWrite(this->pinPwm, HIGH);
	
	//En even de snelheden updaten
	setSpeed = 0;
	curSpeed = 0;
}

// Volgende functies erin gehouden omdat deze er waren

//Gewoon starten met snelheid 120 met mogelijkheid om richting op te geven
//Houdt rekening met de ingestelde vertraging
void Septillion_LM298::start(byte direction){
	if(direction){
		this->speed(120);
	}
	else{
		this->speed(-120);
	}
}

//Stop de treinen
//Houdt rekening met ingestelde vertraging
void Septillion_LM298::stop(){
	this->speed(0);
}