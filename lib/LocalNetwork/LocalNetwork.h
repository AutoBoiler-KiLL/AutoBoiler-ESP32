#ifndef LOCAL_NETWORK_H
#define LOCAL_NETWORK_H

#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WebServer.h>

#include "KiLL.h"
#include "Utils.h"

class LocalNetwork {
public:
    LocalNetwork();

    void initialize();
    void stopAccessPoint();

    void setupServer();
    void startServer();
    void stopServer();

    void setupLocalNetwork();

    const String getHostname();

private:
    /// @brief Maximum number of MDNS retries
    static constexpr uint8_t MAX_MDNS_RETRIES = 10;
    /// @brief HTTP port
    static constexpr uint8_t HTTP_PORT = 80;
    
    /// @brief Local web server instance
    WebServer server;

    /// @brief Local network SSID
    const String SSID();
    
    /// MARK: Routes

    bool checkRequestData(JsonDocument& document);

    /// @brief Root to check if the ESP32 is reachable
    void handleRoot();
    /// @brief Root for not found
    void handleNotFound();
    /// @brief Root to setup the KiLL
    void handleSetup();
    /// @brief Root to reset the ESP32 to factory settings
    void handleResetFactory();
    /// @brief Root to manage commands for turning on/off and setting the temperature
    void handleCommand();
};

#endif