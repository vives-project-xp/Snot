#include "Game.h"
#include "mbed.h"
#include "mbed_trace.h"

#define TRACE_GROUP "GAME"

Game::Game() {
    this->linkedList = new LinkedList2();
    currentMission = NULL;
    complete = false;
}

void Game::run(){
   //If mission complete, request new mission
    // if( currentMission->isComplete() ) loadNextMission();
}

void Game::debug(){
    if(currentMission == NULL) tr_warn("No current mission");
    else{
      tr_debug("Mission Name: %s", currentMission->getName().c_str());
      tr_debug("Mission ID: %i", currentMission->getId());
      tr_debug("Mission Description: %s", currentMission->getDescription().c_str());
      currentMission->debug();
    }
}

void Game::loadNextMission(){
    if(currentMission == NULL){
        this->currentNode = this->linkedList->getFirstNode();
        this->currentMission = currentNode->getData();
        this->currentMission->attachBoard(board);
    }
    else{
        this->currentNode = this->currentNode->getNextNode();
        if(this->currentNode == NULL){
            //Geen missions meer
            this->board->lcd->setEndScreen();
            this->currentMission = NULL;
            complete = true;
        }
        else{
            this->currentMission = this->currentNode->getData();
            this->currentMission->attachBoard(board);
        }

    }

}


void Game::addMission(Mission * mission){
    this->linkedList->addNode(mission);
}

void Game::addBasicValues(std::string name, std::string box_description, int length, int time){
    this->name = name;
    this->box_description = box_description;
    this->length = length;
    this->time = time;
}


int Game::getCurrentMissionId(){
    if(this->currentMission == NULL) return 0;
    else return this->currentMission->getId();
}

void Game::attachBoard(Board* board){
    this->board = board;
}

// bool Game::isComplete(){
//     return this->complete;
// }

void Game::toLcd(){
    this->board->lcd->setTime(1,2,3);
    this->board->lcd->setStatusGps(1);
    // if(isComplete()) this->board->lcd->setMessageScreen("No missions available");
    // else{
    //     this->currentMission->toLcd();
    // }
}
