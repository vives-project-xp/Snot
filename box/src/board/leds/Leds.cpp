#include "Leds.h"

Leds::Leds(I2C* i2c){
    this->leds = new TLC59116(i2c, this->address);
    resetLeds();
}

void Leds::setLed(int led, float red, float green, float blue){
    int startLed = led*3;
    leds->setChannel(startLed, green);
    leds->setChannel(startLed+1, red);
    leds->setChannel(startLed+2, blue);
}

void Leds::setGlobalBrightness(float brightness){
    leds->setBrightness(brightness);
}

void Leds::resetLeds(){
    for(int i = 0; i < 15; i++){
        leds->setChannel(i, 0);
    }
}

void Leds::reactToDistance(int distance, int radius){
    int distMeasure = radius * 20;

    if(distance < distMeasure*0.1){
        setLed(0,0,1,0);
        setLed(1,0,1,0);
        setLed(2,0,1,0);
        setLed(3,0,1,0);
        setLed(4,0,1,0);
    }else if(distance < distMeasure*0.25){
        setLed(0,1,0,0);
        setLed(1,1,0,0);
        setLed(2,1,0,0);
        setLed(3,1,0,0);
        setLed(4,0,0,0);
    }else if(distance < distMeasure*0.5){
        setLed(0,1,0,0);
        setLed(1,1,0,0);
        setLed(2,1,0,0);
        setLed(3,0,0,0);
        setLed(4,0,0,0);
    }else if(distance < distMeasure*0.75){
        setLed(0,1,0,0);
        setLed(1,1,0,0);
        setLed(2,0,0,0);
        setLed(3,0,0,0);
        setLed(4,0,0,0);
    }else if(distance < distMeasure){
        setLed(0,1,0,0);
        setLed(1,0,0,0);
        setLed(2,0,0,0);
        setLed(3,0,0,0);
        setLed(4,0,0,0);
    }
    else{
        setLed(0,0,0,1);
        setLed(1,0,0,1);
        setLed(2,0,0,1);
        setLed(3,0,0,1);
        setLed(4,0,0,1);
    }
}

void Leds::setStartState(){
    setGlobalBrightness(0.2);
    setLed(0,1,0,0);
    setLed(1,0,1,0);
    setLed(2,0,0,1);
    setLed(3,1,1,0);
    setLed(4,0,1,1);
}
