#include <Arduino.h>

#include "Connectivity/GlobalNetwork.h"
#include "KiLL.h"

void setup() {
  Serial.begin(115200);
  pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
  setupLocalNetwork();
  WiFi.onEvent(onWiFiEvent);
}

void loop() {
  localServer.handleClient();
  checkForFactoryReset();
  tryReconnectWiFi(); 
}