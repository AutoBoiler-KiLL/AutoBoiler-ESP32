#include <Arduino.h>
#include "WifiControl.h"
#include "PowerControl.h"


void setup () {
  Serial.begin(9600);
  initWifi();
}

void loop () {
  server.handleClient();
}