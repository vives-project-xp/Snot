#pragma once

#include "mbed.h"
#include "Keypad.h"
#include "mbed_events.h"

class NumberPad : public Keypad{

    public:
        NumberPad(EventQueue* queue);

    public:
        void runThisMethod(int index);
        void attach(mbed::Callback<void(uint32_t)>);

        static const char KEYS[];
};
