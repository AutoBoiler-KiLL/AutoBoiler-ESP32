#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID {
public:
    PID();

    int control(double targetTemperature, double currentTemperature, double minTemperature, double maxTemperature);

private:
    static constexpr double Kp = 5;
    static constexpr double Ki = 2;
    static constexpr double Kd = 2;

    static constexpr uint8_t INTERVAL = 15;
    static constexpr unsigned long RESET_INTEGRAL_TIME = 3000;

    double previousError;
    double errorSum;
    double P, I, D, pid;
    bool resetIntegral;
    unsigned long previousMillis;
    int power;

    unsigned long initialTime;
};

#endif