#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Adafruit_ADS1X15.h>

class TemperatureSensor {
public:
    TemperatureSensor();
    void begin();
    float readTemperature(uint8_t chanel);

private:
    Adafruit_ADS1115* ads;
};

#endif
