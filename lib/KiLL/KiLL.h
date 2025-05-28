#ifndef KILL_H
#define KILL_H

#include <Arduino.h>

class LocalNetwork;
class GlobalNetwork;
class Boiler;

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

private:
    static constexpr uint8_t MINIMUM_TEMPERATURE = 25;
    static constexpr uint8_t MAXIMUM_TEMPERATURE = 45;

    static constexpr uint8_t FACTORY_RESET_PIN = 10;
    static constexpr uint16_t FACTORY_RESET_HOLD_TIME = 10000;

    bool factoryButtonPressed;
    unsigned long factoryButtonPressedTime;

    LocalNetwork* localNetwork;
    GlobalNetwork* globalNetwork;
    Boiler* boiler;
};

#endif

/*

#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WebServer.h>

/// @brief Handles the command received from the local network or the web socket.
void handleCommand(JsonDocument& document, WebServer* localServer = nullptr, WebSocketsClient* webSocket = nullptr) {
    String command = document["command"] | "";
    String source = localServer ? "[LocalNetwork]" : "[GlobalNetwork]";
    String response;

    // TODO: Implement the real JSON response (see SmartBoiler-Server)
    // TODO: Implement the turn on, turn off, set temperature commands
    if (command == "turn_on") {
        Serial.println(source + " Command: Turn on");
        response = "{\"status\": \"OK\"}";
    } else if (command == "turn_off") {
        Serial.println(source + " Command: Turn off");
        response = "{\"status\": \"OK\"}";
    } else if (command == "set_temperature") {
        Serial.println(source + " Command: Set temperature");
        uint8_t temperature = document["temperature"] | 0;
        if (temperature < MINIMUM_TEMPERATURE || temperature > MAXIMUM_TEMPERATURE) {
            Serial.println(source + " Error: Invalid temperature value");
            response = "{\"error\": \"Invalid Temperature\"}";
        } else {
            Serial.println(source + " Command: Set temperature to " + String(temperature));
            writeTemperature(temperature);
            response = "{\"status\": \"OK\"}";
        }
    } else {
        Serial.println(source + " Error: Invalid command");
        response = "{\"error\": \"Invalid Command\"}";
    }

    if (localServer) {
        localServer->send(200, "application/json", response);
    } else if (webSocket) {
        webSocket->sendTXT(response);
    } else {
        Serial.println("Error: No local server or web socket available to handle the command.");
    }
}

#endif
*/