#ifndef DISPLAY_OLED_H
#define DISPLAY_OLED_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

class Display {
public:
    Display();
    void beginDisplay();
    void show(int setPoint, float trueValue);

private:
    Adafruit_SSD1306* display;
    static constexpr uint16_t SCREEN_WIDTH = 128;
    static constexpr uint16_t SCREEN_HEIGHT = 64;
};

#endif
