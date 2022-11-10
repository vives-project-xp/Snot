// #include "monkeyproof.h"
#include "mbed.h"
#include "Servo.h"
 
Servo myservo(D9);
InterruptIn cap_button(D7);
InterruptIn NFC(D2);
DigitalOut led(D4);
Thread thread1;
Thread thread2;
bool isSpiked = false;
bool isUnlocked = false;
bool capIsOn = false;
bool threadStarted = false;

void loss() {
    myservo.write(0.2);
}

void spike() {
    led.write(1);
    isSpiked = true;
    capIsOn = false;
}

void tighten() {
    if (!isUnlocked) {
        cap_button.rise(spike);
        isUnlocked = false;
        capIsOn = true;
        myservo.write(0.45);
    }
}

void closeCapAfterNFC() {
    ThisThread::sleep_for(5s);
    tighten();
    threadStarted = true;
}

void nfcAway() {
    isUnlocked = false;
}

void closeThread() {
    // thread1.terminate();
}

void notspike() {
    capIsOn = false;
    // thread2.start(closeThread);
}

void detect() {
    isUnlocked = true;
    if (!isSpiked) {
        loss();
        cap_button.rise(notspike);
        // thread1.start(closeCapAfterNFC);
    }
}

int main() {
    led.write(0);
    myservo.write(0);
    cap_button.fall(tighten);
    cap_button.rise(spike);
    NFC.fall(detect);
    NFC.rise(nfcAway);

    while (true) {
        if (!threadStarted && isUnlocked && capIsOn) {
            printf("KHA");
            thread1.start(closeCapAfterNFC);
        }

    }
    return 0;
}
