#include "Keys.h"

DigitalIn key(PTB3);

Keys::Keys(){
        
}

int Keys::isKeyTurned(){
    return key.read();
}