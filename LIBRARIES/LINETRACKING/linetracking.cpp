#include "linetracking.h"
#import <Arduino.h>

static int pinSL;
static int pinSM;
static int pinSR;

void initLineTracking(int pinSensorLeft, int pinSensorMiddle, int pinSensorRight){
  pinMode(pinSensorLeft,INPUT);
  pinMode(pinSensorMiddle,INPUT);
  pinMode(pinSensorRight,INPUT);
  pinSL = pinSensorLeft;
  pinSM = pinSensorMiddle;
  pinSR = pinSensorRight;
}


//QUAN ESTA EN LINIA NEGRE RETORNA 3.3 VOLTS QUAN ES TROBA EN BLANC RETORNA 0
bool reflexionLight(char sensor){
  int pin;
  if(sensor == 'L'){
    pin = pinSL;
  } else if(sensor == 'M') {
    pin = pinSM;
  } else if(sensor == 'R') {
    pin = pinSR;
  }

  if (digitalRead(pin) == LOW) {
    return true;
  }
  return false;
}
