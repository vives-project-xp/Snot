#define MBED_CONF_RTOS_PRESENT 1
#include "mbed.h"
#include "rtos/Thread.h"

#include "pn532.h"
#include "Servo.h"


DigitalOut leds(LED1);

PN532 rfid(D11, D12, D13, D10);
Servo cap(D9);

bool firstCard = true;

void loop();

void defaultCardInfo(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
void goodCard(void);
bool setKey(uint8_t uid[], uint8_t uidLength, uint8_t password[]);

void error(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
void unsupportedCard(uint8_t *uid, uint8_t uidLength);
bool checkMaster(uint8_t uid[], uint8_t uidLength);

// servo function
void openCap(void);
void closeCap(void);

int main() {
    printf("Hello!\r\n");

    uint32_t versiondata = rfid.getFirmwareVersion();
    if (!versiondata) {
        printf("Didn't find PN53x board\r\n");
        while (1); // halt
    }

    printf("Found chip PN5%lx\r\n", ((versiondata>>24) & 0xFF));
    printf("Firmware ver. %lu.%lu\r\n", (versiondata>>16) & 0xFF,
                                    (versiondata>>8) & 0xFF);

    rfid.SAMConfig();

    printf("Waiting for an ISO14443A Card ...\r\n");

    while (1) {
        loop();
    }
}


void loop() {
    
    uint8_t success, i;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    static uint8_t lastUID[7];
    static uint8_t lastUIDLength;    
    uint8_t password[6] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 }; // password set to 4th block

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    
    // if first card save uid

    if (firstCard){
        for (i = 0; i < uidLength; i++) {
            lastUID[i] = uid[i];
        }
        lastUIDLength = uidLength;
        printf("saved new card\r\n");
        firstCard = false;
        wait_ms(300);
        return;
    }
    
    
    if (uidLength != 4) {unsupportedCard(uid, uidLength); return;} 
    // We probably have a Mifare Classic card ...
    uint32_t cardid = uid[0];
    cardid <<= 8;
    cardid |= uid[1];
    cardid <<= 8;
    cardid |= uid[2];
    cardid <<= 8;
    cardid |= uid[3];

    // default user keyA
    uint8_t userKeyA[3] = {0x31, 0x32, 0x33};
    success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, userKeyA);
    // if authentication doesn't work there is probably default keyA > change it
    if (!success) { 
		// default user pass didn't work? check master first
        if(checkMaster(uid, uidLength)){
            firstCard = true;
            wait_ms(200);
            return;
        }
        // Only for this example, else wrong key
        // not master? try change manufacture key
		// if(!setKey(uid, uidLength, password)){
        //     wait_ms(1000);
        //     return;
        // }
	}
            
    uint8_t data[16];

    // Try to read password from block 4
    success = rfid.mifareclassic_ReadDataBlock(4, data);
    if (!success) { error(uid, uidLength, cardid); return; }
    
    // Display some basic information about the card
    defaultCardInfo(uid, uidLength, cardid);

    // check if password in block 4 is correct
    for(int i = 0; i < 6; i++){
        if(data[i] != password[i]){
            printf("Wrong card\n");
            wait_ms(200);
            return;
        }
    }

    goodCard();
    
    wait_ms (200);
    
}

void error(uint8_t *uid, uint8_t uidLength, uint32_t cardid) { 

    printf("Found an ISO14443A card\r\n");
    printf("  UID Length: %d bytes\r\n", uidLength);
    printf("  UID Value: ");
    rfid.PrintHex(uid, uidLength);

    printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
    printf("Ooops ... unable to read the requested block.  Try another key?\r\n");
    printf("\r\n");
}

void unsupportedCard(uint8_t *uid, uint8_t uidLength){

    printf("Found an ISO14443A card\r\n");
    printf("  UID Length: %d bytes\r\n", uidLength);
    printf("  UID Value: ");
    rfid.PrintHex(uid, uidLength);

    printf("Unsupported card type\r\n");
    printf("\r\n");
}

void defaultCardInfo(uint8_t *uid, uint8_t uidLength, uint32_t cardid){
    printf("Found an ISO14443A card\r\n");
    printf("  UID Length: %d bytes\r\n", uidLength);
    printf("  UID Value: ");
    rfid.PrintHex(uid, uidLength);

    printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
    printf("\r\n");
}

// Add blinking LED on new thread > on wrong/right card?
void goodCard(void){
    openCap();
    printf("That's a good card!\r\n");
   
    wait_ms (2000); // Need check comfortable wait time
    closeCap();
}

bool checkMaster(uint8_t uid[], uint8_t uidLength){
    
    uint8_t masterKey[] { 0x31,0x32,0x33 };
    uint8_t masterPass[] { 0x34,0x35,0x36 };

    bool success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, masterKey);
    
    if(!success) return false;

    success = rfid.mifareclassic_WriteDataBlock(4, masterPass);

    if(!success) {
        printf("Something went wrong ask Szymon <3");
        return false;
    }
    return true;
}

void closeCap() {
    cap.write(0.5);
}

void openCap(){
    cap.write(0);
}

// call on authy error with *new* default Key
bool setKey(uint8_t uid[], uint8_t uidLength, uint8_t password[]){
	uint8_t oldKeyA[] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	bool success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 7, 0, oldKeyA);
	if(!success){
        printf("Couldn't authenticate, try other key?\n");
		return false;
	}


    // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
	uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0xFF, 0x07, 0x80, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    // set new KeyA
    success = rfid.mifareclassic_WriteDataBlock(7, newKeyA);

    if(!success){
        printf("Something went wrong and couldn't write new Key\n");
        return false;
    }

    // write new user password to block 4
    // rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, newKeyA);
    success = rfid.mifareclassic_WriteDataBlock(4, password);
    if(!success){
        printf("Couldn't change password");
        return false;
    }
    printf("Succesfully set password\n");
    
    return true;
}