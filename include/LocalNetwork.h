#ifndef LOCAL_NETWORK_H
#define LOCAL_NETWORK_H

#include "Kill.h"
#include <ESPmDNS.h>

/// @brief Set up the ESP32 to be accessible on the local network.
void setupLocalNetwork() {
    String hostname = "kill-" + espId();
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

#endif