#ifndef MEMORY_H
#define MEMORY_H

#include <EEPROM.h>

/// @brief Defines the size of the EEPROM
const int EEPROM_SIZE = 250; 

/// @brief Defines the address where the SSID will be stored
const int SSID_ADDRESS = 0;
/// @brief Defines the address where the password will be stored
const int PASS_ADDRESS = 75;
/// @brief Defines the address where the app id will be stored
const int APP_ID_ADDRESS = 150;
/// @brief Defines the address where the temperature will be stored
const int TEMPERATURE_ADDRESS = 200;

String ssidSaved = "";
String passwordSaved = "";

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

/// @brief Removes everything from the EEPROM
void resetToFactorySettings() {
    EEPROM.writeString(SSID_ADDRESS, "");
    EEPROM.writeString(PASS_ADDRESS, "");
    EEPROM.writeString(APP_ID_ADDRESS, "");
    EEPROM.commit();
    Serial.println("[EEPROM] Factory settings restored.");
}

#endif
