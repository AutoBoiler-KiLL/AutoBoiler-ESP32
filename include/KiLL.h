#ifndef KILL_H
#define KILL_H

#include "Memory.h"

const uint8_t MINIMUM_TEMPERATURE = 25;
const uint8_t MAXIMUM_TEMPERATURE = 45;

const uint8_t FACTORY_RESET_PIN = 10;
const int FACTORY_RESET_TIMEOUT = 10000; 

bool factoryButtonPressed = false;
unsigned long factoryButtonPressedTime = 0;

/// @brief Get the ESP32's unique identifier (given in the Espressif factory).
/// This will always be the same for a given ESP32, no matter how many times it is flashed.
/// @return A String containing the ESP32's unique ID. Example: "5CCF7F0D3A4A"
String espId() {
    uint64_t chipId = ESP.getEfuseMac();
    char buffer[13];
    sprintf(buffer, "%04X%08X", (uint16_t) (chipId >> 32), (uint32_t) chipId);
    return String(buffer);
}

/// @brief Resets the ESP32 to factory settings.
/// This will clear the EEPROM and restart the ESP32.
void resetToFactorySettings() {
    Serial.println("[ESP32] Resetting to factory settings...");
    clearMemory();
    delay(3000);
    ESP.restart();
}

void checkForFactoryReset() {
    bool buttonPressed = digitalRead(FACTORY_RESET_PIN) == LOW;

    // Button pressed for the first time
    if (buttonPressed && !factoryButtonPressed) {
        factoryButtonPressed = true;
        factoryButtonPressedTime = millis();
    } 
    
    // Button released before timeout
    if (!buttonPressed && factoryButtonPressed) {
        factoryButtonPressed = false;
        if (millis() - factoryButtonPressedTime >= FACTORY_RESET_TIMEOUT) {
            resetToFactorySettings();
        }
    }

    // Button pressed for timeout, reset to factory settings
    if (buttonPressed && millis() - factoryButtonPressedTime >= FACTORY_RESET_TIMEOUT) {
        Serial.println("[ESP32] Factory reset button pressed for too long. Resetting to factory settings...");
        resetToFactorySettings();
    }
}

#endif