#include "Utils.h"

#include <Arduino.h> // For Serial
#include <KiLL.h>    // For KiLL::espId()
#include <Memory.h>  // For Memory::getAppId()

namespace Utils {
    bool verifyRequest(JsonDocument document) {
        String receivedEspId = document["espId"] | "";
        String appId = document["appId"] | "";

        if (receivedEspId != KiLL::espId() || appId != Memory::getAppId()) {
            Serial.println("[LocalNetwork] Error: Invalid espId or appId. ESP ID: " + receivedEspId + ", App ID: " + appId);
            return false;
        }

        return true;
    }
} 