#pragma once

#include <string>
#include "Mission.h"


    class LinkableNode{

    private:
        Mission * data;
        LinkableNode * nextNode;

    public:
        LinkableNode(Mission * data);
        ~LinkableNode();

    public:
        void setNextNode(LinkableNode * node);

    public:
        LinkableNode* getNextNode();
        Mission* getData();
        std::string toString();
    };

