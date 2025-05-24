#include "KiLL.h"

#include <Arduino.h>       // For pinMode, millis, Serial, etc.
#include <Memory.h>        // For Memory functions
#include "GlobalNetwork.h" // Definition for GlobalNetwork
#include "LocalNetwork.h"  // Definition for LocalNetwork

// Constructor: Initialize pointers and allocate objects
KiLL::KiLL() {
    localNetwork = new LocalNetwork();       // Allocate LocalNetwork object
    // Pass the now valid localNetwork pointer to GlobalNetwork constructor if needed
    // Assuming GlobalNetwork constructor takes LocalNetwork* or LocalNetwork&
    // If GlobalNetwork needs localNetwork during its construction, ensure order or pass *localNetwork
    globalNetwork = new GlobalNetwork(*localNetwork); // Example: If GlobalNetwork constructor needs a LocalNetwork reference

    factoryButtonPressed = false;
    factoryButtonPressedTime = 0;
}

// Destructor: Clean up allocated objects
KiLL::~KiLL() {
    delete localNetwork;
    delete globalNetwork;
    localNetwork = nullptr;  // Good practice to nullify pointers after deletion
    globalNetwork = nullptr;
}

void KiLL::setup() {
    pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
    Memory::initialize();
    localNetwork->setupLocalNetwork(); // Use -> for pointers
    localNetwork->setupServer();       // Use -> for pointers

    if (Memory::verifyContent()) {
        globalNetwork->startWiFiConnection(); // Use -> for pointers
    } else {
        localNetwork->initialize();         // Use -> for pointers
    }
}

const String KiLL::espId() {
    uint64_t chipId = ESP.getEfuseMac();
    char buffer[13];
    sprintf(buffer, "%04X%08X", (uint16_t)(chipId >> 32), (uint32_t)chipId);
    return String(buffer);
}

void KiLL::keepLocalServerAlive() {
    if (globalNetwork->isConnectedToWifi()) { // Use -> for pointers
        localNetwork->keepServerAlive();      // Use -> for pointers
    }
}

void KiLL::tryToReconnectToWifi() {
    globalNetwork->tryReconnectWiFi(); // Use -> for pointers
}

void KiLL::checkForFactoryReset() {
    bool isButtonNowPressed = digitalRead(FACTORY_RESET_PIN) == LOW;

    // Button pressed for the first time
    if (isButtonNowPressed && !factoryButtonPressed) {
        factoryButtonPressed = true;
        factoryButtonPressedTime = millis();
    } 
    
    // Button pressed for FACTORY_RESET_HOLD_TIME, reset to factory settings
    if (isButtonNowPressed && millis() - factoryButtonPressedTime >= FACTORY_RESET_HOLD_TIME) {
        Serial.println("[KiLL] Factory reset button pressed for too long. Resetting to factory settings...");
        resetToFactorySettings();
    }

    // Button released before timeout
    if (!isButtonNowPressed && factoryButtonPressed) {
        factoryButtonPressed = false;
        factoryButtonPressedTime = 0;
    }    
}

void KiLL::resetToFactorySettings() {
    Serial.println("[KiLL] Resetting to factory settings...");
    Memory::clear();
    delay(3000);
    ESP.restart();
}