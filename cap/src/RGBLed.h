#pragma once
#include "mbed.h"
namespace cap {
    class RGBLed {
        private:
            PwmOut* ledR;
            PwmOut* ledG;
            PwmOut* ledB;
        
        public:
            RGBLed(PinName ledRPin, PinName ledGPin, PinName ledBPin);
            ~RGBLed();

            void setColor(int red, int green, int blue);
    };
};
