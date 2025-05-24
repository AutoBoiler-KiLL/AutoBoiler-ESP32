#include <Arduino.h>
#include <KiLL.h>

KiLL smartBoiler;

void setup() {
  Serial.begin(115200);
  smartBoiler.setup();
}

void loop() {
  smartBoiler.keepServersAlive();
  smartBoiler.checkForFactoryReset();
  smartBoiler.tryToReconnectToWifi();
}