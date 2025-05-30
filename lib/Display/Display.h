#ifndef DISPLAY_OLED_H
#define DISPLAY_OLED_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

class Display {
public:
    Display();
    void beginDisplay(int initSetPoint, double currentValue);
    void updateSetPoint(int newSetPoint);
    void updateTrueValue(float newTrueValue);
    void show();  // usa los valores actuales

private:
    Adafruit_SSD1306* display;
    static constexpr uint16_t SCREEN_WIDTH = 128;
    static constexpr uint16_t SCREEN_HEIGHT = 64;

    int setPoint;
    float trueValue;
};

#endif
