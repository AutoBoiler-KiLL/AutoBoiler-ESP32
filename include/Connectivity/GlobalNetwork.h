#ifndef GLOBAL_NETWORK_H
#define GLOBAL_NETWORK_H

#include <WebSocketsClient.h>
#include <WiFi.h>

#include "LocalNetwork.h"
#include "Memory.h"

const uint16_t WIFI_TIMEOUT = 5000;
const String serverURL = "smartboiler-server.onrender.com";

volatile bool wifiConnected = false;
unsigned long lastWiFiAttempt = 0;
const unsigned long WIFI_RETRY_INTERVAL = 5000;

WebSocketsClient webSocket;
void setupWebSocket();

/// @brief Connects to WiFi using credentials stored in memory
void startWiFiConnect() {
    String ssid = memorySSID();
    String password = memoryPassword();
    Serial.println("[GlobalNetwork] Connecting to WiFi network: " + ssid);
    WiFi.begin(ssid, password);
    lastWiFiAttempt = millis();
}

/// @brief Handles WiFi connection and disconnection events
void onWiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("[GlobalNetwork] WiFi connected! IP address: " + WiFi.localIP());
            wifiConnected = true;
            stopWiFiAccessPoint();
            stopLocalServer();
            setupWebSocket();
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("[GlobalNetwork] WiFi disconnected, enabling SoftAP and local server");
            wifiConnected = false;
            initializeWiFiAccessPoint();
            startLocalServer();
            break;
        default:
            break;
    }
}

/// @brief Tries to reconnect to WiFi if it is not connected
void tryReconnectWiFi() {
    if (verifyMemory() && !wifiConnected && millis() - lastWiFiAttempt > WIFI_RETRY_INTERVAL) {
        lastWiFiAttempt = millis();
        Serial.println("[GlobalNetwork] Attempting to reconnect to WiFi...");
        startWiFiConnect();
    }
}

/// @brief Handles WebSocket events
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            Serial.println("[GlobalNetwork] Connected to server, stopping local server");
            stopLocalServer();
            break;
        
        case WStype_DISCONNECTED:
            Serial.println("[GlobalNetwork] Disconnected from server, starting local server");
            startLocalServer();
            break;
        
        case WStype_TEXT:
            String command = String((char*)payload);
            JsonDocument document;
            
            DeserializationError error = deserializeJson(document, command);
            if (error) {
                Serial.println("[GlobalNetwork] Error parsing command: " + String(error.c_str()));
                return;
            }

            if (!verifyRequest(document)) return;

            handleCommand(document, nullptr, &webSocket);
            break;
    }
}

/// @brief Connects to the server using WebSocket
void setupWebSocket() {
    webSocket.beginSSL(serverURL, 443, "/socket");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(500);
}

#endif