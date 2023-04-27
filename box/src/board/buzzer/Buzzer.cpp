
#include "Buzzer.h"

Buzzer::Buzzer() : pwm(PTA2){
    pwm = 0.0;
}

void Buzzer::buttonBeep(){
    playFreq(420, 100, 1);
}

void Buzzer::playFreq(float frequency, float duration, float volume){
        pwm.period(1.0/frequency);
        pwm = volume/2.0;
        ThisThread::sleep_for(std::chrono::milliseconds((int)duration));
        pwm = 0.0;
}

void Buzzer::startupSound(){
    playFreq(523, 200, 0.5); // C5
    playFreq(659, 200, 0.5); // E5
    playFreq(784, 200, 0.5); // G5
    playFreq(988, 200, 0.5); // B5
    playFreq(1174, 200, 0.5); // D6
    playFreq(1397, 200, 0.5); // F#6
    playFreq(1760, 200, 0.5); // A6
    playFreq(2093, 200, 0.5); // C7
    playFreq(3520, 200, 0.5); // A7
}

void Buzzer::keypadBeep(char button){
    switch(button){
        case '1': playFreq(300, 100, 0.5);  break;
        case '2': playFreq(350, 100, 0.5);  break;
        case '3': playFreq(400, 100, 0.5);  break;
        case '4': playFreq(450, 100, 0.5);  break;
        case '5': playFreq(500, 100, 0.5);  break;
        case '6': playFreq(550, 100, 0.5);  break;
        case '7': playFreq(600, 100, 0.5);  break;
        case '8': playFreq(650, 100, 0.5);  break;
        case '9': playFreq(700, 100, 0.5);  break;
        case '*': playFreq(750, 100, 0.5);  break;
        case '0': playFreq(800, 100, 0.5);  break;
        case '#': playFreq(850, 100, 0.5);  break;
    }
}

void Buzzer::completeSound(){
    playFreq(1318, 130, 1);
    playFreq(1567, 130, 1);
    playFreq(2637, 130, 1);
    playFreq(2093, 130, 1);
    playFreq(2349, 130, 1);
    playFreq(3135, 130, 1);
}

void Buzzer::failSound(){
    playFreq(800, 100, 1);
    playFreq(600, 100, 1);
    playFreq(400, 100, 1);
    playFreq(200, 200, 1);
}