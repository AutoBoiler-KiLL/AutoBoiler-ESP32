#ifndef MEMORY_H
#define MEMORY_H

#include <EEPROM.h>

/// @brief Defines the size of the EEPROM
const uint8_t EEPROM_SIZE = 250; 

/// @brief Defines the address where the SSID will be stored
const uint8_t SSID_ADDRESS = 0;
/// @brief Defines the address where the password will be stored
const uint8_t PASS_ADDRESS = 75;
/// @brief Defines the address where the app id will be stored
const uint8_t APP_ID_ADDRESS = 150;
/// @brief Defines the address where the temperature will be stored
const uint8_t TEMPERATURE_ADDRESS = 200;

/// @brief Returns the SSID stored in the EEPROM
String memorySSID() {
    return EEPROM.readString(SSID_ADDRESS);
}

/// @brief Returns the password stored in the EEPROM
String memoryPassword() {
    return EEPROM.readString(PASS_ADDRESS);
}

/// @brief Returns the app id stored in the EEPROM
String memoryAppId() {
    return EEPROM.readString(APP_ID_ADDRESS);
}

/// @brief Returns the temperature in Celsius stored in the EEPROM
int memoryTemperature() {
    return EEPROM.readInt(TEMPERATURE_ADDRESS);
}

/// @brief Initializes the EEPROM with the defined size
void initMemory() {
    EEPROM.begin(EEPROM_SIZE);
}

/// @brief Checks if the EEPROM has saved data
/// @return False if the EEPROM is empty, true otherwise
bool verifyMemory() {
    return  EEPROM.readString(SSID_ADDRESS).length() != 0 && 
            EEPROM.readString(PASS_ADDRESS).length() != 0 &&
            EEPROM.readString(APP_ID_ADDRESS).length() != 0;
}

/// @brief Writes the SSID, password and app id to the EEPROM
void writeMemory(String ssid, String password, String appId) {
    EEPROM.writeString(SSID_ADDRESS, ssid);
    EEPROM.writeString(PASS_ADDRESS, password);
    EEPROM.writeString(APP_ID_ADDRESS, appId);
    EEPROM.commit();
    Serial.println("[EEPROM] SSID, password and app id saved.");
}

/// @brief Writes the temperature in Celsius to the EEPROM
void writeTemperature(int temperature) {
    EEPROM.writeInt(TEMPERATURE_ADDRESS, temperature);
    EEPROM.commit();
    Serial.println("[EEPROM] Temperature saved.");
}

/// @brief Removes everything from the EEPROM
void clearMemory() {
    EEPROM.writeString(SSID_ADDRESS, "");
    EEPROM.writeString(PASS_ADDRESS, "");
    EEPROM.writeString(APP_ID_ADDRESS, "");
    EEPROM.writeInt(TEMPERATURE_ADDRESS, 0);
    EEPROM.commit();
    Serial.println("[EEPROM] Factory settings restored.");
}

#endif
