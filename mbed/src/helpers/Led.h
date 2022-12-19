// convert RGB (0-255) to V
#pragma once
#include "mbed.h"


namespace Helpers {
    class Led {
        public:
            Led(PwmOut* red, PwmOut* green, PwmOut* blue);
            
            // preset colors
            
            void red();
            void green();
            void blue();
            void clear();

            // RGB value 0-255
            void RGB(int red, int green, int blue);

        private:
            PwmOut* _red;
            PwmOut* _green;
            PwmOut* _blue;
    };
}
