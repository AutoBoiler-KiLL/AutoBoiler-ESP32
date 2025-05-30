#ifndef BOILER_H
#define BOILER_H

class Boiler {
public:
    Boiler();

    void setTargetTemperature(int temperature);
    int getTargetTemperature();

    void turnOn();
    void turnOff();
    bool getIsOn();

    double getCurrentTemperature();
    void setCurrentTemperature(double temperature);


private:
    double currentTemperature;
    bool isOn;
};

#endif