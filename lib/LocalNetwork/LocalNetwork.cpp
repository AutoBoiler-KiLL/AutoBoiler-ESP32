#include "LocalNetwork.h"

#include "Memory.h"
#include "KiLL.h"

LocalNetwork::LocalNetwork() : server(HTTP_PORT) {}

const String LocalNetwork::getHostname() {
    return "http://KiLL-" + KiLL::espId() + ".local/";
}

const String LocalNetwork::SSID() {
    return "KiLL-" + KiLL::espId();
}

void LocalNetwork::onStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("[LocalNetwork] Station connected: ");
    for (uint8_t i = 0; i < 6; i++) {
        Serial.printf("%02X", info.wifi_ap_staconnected.mac[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
}

void LocalNetwork::onStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.print("[LocalNetwork] Station disconnected: ");
    for (uint8_t i = 0; i < 6; i++) {
        Serial.printf("%02X", info.wifi_ap_stadisconnected.mac[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
}

void LocalNetwork::initialize() {
    WiFi.mode(WIFI_AP_STA);
    
    // Configure custom IP for soft AP
    IPAddress local_IP(192, 168, 39, 12);
    IPAddress gateway(192, 168, 39, 1);
    IPAddress subnet(255, 255, 255, 0);
    
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(SSID());
    
    Serial.println("[LocalNetwork] WiFi Access Point started");
    Serial.println("[LocalNetwork] IP Address: " + WiFi.softAPIP().toString());

    // Register Wi-Fi event handlers
    WiFi.onEvent(onStationConnected, ARDUINO_EVENT_WIFI_AP_STACONNECTED);
    WiFi.onEvent(onStationDisconnected, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
}

void LocalNetwork::stopAccessPoint() {
    WiFi.softAPdisconnect(true);
    Serial.println("[LocalNetwork] WiFi Access Point stopped");
}

void LocalNetwork::setupServer() {
    server.on("/", HTTP_GET, std::bind(&LocalNetwork::handleRoot, this));
    server.on("/local", HTTP_GET, std::bind(&LocalNetwork::handleLocal, this));
    server.onNotFound(std::bind(&LocalNetwork::handleNotFound, this));
    server.on("/setup", HTTP_POST, std::bind(&LocalNetwork::handleSetup, this));
    server.on("/kill_reset_factory", HTTP_POST, std::bind(&LocalNetwork::handleResetFactory, this));
    server.on("/command", HTTP_POST, std::bind(&LocalNetwork::handleCommand, this));

    startServer();
}

void LocalNetwork::startServer() {
    server.begin();
    Serial.println("[LocalNetwork] Local server started at " + getHostname());
}

void LocalNetwork::stopServer() {
    server.stop();
    Serial.println("[LocalNetwork] Local server stopped");
}

void LocalNetwork::setupLocalNetwork() {
    Serial.print("[LocalNetwork] Setting up local network");
    String hostname = SSID();
    
    uint8_t retryAttempt = 0;
    while (!MDNS.begin(hostname) && retryAttempt <= MAX_MDNS_RETRIES) {
        Serial.print(".");
        delay(1000);
        retryAttempt++;
    }

    if (retryAttempt >= MAX_MDNS_RETRIES) {
        Serial.println("\n[LocalNetwork] Error setting up MDNS responder! Restarting...");
        ESP.restart();
    } else {
        Serial.println("\n[LocalNetwork] mDNS responder started");
    }
}

void LocalNetwork::keepServerAlive() {
    server.handleClient();
}

// MARK: Routes

bool LocalNetwork::checkRequestData(JsonDocument& document, const String source) {
    if (!server.hasArg("plain")) {
        Serial.println("[LocalNetwork] Error: No data on " + source);
        server.send(400, "application/json", "{\"error\": \"No Data\"}");
        return false;
    }

    String json = server.arg("plain");
    DeserializationError error = deserializeJson(document, json);

    if (error) {
        Serial.println("[LocalNetwork] Error: Failed to parse setup data");
        server.send(400, "application/json", "{\"error\": \"Invalid Data\"}");
        return false;
    }

    return true;
}

void LocalNetwork::handleRoot() {
    server.send(200, "text/plain", "KiLL");
}

void LocalNetwork::handleLocal() {
    server.send(200, "text/plain", SSID());
}

void LocalNetwork::handleNotFound() {
    server.send(404, "text/plain", "Not found");
}

void LocalNetwork::handleSetup() {
    if (Memory::verifyContent()) {
        Serial.println("[LocalNetwork] Error: Tried to setup KiLL twice.");
        server.send(400, "application/json", "{\"error\": \"KiLL already setup.\"}");
        return;
    }

    JsonDocument document;
    if (!checkRequestData(document, "setup")) return;

    String ssid = document["ssid"] | "";
    String password = document["password"] | "";
    String appId = document["appId"] | "";

    if (ssid.length() == 0 || password.length() == 0 || appId.length() == 0) {
        Serial.println("[LocalNetwork] Error: Missing data on setup. SSID: " + ssid + ", Password: " + password + ", App ID: " + appId);
        server.send(400, "application/json", "{\"error\": \"Missing Data\"}");
        return;
    }

    Serial.println("[LocalNetwork] Received data on setup: SSID: " + ssid + ", Password: " + password + ", App ID: " + appId);

    server.send(200, "application/json", "{\"status\": \"OK\"}");

    Memory::write(ssid, password, appId);
}

void LocalNetwork::handleResetFactory() {
    JsonDocument document;
    if (!checkRequestData(document, "reset factory")) return;

    if (Utils::verifyRequest(document)) {
        server.send(200, "application/json", "{\"status\": \"OK\"}");
        KiLL::resetToFactorySettings();
    } else {
        server.send(400, "application/json", "{\"error\": \"Missing authentication\"}");
    }
}

void LocalNetwork::handleCommand() {
    JsonDocument document;
    if (!checkRequestData(document, "command")) return;
    if (!Utils::verifyRequest(document)) {
        server.send(400, "application/json", "{\"error\": \"Missing authentication\"}");
        return;
    }

    String command = document["command"] | "";
    String value = document["value"] | "";

    // TODO: Implement command handling
    server.send(200, "application/json", "{\"status\": \"OK\"}");
}