#include "LinkableNode.h"


#include <sstream>


    LinkableNode::LinkableNode(Mission * data){
        this->data = data;
        this->nextNode = NULL;
    }

    std::string LinkableNode::toString(void){
        std::stringstream ss;
        ss << "Data = " << "NOT SUPPORTED YET" << std::endl;
        return ss.str();
    }

    void LinkableNode::setNextNode(LinkableNode * node){
        this->nextNode = node;
    }

    LinkableNode* LinkableNode::getNextNode(){
        return this->nextNode;
    }
    
    Mission* LinkableNode::getData(){
        return this->data;
    }

    LinkableNode::~LinkableNode(){

    }
