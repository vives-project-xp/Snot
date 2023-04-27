#pragma once

#include "Board.h"
#include "Game.h"
#include "mbed_events.h"

class GameHandler {
    private:
        static const float LCD_INTERVAL;

        Board * board;
        bool gameRunning;

        EventQueue* queue;

    public:
        Game * game;

    public:
        GameHandler(char * buffer, Board * board, EventQueue* queue);
        void run();
        void toLcd();
};
