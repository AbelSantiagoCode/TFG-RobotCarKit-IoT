#include "ultrasound.h"
#import <Arduino.h>

#define CM_US 58.0

static int triggerSignal;
static int echoSignal;

void initUltSound(int Echo, int Trig){
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  triggerSignal = Trig;
  echoSignal = Echo;
}

float distanceCMUltSound(void){
  digitalWrite(triggerSignal,LOW);
  delayMicroseconds(2);
  digitalWrite(triggerSignal, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerSignal, LOW);
  float distanceCM = pulseIn(echoSignal, HIGH);
  distanceCM = distanceCM/CM_US;
  return distanceCM;
}

float distanceUltSound(void){
  return (distanceCMUltSound()/100.0);
}
