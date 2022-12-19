#include "mbed.h"
#include "pn532.h"
#include "Servo.h"
#include "helpers.h"
#include "Led.h"

//add thread for led blinking
PN532 m_rfid(D11, D12, D13, D10);
Servo m_cap(D9);

// change it to be inside Led object
PwmOut red(PA_1); 
PwmOut green(PB_0);
PwmOut blue(D6);  // should be PB_3

Helpers::Led led(&red, &green, &blue);
Helpers::Helper helper(&m_rfid, &m_cap, &led);

bool firstCard = true;
void loop();

uint8_t password[6] = { 0 };

int main() {
    printf("Hello!\r\n");
        // Checks if it can detect the PN532 shield
        // if not found check if you use SPI or I2C
        // and your wires
        uint32_t versiondata = m_rfid.getFirmwareVersion();
        if (!versiondata) {
            printf("Didn't find PN53x board\r\n");
            helper.led->blue();
            while(1);
        }

    printf("Found chip PN5%lx\r\n", ((versiondata>>24) & 0xFF));
    printf("Firmware ver. %lu.%lu\r\n", (versiondata>>16) & 0xFF, (versiondata>>8) & 0xFF);

    m_rfid.SAMConfig();

    printf("Waiting for an ISO14443A Card ...\r\n");

    while (true) {
        loop();
    }
}

// Main loop
void loop() {
    bool success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint8_t keyA[6] = { 0x31, 0x32, 0x33, 0x00,0x00,0x00};

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)

    // tldr; waits for card
    success = m_rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (uidLength != 4) {helper.unsupportedCard(uid, uidLength); return;} 
    // We probably have a Mifare Classic card ...
    uint32_t cardid = uid[0];
    cardid <<= 8;
    cardid |= uid[1];
    cardid <<= 8;
    cardid |= uid[2];
    cardid <<= 8;
    cardid |= uid[3];

    // To set cards uncoment single line of your choice and return.
    // If the card is set you will see prompt in terminal
    // after this it will fail because keyA will be changed
    
    // helper.setMaster(uid, uidLength);
    // helper.setGood(uid, uidLength);
    // helper.setBad(uid, uidLength);
    // return;

    // Authenticate with default keyA set by me :)
    success = m_rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 8, 0, keyA);
    if (!success) {helper.error(uid, uidLength, cardid); return;}

    uint8_t data[16];

    // Try to read password from block 4
    success = m_rfid.mifareclassic_ReadDataBlock(8, data);
    if (!success) {helper.error(uid, uidLength, cardid); return; }
    
    // Uncomment to display some basic information about the card
    // helper.defaultCardInfo(uid, uidLength, cardid);

    // check master card
    // if master card found, look for new card to save passowrd
    if(helper.checkMaster(data)) {
        firstCard = true;
        helper.openCap();
        // Show with lights that master card is found
        for(int i = 0; i < 3; i++){
            helper.led->green();
            ThisThread::sleep_for(50ms);
            helper.led->red();
            ThisThread::sleep_for(50ms);
        }
        helper.led->clear();
        return;
    }

    // if firstcard save password inside
    if(firstCard) {
        for(int i = 0; i < 6; i++){
            password[i] = data[i];
            helper.led->green();
            ThisThread::sleep_for(50ms);
            helper.led->clear();
            ThisThread::sleep_for(50ms);
            
        }
        
        printf("Saved new card\r\n");
        firstCard = false;
        ThisThread::sleep_for (200ms);
        return;
    }

    // check if password in block 4 is correct
    for(int i = 0; i < 6; i++) { // later check for crash
        if(data[i] != password[i]){
            helper.badCard();
            ThisThread::sleep_for (200ms);
            return;
        }
    }

    helper.goodCard();
}
