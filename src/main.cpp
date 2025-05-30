#include <Arduino.h>
#include <KiLL.h>
#include <Display.h>

KiLL smartBoiler;
Display display;

void setup() {
  Serial.begin(115200);
  smartBoiler.setup();
  display.beginDisplay();
}

void loop() {
  smartBoiler.keepServersAlive();
  smartBoiler.checkForFactoryReset();
  smartBoiler.tryToReconnectToWifi();
}