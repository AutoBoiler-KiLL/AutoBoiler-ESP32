#ifndef LOCAL_NETWORK_H
#define LOCAL_NETWORK_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include "KiLL.h"

const int MAX_MDNS_RETRIES = 10;
const int HTTP_PORT = 80;

/// @brief Global local web server instance
WebServer localServer(HTTP_PORT);

/// @brief Local network SSID
const String SSID() {
    return "KiLL-" + espId();
}

/// @brief The URL to access the ESP32's web server on the local network. 
/// Example: "http://kill-5CCF7F0D3A4A.local/"
String getLocalNetworkHostname() {
    return "http://kill-" + espId() + ".local/";
}

/// @brief Tries to set up the ESP32 to be accessible on the local network.
void setupLocalNetwork() {
    Serial.print("[LocalNetwork] Setting up local network");
    String hostname = SSID();
    
    int retryAttempt = 0;
    while (!MDNS.begin(hostname) && retryAttempt <= MAX_MDNS_RETRIES) {
        Serial.print(".");
        delay(1000);
        retryAttempt += 1;
    }

    if (retryAttempt >= MAX_MDNS_RETRIES) Serial.println("\n[LocalNetwork] Error setting up MDNS responder!");
    else Serial.println("\n[LocalNetwork] mDNS responder started");
}

/// @brief Sets up the local web server to handle the setup request.
void setupLocalServer() {
    Serial.println("[LocalNetwork] Setting up local server");

    // Root to setup the KiLL
    localServer.on("/setup", HTTP_POST, []() {
        if (localServer.hasArg("plain")) {
            String json = localServer.arg("plain");
            StaticJsonDocument<256> document;
            DeserializationError error = deserializeJson(document, json);

            if (error) {
                Serial.println("[LocalNetwork] Error parsing JSON");
                localServer.send(400, "application/json", "{\"error\": \"Invalid Data\"}");
                return;
            }

            String ssid = document["ssid"] | "";
            String password = document["password"] | "";
            String appId = document["appId"] | "";

            if (ssid.length() == 0 || password.length() == 0 || appId.length() == 0) {
                Serial.println("[LocalNetwork] Error: Missing data");
                localServer.send(400, "application/json", "{\"error\": \"Missing Data\"}");
                return;
            }

            Serial.println("[LocalNetwork] Received data: SSID: " + ssid + ", Password: " + password + ", App ID: " + appId);

            localServer.send(200, "application/json", "{\"status\": \"OK\"}");
            writeMemory(ssid, password, appId);
        } else {
            Serial.println("[LocalNetwork] Error: No data");
            localServer.send(400, "application/json", "{\"error\": \"No Data\"}");
        }
    });

    localServer.on("kill_reset_factory", HTTP_GET, []() {
        Serial.println("[LocalNetwork] Resetting to factory settings");
        localServer.send(200, "application/json", "{\"status\": \"OK\"}");
        resetToFactorySettings();
    });

    localServer.on("/", HTTP_GET, []() {
        localServer.send(200, "text/plain", "KiLL");
    });

    localServer.begin();
    Serial.println("[LocalNetwork] Local server started");
}

#endif