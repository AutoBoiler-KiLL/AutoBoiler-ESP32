#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Adafruit_ADS1X15.h>

class TemperatureSensor {
public:
    TemperatureSensor();

    /// @brief Initialize the temperature sensor.
    void begin();

    /// @brief Read the temperature from the sensor.
    /// @param channel The channel to read the temperature from.
    /// @return The temperature in Celsius.
    double readTemperature(uint8_t channel);

private:
    Adafruit_ADS1115* ads;

    static constexpr double R_FIXED = 3250.0;

    static constexpr double R0 = 100.0;
    static constexpr double alpha = 0.00385;
    
    static constexpr uint8_t TEMPERATURE_WINDOW_SIZE = 50;
    double temperatureWindow[TEMPERATURE_WINDOW_SIZE];
    uint8_t windowIndex;
    bool windowFilled;
    
    double getFilteredTemperature(double newReading);

    double getVCC();
};

#endif
