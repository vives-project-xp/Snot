#include "KeyMission.h"
#include "mbed_trace.h"

#define TRACE_GROUP "KEYMISSION"

KeyMission::KeyMission(std::string name, int id, std::string description, int typeId): Mission(name, id, description, typeId){
}

void KeyMission::debug(){
    tr_debug("Key turned: %s", board->keys->isKeyTurned() ? "YES" : "NO");
}

bool KeyMission::isComplete(){
    if(board->keys->isKeyTurned()){
        board->buzzer->completeSound();
        return true;
    }
    else return false;
}

void KeyMission::toLcd(){
    this->board->lcd->setMessageScreen(getDescription());
}