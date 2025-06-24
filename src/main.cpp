#include <Arduino.h>
#include <KiLL.h>

KiLL autoBoiler;

void setup() {
  Serial.begin(115200);
  autoBoiler.setup();
}

void loop() {
  autoBoiler.keepServersAlive();
  autoBoiler.tryToReconnectToWifi();
  
  autoBoiler.checkUserInteraction();
  autoBoiler.controlTemperature();
}