#include "KiLL.h"

#include <Arduino.h>    

#include "Memory.h"        
#include "GlobalNetwork.h"
#include "LocalNetwork.h" 

KiLL::KiLL() {
    localNetwork = new LocalNetwork();      
    globalNetwork = new GlobalNetwork(*localNetwork);

    factoryButtonPressed = false;
    factoryButtonPressedTime = 0;
}

KiLL::~KiLL() {
    delete localNetwork;
    delete globalNetwork;
    localNetwork = nullptr;
    globalNetwork = nullptr;
}

void KiLL::setup() {
    pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
    Memory::initialize();
    
    localNetwork->initialize();
    localNetwork->setupLocalNetwork();
    localNetwork->setupServer();

    if (Memory::verifyContent()) {
        globalNetwork->startWiFiConnection();
    }
}

const String KiLL::espId() {
    uint64_t chipId = ESP.getEfuseMac();
    char buffer[13];
    sprintf(buffer, "%04X%08X", (uint16_t)(chipId >> 32), (uint32_t)chipId);
    return String(buffer);
}

void KiLL::keepServersAlive() {
    localNetwork->keepServerAlive();
    if (globalNetwork->isConnectedToWifi()) {
        globalNetwork->keepConnectionWithServer();
    }
}

void KiLL::tryToReconnectToWifi() {
    globalNetwork->tryReconnectWiFi();
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