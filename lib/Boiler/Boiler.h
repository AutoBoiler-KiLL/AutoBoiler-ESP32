#ifndef BOILER_H
#define BOILER_H

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

private:
    int currentTargetTemperature;
    double currentTemperature;
    bool isOn;
    TemperatureSensor* temperatureSensor;
};

#endif
