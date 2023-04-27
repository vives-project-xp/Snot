#include "mbed.h"
#include "GameHandler.h"
#include "GameCreator.h"

const float GameHandler::LCD_INTERVAL = 3.0;

GameHandler::GameHandler(char * buffer, Board * board, EventQueue* queue){
    this->board = board;
    this->queue = queue;
    game = GameCreator::createInstance(buffer);
    game->attachBoard(this->board);
    game->loadNextMission();
    gameRunning = true;
    queue->call_every( 
        std::chrono::seconds((int)LCD_INTERVAL),
        mbed::Callback<void()>(game, &Game::toLcd)
    );
}

void GameHandler::run(){
    // if(game->isComplete()) gameRunning = false;
    if(gameRunning) game->run();
}
