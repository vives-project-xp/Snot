#include "LinkedList2.h"
#include <sstream>



    LinkedList2::LinkedList2(){
        this->headNode = NULL;
    }

    void LinkedList2::addNode(Mission * data){
        if(this->headNode == NULL) headNode = new LinkableNode(data);
        else getLastNode()->setNextNode(new LinkableNode(data));
    }

    std::string LinkedList2::toString(){
        std::stringstream ss;
        ss << "Data in the linked list:" << std::endl;

        if(this->headNode == NULL){
            ss << "NO DATA IN LIST" << std::endl;
        }
        else{
            LinkableNode * last = this->headNode;
            while(last != NULL){
                ss << last->toString();
                last = last->getNextNode();
            }
            ss << std::endl;
        }

        return ss.str();
    }

    LinkableNode* LinkedList2::getLastNode(){
        LinkableNode * last = this->headNode;
        while(last->getNextNode() != NULL){
            last = last->getNextNode();
        }
        return last;
    }

    LinkableNode* LinkedList2::getSecondLastNode(){
        LinkableNode * last = this->headNode;
        while(last->getNextNode()->getNextNode() != NULL){
            last = last->getNextNode();
        }
        return last;
    }

    void LinkedList2::removeNode(){
        if(this->headNode == NULL){}         //Er is geen hoofdnode -> Niets verwijderen
        else if(this->headNode->getNextNode() == NULL){ //Er is alleen een hoofnode -> hoofdnode verwijderen
            delete this->headNode;
            this->headNode = NULL;
        }
        else{
            delete getSecondLastNode()->getNextNode();
            getSecondLastNode()->setNextNode(NULL);
        }
    }

    LinkedList2::~LinkedList2(){
        while(this->headNode != NULL){
            removeNode();
        }
    }
    
    LinkableNode* LinkedList2::getFirstNode(){
        return this->headNode;
    }

