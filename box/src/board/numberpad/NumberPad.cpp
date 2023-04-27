#include "NumberPad.h"

const char NumberPad::KEYS[] = {'1','2','3','4','5','6','7','8','9','*','0','#'};

NumberPad::NumberPad(EventQueue* queue) : Keypad(queue, PTC4, PTB23, PTC2, PTE26, PTB20, PTB2, PTC3, NC){

}

void NumberPad::attach(mbed::Callback<void(uint32_t)> cb) {
    _callback = cb;
}