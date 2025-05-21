#include <Arduino.h>
#include <KiLL.h>

KiLL kill;

void setup() {
  Serial.begin(115200);
  kill.setup();
}

void loop() {
  kill.keepLocalServerAlive();
  kill.checkForFactoryReset();
  kill.tryToReconnectToWifi();
}