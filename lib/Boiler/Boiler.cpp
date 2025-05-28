#include "Boiler.h"
#include "Memory.h"

Boiler::Boiler() {
    currentTemperature = 25;
    isOn = true;
}

void Boiler::setTargetTemperature(int temperature) {
    Memory::writeTemperature(temperature);
}

double Boiler::getCurrentTemperature() {
    return currentTemperature;
}

bool Boiler::getIsOn() {
    return isOn;
}