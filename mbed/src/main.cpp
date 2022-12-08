#include "mbed.h"
#include "pn532.h"
#include "Servo.h"
#include "helpers.h"


//add thread for led blinking
PN532 m_rfid(D11, D12, D13, D10);
Servo m_cap(D9);
Helpers::Helper helper(&m_rfid, &m_cap);

bool firstCard = true;
void loop();

uint8_t password[6] = { 0 };

int main() {
    printf("Hello!\r\n");
        // Checks if it can detect the PN532 shield
        uint32_t versiondata = m_rfid.getFirmwareVersion();
        if (!versiondata) {
            printf("Didn't find PN53x board\r\n");
            while(1);
        }

    printf("Found chip PN5%lx\r\n", ((versiondata>>24) & 0xFF));
    printf("Firmware ver. %lu.%lu\r\n", (versiondata>>16) & 0xFF, (versiondata>>8) & 0xFF);

    m_rfid.SAMConfig();

    printf("Waiting for an ISO14443A Card ...\r\n");

    while (1) {
        loop();
    }
}


void loop() {
    
    bool success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint8_t keyA[6] = { 0x31, 0x32, 0x33, 0x00,0x00,0x00};
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
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


    // default user keyA
    success = m_rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 8, 0, keyA);
    if (!success) {helper.error(uid, uidLength, cardid); return;}

    uint8_t data[16];

    // Try to read password from block 4
    success = m_rfid.mifareclassic_ReadDataBlock(8, data);
    if (!success) {helper.error(uid, uidLength, cardid); return; }
    
    // Display some basic information about the card
    // defaultCardInfo(uid, uidLength, cardid);

    // check master card
    if(helper.checkMaster(data)){
        firstCard = true;
        helper.goodCard();
        return;
    }

    // if firstcard save password
    if(firstCard){
        for(int i = 0; i < 6; i++){
            password[i] = data[i];
        }
        printf("Saved new card\r\n");
        firstCard = false;
        ThisThread::sleep_for (200ms);
        return;
    }

    // check if password in block 4 is correct
    for(int i = 0; i < 6; i++){ // later check for crash
        if(data[i] != password[i]){
            printf("Wrong card\r\n");
            ThisThread::sleep_for (200ms);
            return;
        }
    }

    helper.goodCard();

}
