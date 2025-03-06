#ifndef POWER_CONTROL_H 
#define POWER_CONTROL_H

#include <Arduino.h>

const int zeroCrossPin = 2;
const int triacPin = 3;
int delayTime;

void triac_triger(){
    digitalWrite(triacPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(triacPin, LOW);
}

void zeroCrossISR () {
    delayMicroseconds(delayTime);
    triac_triger();
}

void setup_power_control(){
    pinMode(triacPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(zeroCrossPin), zeroCrossISR, RISING);
}

#endif 
