#ifndef KILL_H
#define KILL_H

#include <Arduino.h>

class LocalNetwork;
class GlobalNetwork;
class Boiler;
class Display;

class KiLL {
public:
    KiLL();
    ~KiLL();

    void setup();
    
    /// @brief Get the ESP32's unique identifier (given in the Espressif factory).
    /// This will always be the same for a given ESP32, no matter how many times it is flashed.
    /// @return A String containing the ESP32's unique ID. Example: "5CCF7F0D3A4A"
    static const String espId();

    void checkForFactoryReset();

    /// @brief Resets the ESP32 to factory settings, clearing the EEPROM and restarting.
    static void resetToFactorySettings();

    void keepServersAlive();
    void tryToReconnectToWifi();

    static constexpr uint8_t MAXIMUM_TEMPERATURE = 50;

    void checkUserInteraction();

    void controlTemperature();

private:
    static constexpr uint8_t FACTORY_RESET_PIN = 10;
    static constexpr uint16_t FACTORY_RESET_HOLD_TIME = 10000;

    static constexpr uint8_t PIN_INCREASE_TARGET_TEMPERATURE = 15;
    static constexpr uint8_t PIN_DECREASE_TARGET_TEMPERATURE = 4;

    bool factoryButtonPressed;
    unsigned long factoryButtonPressedTime;

    LocalNetwork* localNetwork;
    GlobalNetwork* globalNetwork;
    Boiler* boiler;
    Display* display;
};

#endif