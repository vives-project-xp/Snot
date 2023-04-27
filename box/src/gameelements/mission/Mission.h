#pragma once

#include <string>
#include <vector>
#include "Board.h"

class Mission {
    private:
       std::string name;
       std::string description;
       int id;
       int typeId;

    protected:
       Board * board;


    public:
        Mission(std::string name, int id, std::string description, int typeId);
        virtual void debug()=0;
        virtual void toLcd()=0;

    public: //Getters
        std::string getName();
        std::string getDescription();
        int getId();
        int getTypeId();
        // virtual bool isComplete()=0;

    public: //Setters
        void attachBoard(Board* board);

};
