#pragma once
#include "TextLCD.h"
#include <string>
#include "./DisplayX.h"


class Lcd : public DisplayX{
    private:
        TextLCD* lcd;
        PwmOut* backlight;
    public:
        Lcd();
        virtual void setEndScreen();
        virtual void setStartScreen();
        virtual void setWrongNumberScreen( void );
        virtual void setMessageScreen(std::string message);
        virtual void setMissionScreen(std::string description, int distance, int radius);
        virtual void setTime(int hours, int minutes, int seconds);
        virtual void eventHandler(char key);
        virtual void setStatusGps(int gpsFix);
};
