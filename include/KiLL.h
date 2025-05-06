#ifndef KILL_H
#define KILL_H

#include "Memory.h"

const int MINIMUM_TEMPERATURE = 25;
const int MAXIMUM_TEMPERATURE = 45;

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
    // Reset the ESP32 to factory settings
    Serial.println("[ESP32] Resetting to factory settings...");
    clearMemory();
    delay(3000);
    ESP.restart();
}

#endif