#include <Arduino.h>
#include "Boiler.h"
#include "Memory.h"
#include "TemperatureSensor.h"

Boiler* Boiler::instance = nullptr;

Boiler::Boiler() {
    temperatureSensor = new TemperatureSensor();
    currentTemperature = 0;
    isOn = true;
    lastMinTempUpdate = 0;
    shouldUpdateMinTemp = true;
    minimumTemperature = 0;
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
    
    lastMinTempUpdate = millis();
    shouldUpdateMinTemp = true;
    
    instance = this;
    attachInterrupt(digitalPinToInterrupt(ZERO_CROSS_DETECTION_PIN), Boiler::zeroCrossISR, RISING);
}


void Boiler::setTargetTemperature(int temperature) {
    setPowerPercent(map(temperature, 23, 50, 0, 100));
    currentTargetTemperature = temperature;
}

void Boiler::turnOn() {
    Serial.println("[Boiler] Turning on");
    isOn = true;
    shouldUpdateMinTemp = false;
}

void Boiler::turnOff() {
    Memory::writeTemperature(currentTargetTemperature);
    Serial.println("[Boiler] Turning off");
    isOn = false;
    shouldUpdateMinTemp = true;
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

int Boiler::getMinimumTemperature() {
    return minimumTemperature;
}

void Boiler::updateMinimumTemperature() {
    if (!shouldUpdateMinTemp) return;
    
    unsigned long currentTime = millis();
    
    bool shouldUpdate = false;
    
    // Check if this is the first time
    if (minimumTemperature == 0) {
        shouldUpdate = true;
        Serial.println("[Boiler] Initial minimum temperature update on startup");
    } else if (lastMinTempUpdate > 0 && (currentTime - lastMinTempUpdate >= MIN_TEMP_UPDATE_INTERVAL)) {
        // Periodic update every MIN_TEMP_UPDATE_INTERVAL
        shouldUpdate = true;
    }
    
    if (shouldUpdate) {
        lastMinTempUpdate = currentTime;
        minimumTemperature = round(currentTemperature + 2);
        Serial.println("[Boiler] Minimum temperature updated: " + String(minimumTemperature) + "°C");
    }
}

double Boiler::controlTemperature() {
    currentTemperature = temperatureSensor->readTemperature(0);
    updateMinimumTemperature();
    
    // TODO: Control temperature using zero-cross detection

    return currentTemperature;
}
