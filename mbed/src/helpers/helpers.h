#pragma once
#include "mbed.h"
#include "pn532.h"
#include "Servo.h"
#include "Led.h"
#include "Thread.h"
#include "EventFlags.h"

namespace Helpers {
  class Helper {
        public:
            Helper(PN532 *rfid, Servo *cap);
            Helper(PN532 *rfid, Servo *cap, Led* led);
            // rfid functions from pn532.h
            void defaultCardInfo(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
            void error(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
            void unsupportedCard(uint8_t *uid, uint8_t uidLength);

            // setters for tags add desc readme good vs bad
            bool setMaster(uint8_t *uid, uint8_t uidLength);
            bool setGood(uint8_t *uid, uint8_t uidLength);
            bool setBad(uint8_t *uid, uint8_t uidLength);
            // set custom password for tag
            bool setKey(uint8_t uid[], uint8_t uidLength, uint8_t password[]);

            // servo functions
            void openCap(void);
            void closeCap(void);

            // Call when a good card is detected, opens the servo in the cap
            void goodCard(void);
            void badCard(void);
            // Checks if presented tag is a master tag
            bool checkMaster(uint8_t data[]);

            // set Pointer to Led object
            void setLed( Led *led);
            Led* led;

            // Breach Thread
            static void breachFun(void* arg);

        private:
            bool NFCUsed = false;
            EventFlags flag;
            
            void blinkLedGood(void);
            void blinkLedBad(void);

            rtos::Thread ledThread;
            PN532* rfid;
            Servo* cap;

            rtos::Thread breachThread;
            DigitalIn breach;
    };
}
