#ifndef BOILER_H
#define BOILER_H

class Boiler {
public:
    Boiler();

    void setTargetTemperature(int temperature);

    double getCurrentTemperature();

private:
    double currentTemperature;
};

#endif