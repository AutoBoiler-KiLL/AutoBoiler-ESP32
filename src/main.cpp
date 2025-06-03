#include <Arduino.h>
#include <KiLL.h>
#include <Display.h>
#include <TemperatureSensor.h>

KiLL smartBoiler;

void setup() {
  Serial.begin(115200);
  smartBoiler.setup();
}

void loop() {
  smartBoiler.keepServersAlive();
  smartBoiler.tryToReconnectToWifi();
  
  smartBoiler.checkUserInteraction();
  smartBoiler.controlTemperature();
}