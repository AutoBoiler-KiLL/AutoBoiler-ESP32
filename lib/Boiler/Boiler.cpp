#include "Boiler.h"
#include "Memory.h"
#include "TemperatureSensor.h"

Boiler::Boiler() {
    temperatureSensor = new TemperatureSensor();
    currentTemperature = 0;
    isOn = true;
}

void Boiler::begin() {
    temperatureSensor->begin();
    currentTargetTemperature = Memory::getTemperature();
    currentTemperature = temperatureSensor->readTemperature(0);
}

void Boiler::setTargetTemperature(int temperature) {
    Memory::writeTemperature(temperature);
    currentTargetTemperature = temperature;
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
    return currentTemperature;
}

bool Boiler::getIsOn() {
    return isOn;
}

int Boiler::getTargetTemperature() {
    return currentTargetTemperature;
}

double Boiler::controlTemperature() {
    currentTemperature = temperatureSensor->readTemperature(0);
    // TODO: Control temperature using zero-cross detection
    return currentTemperature;
}
