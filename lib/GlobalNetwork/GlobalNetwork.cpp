#include <Memory.h>
#include <GlobalNetwork.h>
#include <Utils.h>

GlobalNetwork::GlobalNetwork() {
    lastWiFiAttempt = 0;
    wifiConnected = false;

    WiFi.onEvent(std::bind(&GlobalNetwork::onWiFiEvent, this, std::placeholders::_1), WiFiEvent_t::ARDUINO_EVENT_MAX);
}

void GlobalNetwork::startWiFiConnection() {
    String ssid = Memory::getSSID();
    String password = Memory::getPassword();
    Serial.println("[GlobalNetwork] Connecting to WiFi network: " + ssid);
    WiFi.begin(ssid, password);
    lastWiFiAttempt = millis();
}

void GlobalNetwork::tryReconnectWiFi() {
    if (Memory::verifyContent() && !wifiConnected && millis() - lastWiFiAttempt > WIFI_RETRY_INTERVAL) {
        lastWiFiAttempt = millis();
        Serial.println("[GlobalNetwork] Attempting to reconnect to WiFi...");
        startWiFiConnection();
    }
}

void GlobalNetwork::onWiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("[GlobalNetwork] WiFi connected! IP Address: " + String(WiFi.localIP()) + " Connecting to server...");
            wifiConnected = true;
            // TODO: Stop SoftAP
            // TODO: Stop local server
            connectWebSocket();
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("[GlobalNetwork] WiFi disconnected, enabling SoftAP and local server");
            wifiConnected = false;
            // TODO: Start SoftAP
            // TODO: Start local server
            break;
        default:
            break;
    }
}

void GlobalNetwork::connectWebSocket() {
    webSocket.beginSSL(serverURL, 443, "/socket");
    webSocket.onEvent(std::bind(&GlobalNetwork::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    webSocket.setReconnectInterval(500);
}

void GlobalNetwork::webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            Serial.println("[GlobalNetwork] Connected to server, stopping local server");
            // TODO: Stop local server
            break;
        
        case WStype_DISCONNECTED:
            Serial.println("[GlobalNetwork] Disconnected from server, starting local server");
            // TODO: Start local server
            break;
        
        case WStype_TEXT:
            String command = String((char*)payload);
            JsonDocument document;
            DeserializationError error = deserializeJson(document, command);
            if (error) {
                Serial.println("[GlobalNetwork] Error parsing command: " + String(error.c_str()));
                return;
            }

            if (!Utils::verifyRequest(document)) return;

            // TODO: Handle command
            break;
    }
}