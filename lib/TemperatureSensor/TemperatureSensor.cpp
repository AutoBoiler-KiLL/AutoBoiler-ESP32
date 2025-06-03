#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor() {
    ads = new Adafruit_ADS1115();
}

void TemperatureSensor::begin() {
    ads->begin();
    ads->setGain(GAIN_SIXTEEN); 
}

double TemperatureSensor::readTemperature(uint8_t channel) {
    int16_t rawADC = ads->readADC_SingleEnded(channel);
    double vOut = (rawADC * 0.256) / 32767.0;
    double R_PT100 = (-vOut * R_FIXED) / (vOut - VCC);
    double temperature = (R_PT100 - R0) / (alpha * R0);
    return temperature;
}
