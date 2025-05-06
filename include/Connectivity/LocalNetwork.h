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

bool verifyRequest(StaticJsonDocument<256> document) {
    String receivedEspId = document["espId"] | "";
    String appId = document["appId"] | "";

    if (receivedEspId != espId() || appId != memoryAppId()) {
        Serial.println("[LocalNetwork] Error: Invalid espId or appId. ESP ID: " + receivedEspId + ", App ID: " + appId);
        return false;
    }
    return true;
}

/// @brief Sets up the local web server to handle the setup request.
void setupLocalServer() {
    Serial.println("[LocalNetwork] Setting up local server");

    // Root to check if the ESP32 is reachable
    localServer.on("/", HTTP_GET, []() {
        localServer.send(200, "text/plain", "KiLL");
    });

    // Root for not found
    localServer.onNotFound([]() {
        Serial.println("[LocalNetwork] Error: Not found");
        localServer.send(404, "text/plain", "Not found");
    });

    // Root to setup the KiLL
    localServer.on("/setup", HTTP_POST, []() {
        if (!localServer.hasArg("plain")) {
            Serial.println("[LocalNetwork] Error: No data on setup");
            localServer.send(400, "application/json", "{\"error\": \"No Data\"}");
            return;
        }

        String json = localServer.arg("plain");
        StaticJsonDocument<256> document;
        DeserializationError error = deserializeJson(document, json);

        if (error) {
            Serial.println("[LocalNetwork] Error parsing data on setup: " + String(error.c_str()));
            localServer.send(400, "application/json", "{\"error\": \"Invalid Data\"}");
            return;
        }

        String ssid = document["ssid"] | "";
        String password = document["password"] | "";
        String appId = document["appId"] | "";

        if (ssid.length() == 0 || password.length() == 0 || appId.length() == 0) {
            Serial.println("[LocalNetwork] Error: Missing data on setup. SSID: " + ssid + ", Password: " + password + ", App ID: " + appId);
            localServer.send(400, "application/json", "{\"error\": \"Missing Data\"}");
            return;
        }

        Serial.println("[LocalNetwork] Received data on setup: SSID: " + ssid + ", Password: " + password + ", App ID: " + appId);

        localServer.send(200, "application/json", "{\"status\": \"OK\"}");
        writeMemory(ssid, password, appId);
    });

    // Root to reset the ESP32 to factory settings, checking espId and appId are correct
    localServer.on("kill_reset_factory", HTTP_POST, []() {
        if (!localServer.hasArg("plain")) {
            Serial.println("[LocalNetwork] Error: No data on reset");
            localServer.send(400, "application/json", "{\"error\": \"No Data\"}");
            return;
        }

        String json = localServer.arg("plain");
        StaticJsonDocument<256> document;
        DeserializationError error = deserializeJson(document, json);

        if (error) {
            Serial.println("[LocalNetwork] Error parsing data on reset: " + String(error.c_str()));
            localServer.send(400, "application/json", "{\"error\": \"Invalid Data\"}");
            return;
        }

        if (verifyRequest(document)) {
            localServer.send(200, "application/json", "{\"status\": \"OK\"}");
            resetToFactorySettings();
        } else {
            localServer.send(400, "application/json", "{\"error\": \"Missing authentication\"}");
        }  
    });

    // Root to manage commands for turning on/off and setting the temperature
    localServer.on("/command", HTTP_POST, []() {
        if (!localServer.hasArg("plain")) {
            Serial.println("[LocalNetwork] Error: No data on command");
            localServer.send(400, "application/json", "{\"error\": \"No Data\"}");
            return;
        }

        String json = localServer.arg("plain");
        StaticJsonDocument<256> document;
        DeserializationError error = deserializeJson(document, json);

        if (error) {
            Serial.println("[LocalNetwork] Error parsing data on command: " + String(error.c_str()));
            localServer.send(400, "application/json", "{\"error\": \"Invalid Data\"}");
            return;
        }

        if (!verifyRequest(document)) {
            localServer.send(400, "application/json", "{\"error\": \"Missing authentication\"}");
            return;
        }

        String command = document["command"] | "";

        if (command == "turn_on") {
            // TODO: Implement turn on command
            // turnOff();
            Serial.println("[LocalNetwork] Command: Turn on");
            localServer.send(200, "application/json", "{\"status\": \"OK\"}");
        } else if (command == "turn_off") {
            // TODO: Implement turn off command
            Serial.println("[LocalNetwork] Command: Turn off");
            localServer.send(200, "application/json", "{\"status\": \"OK\"}");
        } else if (command == "set_temperature") {
            int temperature = document["temperature"] | 0;
            if (temperature < MINIMUM_TEMPERATURE || temperature > MAXIMUM_TEMPERATURE) {
                Serial.println("[LocalNetwork] Error: Invalid temperature value");
                localServer.send(400, "application/json", "{\"error\": \"Invalid Temperature\"}");
                return;
            }
            
            // TODO: Implement set temperature command
            // setTemperature(temperature);
            
            Serial.println("[LocalNetwork] Command: Set temperature to " + String(temperature));
            writeTemperature(temperature);
        } else {
            Serial.println("[LocalNetwork] Error: Invalid command");
            localServer.send(400, "application/json", "{\"error\": \"Invalid Command\"}");
            return;
        }
    });

    localServer.begin();
    Serial.println("[LocalNetwork] Local server started");
}

#endif