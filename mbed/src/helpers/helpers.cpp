#include "helpers.h"

namespace Helpers {
    Helper::Helper(PN532 *rfid, Servo *cap): rfid(rfid), cap(cap), breach(D7){

    };

    Helper::Helper(PN532 *rfid, Servo *cap, Led *led): rfid(rfid), cap(cap), led(led), breach(D7) {

        breachThread.start(callback(Helper::breachFun, this));
        
    };

    // Error log
    void Helper::defaultCardInfo(uint8_t *uid, uint8_t uidLength, uint32_t cardid) {
        printf("Found an ISO14443A card\r\n");
        printf("  UID Length: %d bytes\r\n", uidLength);
        printf("  UID Value: ");
        rfid->PrintHex(uid, uidLength);

        printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
        printf("\r\n");
    }

    // Error log
    void Helper::error(uint8_t *uid, uint8_t uidLength, uint32_t cardid) { 
        printf("Found an ISO14443A card\r\n");
        printf("  UID Length: %d bytes\r\n", uidLength);
        printf("  UID Value: ");
        rfid->PrintHex(uid, uidLength);

        printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
        printf("Ooops ... unable to read the requested block.  Try another key?\r\n");
        printf("\r\n");
    } 

    // Error log
    void Helper::unsupportedCard(uint8_t *uid, uint8_t uidLength) {
        printf("Found an ISO14443A card\r\n");
        printf("  UID Length: %d bytes\r\n", uidLength);
        printf("  UID Value: ");
        rfid->PrintHex(uid, uidLength);

        printf("Unsupported card type\r\n");
        printf("\r\n");
    }

    // Call it only after authorization
    // Check's if password is correct
    bool Helper::checkMaster(uint8_t data[]) {
        uint8_t masterPass[3] { 0x34,0x35,0x36 };

        //check password
        for(int i = 0; i < 3; i++){
            if(data[i] != masterPass[i]){
                return false;
            }
        }
        // flag sets to reset led after picking up without NFC
        flag.set(1);
        printf("Found master card\r\n");
        flag.set(0);
        return true;
    }

    void Helper::closeCap() {
        cap->write(0.5);
        led->red();
        ThisThread::sleep_for(100ms);
        led->clear();
    }

    void Helper::openCap() {
        cap->write(0);
    }

    // KeyA setter
    bool Helper::setKey(uint8_t uid[], uint8_t uidLength, uint8_t password[]) {
        uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };

        bool success = rfid->mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
        if (!success) {
            printf("Couldn't authenticate, try other key?\n");
            return false;
        }
        
        uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        // set new KeyA
        success = rfid->mifareclassic_WriteDataBlock(11, newKeyA);

        if (!success) {
            printf("Something went wrong and couldn't write new Key\n");
            return false;
        }

        // write new user password to block 8
        success = rfid->mifareclassic_WriteDataBlock(8, password);
        if (!success) {
            printf("Couldn't change password");
            return false;
        }
        printf("Succesfully set password\n");
        
        return true;
    }

    // Set default master card
    bool Helper::setMaster(uint8_t uid[], uint8_t uidLength) {
        uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
        uint8_t masterPass[3] { 0x34,0x35,0x36 };
        bool success = rfid->mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
        if (!success){
            printf("Couldn't authenticate, try other key?\n");
            return false;
        }

        // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
        uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        // set new KeyA
        success = rfid->mifareclassic_WriteDataBlock(11, newKeyA);

        if (!success) {
            printf("Something went wrong and couldn't write new Key\n");
            return false;
        }

        // write new user password to block 4
        success = rfid->mifareclassic_WriteDataBlock(8, masterPass);
        if (!success) {
            printf("Couldn't change password");
            return false;
        }
        printf("Succesfully set password\n");
        
        return true;
    }

    // Set card with default password
    bool Helper::setGood(uint8_t uid[], uint8_t uidLength ) {
        uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
        uint8_t password[6] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };
        bool success = rfid->mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
        if (!success) {
            printf("Couldn't authenticate, try other key?\n");
            return false;
        }

        // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
        uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        // set new KeyA
        success = rfid->mifareclassic_WriteDataBlock(11, newKeyA);

        if (!success) {
            printf("Something went wrong and couldn't write new Key\n");
            return false;
        }

        // write new user password to block 4
        success = rfid->mifareclassic_WriteDataBlock(8, password);
        if (!success) {
            printf("Couldn't change password");
            return false;
        }
        printf("Succesfully set password\n");
        
        return true;
    }

    // set card with different password
    bool Helper::setBad(uint8_t uid[], uint8_t uidLength ) {
        uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
        uint8_t password[6] = {0x31,0x32,0x33,0x35,0x34,0x36};
        bool success = rfid->mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
        if (!success) {
            printf("Couldn't authenticate, try other key?\n");
            return false;
        }

        // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
        uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        // set new KeyA
        success = rfid->mifareclassic_WriteDataBlock(11, newKeyA);

        if (!success) {
            printf("Something went wrong and couldn't write new Key\n");
            return false;
        }

        // write new user password to block 4
        success = rfid->mifareclassic_WriteDataBlock(8, password);
        if (!success) {
            printf("Couldn't change password");
            return false;
        }
        printf("Succesfully set password\n");
        
        return true;
    }

    void Helper::setLed(Led * led) {
      this->led = led;
    }

    void Helper::blinkLedGood() {
      // auto c_this = (Helper *) arg;
      led->green();
      ThisThread::sleep_for(1000ms);
      led->clear();
      ThisThread::sleep_for(1000ms);
    }

    void Helper::blinkLedBad() {
      // auto c_this = (Helper *) arg;
      for(int i = 0; i < 10; i++){
        led->red();
        ThisThread::sleep_for(100ms);
        led->clear();
        ThisThread::sleep_for(100ms);
      }
    }

    void Helper::goodCard(void) {
      openCap();
      printf("good card\r\n");
      blinkLedGood();
      closeCap();
    }

    void Helper::badCard(void) {
		flag.set(0);
      closeCap();
      printf("Wrong card\r\n");
      blinkLedBad();
    }

    void Helper::breachFun(void* arg){
        auto c_this = (Helper*) arg;
				uint32_t flag_read;
        while(1){
            // check for breachButton on D7
            if(c_this->breach.read() == 1){
                ThisThread::sleep_for(500ms);
                continue;
            }
            
             // if cap open w/o nfc > RED
             flag_read = c_this->flag.wait_any_for(0 || 1, 1ms);
            if(flag_read != 1){
                while(flag_read != 1){
                    flag_read = c_this->flag.wait_any_for(0 || 1, 1ms);
                    c_this->led->red();
                    ThisThread::sleep_for(50ms);
                    c_this->led->clear();
                    ThisThread::sleep_for(50ms);
                }
            }

            // wait for cap to be closed
            
            while(c_this->breach.read() == 0){
                ThisThread::sleep_for(50ms);
            }
        
            ThisThread::sleep_for(500ms);
        }
    }
}
