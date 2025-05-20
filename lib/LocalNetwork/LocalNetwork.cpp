#include <Memory.h>
#include <LocalNetwork.h>

LocalNetwork::LocalNetwork() : server(HTTP_PORT) {}

const String LocalNetwork::getHostname() {
    return "http://" + espId() + ".local/";
}

const String LocalNetwork::SSID() {
    return "KiLL-" + espId();
}

void LocalNetwork::initialize() {
    WiFi.softAP(SSID());
    Serial.println("[LocalNetwork] WiFi Access Point started");
    Serial.println("[LocalNetwork] IP Address: " + WiFi.softAPIP().toString());
}

void LocalNetwork::stopAccessPoint() {
    WiFi.softAPdisconnect(true);
    Serial.println("[LocalNetwork] WiFi Access Point stopped");
}

void LocalNetwork::setupServer() {
    Serial.println("[LocalNetwork] Setting up local server");

    server.on("/", HTTP_GET, std::bind(&LocalNetwork::handleRoot, this));
    server.onNotFound(std::bind(&LocalNetwork::handleNotFound, this));
    server.on("/setup", HTTP_POST, std::bind(&LocalNetwork::handleSetup, this));
    server.on("/kill_reset_factory", HTTP_POST, std::bind(&LocalNetwork::handleResetFactory, this));
    server.on("/command", HTTP_POST, std::bind(&LocalNetwork::handleCommand, this));

    startServer();
}

void LocalNetwork::startServer() {
    server.begin();
    Serial.println("[LocalNetwork] Local server started");
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

// MARK: Routes

bool LocalNetwork::checkRequestData(JsonDocument& document) {
    if (!server.hasArg("plain")) {
        Serial.println("[LocalNetwork] Error: No data on setup");
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

void LocalNetwork::handleNotFound() {
    Serial.println("[LocalNetwork] Error: Not found");
    server.send(404, "text/plain", "Not found");
}

void LocalNetwork::handleSetup() {
    JsonDocument document;
    if (!checkRequestData(document)) return;

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
    if (!checkRequestData(document)) return;

    if (Utils::verifyRequest(document)) {
        server.send(200, "application/json", "{\"status\": \"OK\"}");
        resetToFactorySettings();
    } else {
        server.send(400, "application/json", "{\"error\": \"Missing authentication\"}");
    }
}

void LocalNetwork::handleCommand() {
    JsonDocument document;
    if (!checkRequestData(document)) return;
    if (!Utils::verifyRequest(document)) {
        server.send(400, "application/json", "{\"error\": \"Missing authentication\"}");
        return;
    }

    String command = document["command"] | "";
    String value = document["value"] | "";

    // TODO: Implement command handling
}