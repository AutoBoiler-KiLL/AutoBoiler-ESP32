#include <Arduino.h>
#include "Boiler.h"
#include "Memory.h"
#include "TemperatureSensor.h"

Boiler* Boiler::instance = nullptr;

Boiler::Boiler() {

    temperatureSensor = new TemperatureSensor();
    currentTemperature = 0;
    isOn = true;
}

void IRAM_ATTR Boiler::zeroCrossISR() {
    if (!instance) return;

    // Enciende si estamos dentro de los ciclos deseados
    if (instance->zeroCrossCounter < instance->desiredActiveCycles) {
        digitalWrite(SSR_ACTIVAION_PIN, HIGH);
    } else {
        digitalWrite(SSR_ACTIVAION_PIN, LOW);
    }

    // Incrementar y reiniciar el ciclo cada 10 cruces
    instance->zeroCrossCounter++;
    if (instance->zeroCrossCounter >= Boiler::totalCycles) {
        instance->zeroCrossCounter = 0;
    }
}

void Boiler::setPowerPercent(uint8_t percent) {
    if (percent > 100) percent = 100;
    desiredActiveCycles = (percent * totalCycles) / 100;  
}

void Boiler::begin() {
    pinMode(SSR_ACTIVAION_PIN, OUTPUT);
    temperatureSensor->begin();
    currentTargetTemperature = Memory::getTemperature();
    currentTemperature = temperatureSensor->readTemperature(0);
    instance = this;
    attachInterrupt(digitalPinToInterrupt(ZERO_CROSS_DETECTION_PIN), Boiler::zeroCrossISR, RISING);
}


void Boiler::setTargetTemperature(int temperature) {
    setPowerPercent(map(temperature, 23,50, 0, 100));
    currentTargetTemperature = temperature;
}

void Boiler::turnOn() {
    Serial.println("[Boiler] Turning on");
    isOn = true;
}

void Boiler::turnOff() {
    Memory::writeTemperature(currentTargetTemperature);
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
