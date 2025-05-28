#include "Boiler.h"
#include "Memory.h"

Boiler::Boiler() {
    currentTemperature = 25;
    isOn = true;
}

void Boiler::setTargetTemperature(int temperature) {
    Memory::writeTemperature(temperature);
}

void Boiler::turnOn() {
    Serial.println("[Boiler] Turning on");
    isOn = true;
}

void Boiler::turnOff() {
    Serial.println("[Boiler] Turning off");
    isOn = false;
}

double Boiler::getCurrentTemperature() {
    currentTemperature = random(25000, 50000) / 1000.0;
    return currentTemperature;
}

bool Boiler::getIsOn() {
    return isOn;
}