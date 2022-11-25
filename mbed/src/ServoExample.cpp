#include "mbed.h"
#include "Servo.h"
 
 
PwmOut motorFwd(D5);
PwmOut motorRev(D6); 
Servo myservo(D9);
InterruptIn user_button(BUTTON1);

void tighten() {
    for(float p = 0.2; p<0.55; p += 0.1) {
        myservo.write(p);
        wait_us(200000);
    }
}

void loss() {
    myservo.write(0.2);
}

void motorOn() {
    printf("DRAAI!\n");
    motorFwd.write(1);
    motorRev.write(0);
}

void motorOff() {
    printf("STOP!\n");
    motorFwd.write(0);
    motorRev.write(0);
}

int main() {
    // user_button.fall(motorOn);
    // user_button.rise(motorOff);

    user_button.rise(loss);
    user_button.fall(tighten);
}