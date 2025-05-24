#include <Arduino.h>
#include <KiLL.h>

KiLL smartBoiler;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  smartBoiler.setup();
}

void loop() {
  smartBoiler.keepLocalServerAlive();
  smartBoiler.checkForFactoryReset();
  smartBoiler.tryToReconnectToWifi();
}