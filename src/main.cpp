#include <Arduino.h>

#include "Connectivity/GlobalNetwork.h"
#include "KiLL.h"
#include "Memory.h"

void setup() {
  Serial.begin(115200);
  pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
  initMemory();
  setupLocalNetwork();
  WiFi.onEvent(onWiFiEvent);
  
  if (verifyMemory()) startWiFiConnect();
  else {
    initializeWiFiAccessPoint();
    startLocalServer();
  }
}

void loop() {
  localServer.handleClient();
  checkForFactoryReset();
  tryReconnectWiFi();
}