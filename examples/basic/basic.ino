#include <Septillion_LM298.h>

Septillion_LM298 myLM298(10, 2, 3);

unsigned int millisLast;

void setup() {
  millisLast = 0;
  myLM298.setDelay(40); //Full speed in 10 seconds

}

void loop() {
  unsigned int millisNow = millis();
  if(millisNow - millisLast > 55000){
    millisLast = millis();
  }
  else if(millisNow - millisLast > 45000){
    myLM298.stop();
  }
  else if(millisNow - millisLast > 20000){
    myLM298.speed(-150);
  }
  else if(millisNow - millisLast > 10000){
    myLM298.speed(150);
  }
  else if(millisNow - millisLast > 0){
    myLM298.speed(50);
  }
  
  
  myLM298.update();
}
