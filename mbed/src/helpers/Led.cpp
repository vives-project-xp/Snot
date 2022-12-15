#include "Led.h"

<<<<<<< HEAD:mbed/src/helperes/Led.cpp
 // Led controls are reversed
namespace Helpers{

=======
namespace Helpers {
>>>>>>> 4cf0bf22739a69662e38cff24a679fff334074a4:mbed/src/helpers/Led.cpp
  Led::Led(PwmOut* red, PwmOut* green, PwmOut* blue): _red(red), _green(green), _blue(blue)
  {
    this->clear();
  }

<<<<<<< HEAD:mbed/src/helperes/Led.cpp
 
  void Led::red(void){
=======
  void Led::red(void) {
>>>>>>> 4cf0bf22739a69662e38cff24a679fff334074a4:mbed/src/helpers/Led.cpp
    _red->write(0);
    _green->write(1);
    _blue->write(1);
  }

  void Led::green(void) {
    _red->write(1);
    _green->write(0);
    _blue->write(1);
  }

  void Led::blue(void) {
    _red->write(1);
    _green->write(1);
    _blue->write(0);
  }

  void Led::clear(void) {
    _red->write(1);
    _green->write(1);
    _blue->write(1);
  }

<<<<<<< HEAD:mbed/src/helperes/Led.cpp
  void Led::RGB(int red, int green, int blue){
    // guards
=======
  void Led::RGB(int red, int green, int blue) {
>>>>>>> 4cf0bf22739a69662e38cff24a679fff334074a4:mbed/src/helpers/Led.cpp
    if(red > 255 || red < 0) return;
    if(green > 255 || green < 0) return;
    if(blue > 255 || blue < 0) return;
    
    // invert values and divide by 255 to get
    // value between 0 and 1
    int rValue = (255 - red) /255;
    int gValue = (255 - green)/255;
    int bValue = (255 - blue) /255;

    _red->write(rValue);
    _green->write(gValue);
    _blue->write(bValue);
  }
<<<<<<< HEAD:mbed/src/helperes/Led.cpp
}
=======
}
>>>>>>> 4cf0bf22739a69662e38cff24a679fff334074a4:mbed/src/helpers/Led.cpp
