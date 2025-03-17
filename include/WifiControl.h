#ifndef WIFI_CONTROL_H
#define WIFI_CONTROL_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>

#include "MemoryControl.h"
#include "Kill.h"

void handleRoot();
void handleData();

bool connected = false;
WebServer server(80);

const String SSID() {
    return "KiLL-" + espId();
}

/// @brief Set up the ESP32 to be accessible on the local network.
void setupLocalNetwork() {
    String hostname = SSID();
    while (!MDNS.begin(hostname)) {
        Serial.println("Error setting up MDNS responder!");
        delay(1000);
    }
    Serial.println("mDNS responder started");
}

/// @brief The URL to access the ESP32's web server on the local network. 
/// Example: "http://kill-5CCF7F0D3A4A.local/"
String getLocalNetworkHostname() {
    return "http://kill-" + espId() + ".local/";
}

/// @brief Connect to the WiFi network using the SSID and password saved in the EEPROM, with a timeout of 10 seconds.
void connectToWiFi() {
    Serial.print("Connecting to WiFi");

    WiFi.begin(ssidSaved.c_str(), passwordSaved.c_str());

    unsigned long startTime = millis();
    const unsigned long timeout = 10000;

    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout) {
        Serial.print(".");
        delay(500);
    }

    connected = WiFi.status() == WL_CONNECTED;

    if (connected) {
        Serial.println("\nConnected to WiFi!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nError: Timeout connecting to WiFi.");
    }
}

void initAP() {
    WiFi.softAP(SSID());
    setupLocalNetwork();
    server.on("/", handleRoot);
    server.on("/data", HTTP_POST, handleData);
    server.begin();
}

void initWiFi() {
    WiFi.mode(WIFI_AP_STA); 
    if(readMemory()) connectToWiFi(); 
    initAP();
}

// MARK: Server Routes

void handleRoot() {
    server.send(200, "text/plain", ssidSaved + " " + passwordSaved + " "+ String(connected)) ;
}

void handleData() {
    if(server.arg("ssid") && server.arg("password")) {
        ssidSaved = server.arg("ssid");
        passwordSaved = server.arg("password");
        writeMemory();
        connectToWiFi();
        server.send(200, "text/plain", "Datos recibidos correctamente");
    } else if (server.arg("tempSP")) {
        int tempSP = server.arg("tempSP").toInt();
    } else {
        server.send(400, "text/plain", "No se recibieron datos.");
    }
}


#endif
