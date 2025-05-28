#include "Memory.h"

void Memory::initialize() {
    EEPROM.begin(EEPROM_SIZE);
    Serial.println("[Memory] EEPROM initialized");
}

bool Memory::verifyContent() {
    return EEPROM.readString(SSID_ADDRESS).length() != 0 && 
           EEPROM.readString(PASS_ADDRESS).length() != 0 &&
           EEPROM.readString(APP_ID_ADDRESS).length() != 0;
}

String Memory::getSSID() {
    return EEPROM.readString(SSID_ADDRESS);
}

String Memory::getPassword() {
    return EEPROM.readString(PASS_ADDRESS);
}

String Memory::getAppId() {
    return EEPROM.readString(APP_ID_ADDRESS);
}

int Memory::getTemperature() {
    return EEPROM.readInt(TEMPERATURE_ADDRESS);
}

void Memory::write(String ssid, String password, String appId) {
    Serial.println("[Memory] Writing credentials to memory");
    EEPROM.writeString(SSID_ADDRESS, ssid);
    EEPROM.writeString(PASS_ADDRESS, password);
    EEPROM.writeString(APP_ID_ADDRESS, appId);
    EEPROM.commit();
}

void Memory::writeTemperature(int temperature) {
    Serial.println("[Memory] Writing temperature " + String(temperature) + "º to memory");
    EEPROM.writeInt(TEMPERATURE_ADDRESS, temperature);
    EEPROM.commit();
}

void Memory::clear() {
    EEPROM.writeString(SSID_ADDRESS, "");
    EEPROM.writeString(PASS_ADDRESS, "");
    EEPROM.writeString(APP_ID_ADDRESS, "");
    EEPROM.writeInt(TEMPERATURE_ADDRESS, 0);
    EEPROM.commit();
}