#pragma once

#include "Mission.h"

class KeyMission : public Mission{

    public:
        KeyMission(std::string name, int id, std::string description, int typeId);
        virtual void debug();
        virtual bool isComplete();
        virtual void toLcd();
};
