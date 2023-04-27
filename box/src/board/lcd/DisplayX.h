#pragma once
#include <string>

class DisplayX{

    public:
        virtual void setEndScreen() = 0;
        virtual void setStartScreen() = 0 ;
        virtual void setMessageScreen(std::string message) = 0;
        virtual void setMissionScreen(std::string description, int distance, int radius) = 0;
        virtual void setTime(int hours, int minutes, int seconds) = 0;
        virtual void eventHandler(char key) = 0;
        virtual void setStatusGps(int gpsFix) = 0;
};
