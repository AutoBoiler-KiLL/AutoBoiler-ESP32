#include <Arduino.h>
#include "WifiControl.h"
#include "PowerControl.h"

void setup () {
  Serial.begin(115200);
  initWiFi();
}

void loop () {
  server.handleClient();
}