#include "RGBLed.h"

namespace cap {
    RGBLed::RGBLed(PinName ledRPin, PinName ledGPin, PinName ledBPin) {
        ledR = new PwmOut(ledRPin);
        ledG = new PwmOut(ledGPin);
        ledB = new PwmOut(ledBPin);
    }

    RGBLed::~RGBLed() {
        delete ledR;
        delete ledG;
        delete ledB;
    }

    void RGBLed::setColor(int red, int green, int blue) {
        printf("red: %d, ", red);
        printf("green: %d, ", green);
        printf("blue: %d\n", blue);
        ledR->write(1 - (float) red);
        ledG->write(1 - (float) green);
        ledB->write(1 - (float) blue);
    }
}
