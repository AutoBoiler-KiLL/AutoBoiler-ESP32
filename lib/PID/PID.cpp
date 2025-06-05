#include "PID.h"

PID::PID() {
    previousError = 0;
    P = 0;
    I = 0;
    D = 0;
    pid = 0;
    resetIntegral = false;
    initialTime = 0;
    errorSum = 0;
    previousMillis = 0;
    power = 0;
}

int PID::control(double targetTemperature, double currentTemperature, double minTemperature, double maxTemperature) {
    if (millis() - previousMillis >= INTERVAL) {
        previousMillis = millis();
    } else {
        return power;
    }

    float error = targetTemperature - currentTemperature;
    float derivativeError = error - previousError;

    P = Kp * error;
    D = Kd * derivativeError / (INTERVAL / 1000.0);

    if (abs(error) > 0.05 * targetTemperature) {
        if (resetIntegral) {
            if (millis() - initialTime > RESET_INTEGRAL_TIME) {
                errorSum = 0;
                resetIntegral = false;
            }
        } else {
            resetIntegral = true;
            initialTime = millis();
        }
    } else if (abs(error) < (0.019 * targetTemperature)) resetIntegral = false;

    errorSum += error * INTERVAL / 1000.0;
    I = Ki * errorSum;

    pid = P + I + D;

    power = constrain(map(pid, minTemperature, maxTemperature, 0, 25), 0, 25);

    return power;
}