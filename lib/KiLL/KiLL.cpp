#include <Memory.h>
#include <KiLL.h>

KiLL::KiLL() : globalNetwork(localNetwork) {
    factoryButtonPressed = false;
    factoryButtonPressedTime = 0;
}

void KiLL::setup() {
    pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
    Memory::initialize();
    localNetwork.setupLocalNetwork();
    localNetwork.setupServer();

    if (Memory::verifyContent()) {
        globalNetwork.startWiFiConnection();
    } else {
        localNetwork.initialize();
    }
}

const String KiLL::espId() {
    uint64_t chipId = ESP.getEfuseMac();
    char buffer[13];
    sprintf(buffer, "%04X%08X", (uint16_t)(chipId >> 32), (uint32_t)chipId);
    return String(buffer);
}

void KiLL::keepLocalServerAlive() {
    if (globalNetwork.isConnectedToWifi()) {
        localNetwork.keepServerAlive();
    }
}

void KiLL::tryToReconnectToWifi() {
    globalNetwork.tryReconnectWiFi();
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