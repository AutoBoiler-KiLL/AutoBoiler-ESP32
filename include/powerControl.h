#ifndef POWER_CONTROL_H 
#define POWER_CONTROL_H

#include <Arduino.h>

const int zeroCrossPin = 2;
const int triacPin = 3;
int delayTime = 0;

void triacTriger() {
    digitalWrite(triacPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(triacPin, LOW);
}

void zeroCrossISR() {
    delayMicroseconds(delayTime);
    triacTriger();
}

void setupPowerControl() {
    pinMode(triacPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(zeroCrossPin), zeroCrossISR, RISING);
}

#endif 
