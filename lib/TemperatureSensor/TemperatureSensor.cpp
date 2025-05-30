 #include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor() {
    ads = new Adafruit_ADS1115();
}

void TemperatureSensor::begin() {
    ads->begin();
}

float TemperatureSensor::readTemperature(uint8_t chanel) {
    int16_t raw = ads->readADC_SingleEnded(chanel);
    return ads->computeVolts(raw);
}
