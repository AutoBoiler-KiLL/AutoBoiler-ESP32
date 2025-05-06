#ifndef KILL_H
#define KILL_H

#include "Memory.h"

const uint8_t MINIMUM_TEMPERATURE = 25;
const uint8_t MAXIMUM_TEMPERATURE = 45;

const uint8_t FACTORY_RESET_PIN = 10;
const uint16_t FACTORY_RESET_HOLD_TIME = 10000; 

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
    Serial.println("[KiLL] Resetting to factory settings...");
    clearMemory();
    delay(3000);
    ESP.restart();
}

/// @brief Checks if the factory reset button has been pressed for `FACTORY_RESET_HOLD_TIME` milliseconds.
void checkForFactoryReset() {
    bool isButtonNowPressed = digitalRead(FACTORY_RESET_PIN) == LOW;

    // Button pressed for the first time
    if (isButtonNowPressed && !factoryButtonPressed) {
        factoryButtonPressed = true;
        factoryButtonPressedTime = millis();
    } 
    
    // Button pressed for FACTORY_RESET_HOLD_TIME, reset to factory settings
    if (isButtonNowPressed && millis() - factoryButtonPressedTime >= FACTORY_RESET_HOLD_TIME) {
        Serial.println("[KiLL] Factory reset button pressed for too long. Resetting to factory settings...");
        resetToFactorySettings();
    }

    // Button released before timeout
    if (!isButtonNowPressed && factoryButtonPressed) {
        factoryButtonPressed = false;
        factoryButtonPressedTime = 0;
    }
}

#endif