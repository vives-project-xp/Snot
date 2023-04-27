#pragma once

#include "Game.h"

class GameCreator {
    public:
        static Game* createInstance(char * buffer);
};
