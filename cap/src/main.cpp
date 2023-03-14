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
        // cap_button.rise(spike);
        isUnlocked = false;
        capIsOn = true;
        myservo.write(0.45);
    }
    // ThisThread::sleep_for(1ms);
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
// This is main.cpp

// #include "mbed.h"
// #include "RGBLed.h"
// // See the program page for information on the timing numbers
// // The given numbers are for the K64F
// using namespace cap;

// RGBLed rgbLed(D3, D5, D6);

// void detectCard() {
//     for (int i = 0; i < 5; i++) {
//         rgbLed.setColor(1, 0, 0);
//         ThisThread::sleep_for(200ms);
//         rgbLed.setColor(0, 1, 0);
//         ThisThread::sleep_for(200ms);
//     }
// }

// void error() {
//     while (true) {
//         rgbLed.setColor(1, 0, 0);
//         ThisThread::sleep_for(20ms);
//         rgbLed.setColor(0, 0, 0);
//         ThisThread::sleep_for(20ms);
//     }
// }

// void tighten() {
//     rgbLed.setColor(1, 0, 0);
//     ThisThread::sleep_for(5000ms);
//     rgbLed.setColor(0, 0, 0);
// }

// void loss() {
//     rgbLed.setColor(0, 1, 0);
//     ThisThread::sleep_for(1000ms);
//     rgbLed.setColor(0, 0, 0);
// }

// int main() {
//     rgbLed.setColor(0, 0, 0);

//     // detectCard();
//     // error();

//     loss();
//     ThisThread::sleep_for(500ms);
//     rgbLed.setColor(0, 0, 0);
//     tighten();
//     rgbLed.setColor(0, 0, 0);

//     // rgbLed.setColor(1, 0, 0);
//     // while (true) {
//     //     rgbLed.setColor(1, 0, 0);
//     //     ThisThread::sleep_for(200ms);
//     //     rgbLed.setColor(0, 1, 0);
//     //     ThisThread::sleep_for(200ms);
//     // }
// }


// //mbed target NUCLEO_L476RG