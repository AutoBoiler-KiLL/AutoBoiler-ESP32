#include <KiLL.h>
#include <Memory.h>

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