#include "Display.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

Display::Display(){
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
}

void Display::beginDisplay(){
    Wire.begin(21, 22); 

    if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Fallo al iniciar la pantalla OLED"));
    }

    display->clearDisplay();
    display->display();
    show(100, 100); 
}


void Display::show(int setPoint, float trueValue) {
    display->clearDisplay();

    display->setTextSize(4);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->print(setPoint);
    int x = display->getCursorX();
    display->drawCircle(x + 2, 5, 2, SSD1306_WHITE);
    display->setCursor(x + 8, 0);
    display->print("C");

    display->setTextSize(2);
    display->setCursor(5, 40);
    display->print(trueValue, 2);
    x = display->getCursorX();
    display->drawCircle(x + 2, 42, 1, SSD1306_WHITE);
    display->setCursor(x + 8, 40);
    display->print("C");

    display->display();
}
