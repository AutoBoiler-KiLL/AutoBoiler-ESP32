 #include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor() {
    ads = new Adafruit_ADS1115();
}

void TemperatureSensor::begin() {
    ads->begin();
    ads->setGain(GAIN_FOUR); 
}

float TemperatureSensor::readTemperature(uint8_t chanel) {
    float vout = ads->computeVolts(ads->readADC_SingleEnded(chanel));

    const float VCC = 3.3;
    const float R_FIXED = 3300.0;

    float r_pt100 = (-vout * R_FIXED) / (vout - VCC);

    // Convertir a °C usando ecuación lineal
    const float R0 = 100.0;
    const float alpha = 0.00385;

    float temperature = (r_pt100 - R0) / (alpha * R0);

    return temperature;
}
