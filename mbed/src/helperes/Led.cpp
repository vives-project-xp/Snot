#include "Led.h"


namespace Helpers{

  Led::Led(PwmOut* red, PwmOut* green, PwmOut* blue): _red(red), _green(green), _blue(blue)
  {
    
  }

  void Led::red(void){
    _red->write(0);
    _green->write(1);
    _blue->write(1);
  }

  void Led::green(void){
    _red->write(1);
    _green->write(0);
    _blue->write(1);
  }

  void Led::blue(void){
    _red->write(1);
    _green->write(1);
    _blue->write(0);

  }

  void Led::clear(void){
    _red->write(1);
    _green->write(1);
    _blue->write(1);
  }

  void Led::RGB(int red, int green, int blue){
    if(red > 255 || red < 0) return;
    if(green > 255 || green < 0) return;
    if(blue > 255 || blue < 0) return;

    int rValue = (255 - red) /255;
    int gValue = (255 - green)/255;
    int bValue = (255 - blue) /255;

    _red->write(rValue);
    _green->write(gValue);
    _blue->write(bValue);
   
  }


}