#include <Arduino.h>

#include "Connectivity/LocalNetwork.h"
#include "Connectivity/GlobalNetwork.h"
#include "KiLL.h"
#include "Memory2.h"

void setup() {
  Serial.begin(115200);

  pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
  
  initializeMemory();
  WiFi.onEvent(onWiFiEvent);
  
  if (verifyMemory()) {
    startWiFiConnect();
  } else {
    initializeWiFiAccessPoint();
    startLocalServer();
  }

  setupLocalNetwork();
}

void loop() {
  localServer.handleClient();
  checkForFactoryReset();
  tryReconnectWiFi();
}