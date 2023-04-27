#pragma once
#include "TLC59116.h"


class Leds{

    private:
        TLC59116* leds;
        static const int address = 0xC0;

    public:
        Leds(I2C* i2c);
        void setLed(int led, float red, float green, float blue);
        void setGlobalBrightness(float brightness);
        void resetLeds();
        void reactToDistance(int distance, int radius);
        void setStartState();

};
