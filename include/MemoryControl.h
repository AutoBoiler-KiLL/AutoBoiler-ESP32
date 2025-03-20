#ifndef MEMORY_CONTROL_H
#define MEMORY_CONTROL_H
#include <EEPROM.h>

/// @brief Defines the size of the EEPROM
const int EEPROM_SIZE = 200; 
/// @brief Defines the address where the SSID and password will be stored
const int SSID_ADDR = 0;
const int PASS_ADDR = 100;

String ssidSaved = "";
String passwordSaved = "";

/// @brief Reads the SSID and password from the EEPROM
/// @return True if the SSID and password were read successfully, false otherwise
bool readMemory() {
    EEPROM.begin(EEPROM_SIZE);

    ssidSaved = EEPROM.readString(SSID_ADDR);
    passwordSaved = EEPROM.readString(PASS_ADDR);

    // LIMPIAR EEPROM
    // for (int i = 0; i < EEPROM_SIZE; i++) {
    //     EEPROM.write(i, 0xFF);
    // }
    EEPROM.commit();
   
    return (ssidSaved.length() != 0) && (passwordSaved.length() != 0);
}

/// @brief Wrties the SSID and password to the EEPROM
/// @return True if the SSID and password were written successfully, false otherwise
bool writeMemory() {
    EEPROM.writeString(SSID_ADDR, ssidSaved);
    EEPROM.writeString(PASS_ADDR, passwordSaved);
    EEPROM.commit();
    return (ssidSaved.length() != 0) && (passwordSaved.length() != 0);
}

#endif
