#include <Arduino.h>
#include <KiLL.h>
#include <Display.h>
#include <TemperatureSensor.h>

KiLL smartBoiler;
Display display;
TemperatureSensor temperatureSensor;

void setup() {
  Serial.begin(115200);
  smartBoiler.setup();
}

void loop() {
  smartBoiler.keepServersAlive();
  smartBoiler.checkUserInteraction();
  smartBoiler.tryToReconnectToWifi();
}