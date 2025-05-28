#ifndef BOILER_H
#define BOILER_H

class Boiler {
public:
    Boiler();

    void setTargetTemperature(int temperature);
    void turnOn();
    void turnOff();

    double getCurrentTemperature();
    bool getIsOn();

private:
    double currentTemperature;
    bool isOn;
};

#endif