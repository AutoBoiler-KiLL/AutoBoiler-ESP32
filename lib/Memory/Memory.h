#ifndef MEMORY_H
#define MEMORY_H

#include <EEPROM.h>

class Memory {
public:
    /// @brief Defines the size of the EEPROM
    static constexpr uint8_t EEPROM_SIZE = 250;
    /// @brief Defines the address where the SSID will be stored
    static constexpr uint8_t SSID_ADDRESS = 0;
    /// @brief Defines the address where the password will be stored
    static constexpr uint8_t PASS_ADDRESS = 75;
    /// @brief Defines the address where the app id will be stored
    static constexpr uint8_t APP_ID_ADDRESS = 150;
    /// @brief Defines the address where the temperature will be stored
    static constexpr uint8_t TEMPERATURE_ADDRESS = 200;

    /// @brief Initializes the EEPROM
    static void initialize();
    /// @brief Verifies if the memory content is valid
    static bool verifyMemoryContent();

    /// @brief Gets the SSID from the EEPROM
    static String getSSID();
    /// @brief Gets the password from the EEPROM
    static String getPassword();
    /// @brief Gets the app id from the EEPROM
    static String getAppId();
    /// @brief Gets the temperature from the EEPROM
    static int getTemperature();

    /// @brief Writes the SSID, password and app id to the EEPROM
    static void writeMemory(String ssid, String password, String appId);
    /// @brief Writes the temperature to the EEPROM
    static void writeTemperature(int temperature);

    /// @brief Removes everything from the EEPROM
    static void clearMemory();
};

#endif
