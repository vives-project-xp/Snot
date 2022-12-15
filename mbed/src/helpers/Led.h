// convert RGB (0-255) to V
#pragma once
#include "mbed.h"


<<<<<<< HEAD:mbed/src/helperes/Led.h
namespace Helpers{
  class Led{

=======
namespace Helpers {
  class Led {
>>>>>>> 4cf0bf22739a69662e38cff24a679fff334074a4:mbed/src/helpers/Led.h
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
<<<<<<< HEAD:mbed/src/helperes/Led.h
      PwmOut* _blue;
=======
      PwmOut* _blue;  
>>>>>>> 4cf0bf22739a69662e38cff24a679fff334074a4:mbed/src/helpers/Led.h
  };
}
