#pragma once

#include "Leds.h"
#include "DisplayX.h"
#include "Keys.h"
#include "Buzzer.h"
#include "NumberPad.h"
#include "mbed_events.h"


class Board{

    public:
        Leds * leds;
        DisplayX * lcd;
        Keys * keys;
        Buzzer * buzzer;
        NumberPad * numberPad;

    public:
        Board(EventQueue* queue);
        void buttonEvent(uint32_t index);


};
