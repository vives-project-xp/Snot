#pragma once

#include "LinkableNode.h"
#include "Mission.h"


class LinkedList2{
    private:
        LinkableNode * headNode;

    public:
        LinkedList2();
        ~LinkedList2();

    public:
        void addNode(Mission * data);
        void removeNode();
        std::string toString();

    private:
        LinkableNode* getLastNode();
        LinkableNode* getSecondLastNode();
        
    public:
        LinkableNode* getFirstNode();

};

