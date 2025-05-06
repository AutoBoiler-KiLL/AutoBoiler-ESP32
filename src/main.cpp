#include <Arduino.h>

#include "WifiControl.h"
#include "PowerControl.h"
#include "KiLL.h"

void setup () {
  Serial.begin(115200);
  pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
  initWiFi();
}

void loop () {
  server.handleClient();
  checkForFactoryReset();
}