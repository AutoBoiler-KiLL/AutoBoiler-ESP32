#ifndef BOILER_H
#define BOILER_H

#include "PID.h"

class TemperatureSensor;

class Boiler {
public:
    Boiler();

    void begin();

    double controlTemperature();

    void setTargetTemperature(int temperature);
    int getTargetTemperature();

    void turnOn();
    void turnOff();
    bool getIsOn();
    
    double getCurrentTemperature();
    
    int getMinimumTemperature();

    static void IRAM_ATTR zeroCrossISR(); 
    void setPowerPercent(uint8_t percent);

    void toggle();

private:
    static constexpr uint8_t BOILER_LED_PIN = 25;
    static constexpr uint8_t ZERO_CROSS_DETECTION_PIN = 16;
    static constexpr uint8_t SSR_ACTIVAION_PIN = 23;
    static constexpr unsigned long MIN_TEMP_UPDATE_INTERVAL = 60 * 1000;
    
    int targetTemperature;
    double currentTemperature;
    bool isOn;
    int minimumTemperature;
    
    unsigned long lastMinTempUpdate;
    bool shouldUpdateMinTemp;
    
    TemperatureSensor* temperatureSensor;
    PID* pid;

    static Boiler* instance; 
    volatile bool zeroCrossDetectedFlag = false;
    int cicleDetectedFlag = 0;

    volatile uint8_t zeroCrossCounter = 0;      
    volatile uint8_t desiredActiveCycles = 8;  
    static const uint8_t totalCycles = 10;

    /// @brief Updates the minimum temperature if starting up or every minute while off
    void updateMinimumTemperature();
};

#endif
