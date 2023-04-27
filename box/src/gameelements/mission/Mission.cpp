#include "Mission.h"
#include <string>

    Mission::Mission(std::string name, int id, std::string description, int typeId){
        this->name = name;
        this->id = id;
        this->description = description;
        this->typeId = typeId;
    }

    std::string Mission::getName(){
        return name;
    }

    int Mission::getId(){
        return id;
    }

    int Mission::getTypeId(){
        return typeId;
    }

    std::string Mission::getDescription(){
        return description;
    }

    void Mission::attachBoard(Board* board){
        this->board = board;
    }
