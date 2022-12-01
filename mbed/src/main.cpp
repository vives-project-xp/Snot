#include "mbed.h"
#include "pn532.h"
#include "Servo.h"

#define NOTE_A4  440
#define NOTE_G7  3136

DigitalOut leds(LED1);

PN532 rfid(D11, D12, D13, D10);
Timer readTimer;
//PwmOut speaker(p21);
Servo cap(D9);

bool firstCard = true;

void loop();

void defaultCardInfo(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
void goodCard(void);

void error(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
void unsupportedCard(uint8_t *uid, uint8_t uidLength);
bool newCard(void);
bool checkMaster(uint8_t data[16]);

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
    uint8_t newCardFound;    

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    
    // Compare and see if we are seeing a new card
    
    newCardFound = 0;

    if (success) {

        if (readTimer.read_ms() > 400) {
            newCardFound = 1;
        }
        readTimer.reset();
        // check later if it's needed
        // Check id lenght -> if the same,
        // check if values are the same
        if (uidLength != lastUIDLength) {
                newCardFound = 1;
        } 
        else {
            for (i = 0; i < uidLength; i++) {
                if (uid[i] != lastUID[i]) {
                    newCardFound = 1;
                    break;
                }
            }
        }
    }

    // Add new card to last seen

    if (newCardFound && firstCard){
        for (i = 0; i < uidLength; i++) {
            lastUID[i] = uid[i];
        }
        lastUIDLength = uidLength;
        printf("saved new card\r\n");
        leds = 1;
        firstCard = false;
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


    // Now we need to try to authenticate it for read/write access
    // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    uint8_t password[6] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };

    // Start with block 4 (the first block of sector 1) since sector 0
    // contains the manufacturer data and it's probably better just
    // to leave it alone unless you know what you're doing

    success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, password);

    if (!success) { 
		
		setPassword(uid, uidLength, 4, 0); 
	}
            
     uint8_t data[16];

    // If you want to write something to block 4 to test with, uncomment
    // the following line and this text should be read back in a minute
    uint8_t defaultAcces[] = {0xFF, 0x07, 0x80, 0x69};
    

    // Try to read the contents of block 4
     success = rfid.mifareclassic_ReadDataBlock(4, data);

    if (!success) { error(uid, uidLength, cardid); return; }
    
    leds = 0x6;

    // read data block #
    // printf("Reading Block 4: ");
	// rfid.PrintHexChar(data, 16);
	// printf("\r\n");

    // Display some basic information about the card
    defaultCardInfo(uid, uidLength, cardid);
    
    // if mastercard save the next seen card
    if(checkMaster(data)){
        firstCard = true;
        //wait_ms (500);
        return;
    }
    
    if(!firstCard){
        for (i = 0; i < uidLength; i++) {
            if(uid[i] != lastUID[i]){
                printf("Wrong Card!\r\n");
                //wait_ms (500);
                return;
            }   
        }   
    }

    goodCard();
    
    //wait_ms (200);
    leds = 0;

    readTimer.reset();
    readTimer.start();
    
}

void error(uint8_t *uid, uint8_t uidLength, uint32_t cardid) { 
    leds = 0x9;

    printf("Found an ISO14443A card\r\n");
    printf("  UID Length: %d bytes\r\n", uidLength);
    printf("  UID Value: ");
    rfid.PrintHex(uid, uidLength);

    printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
    printf("Ooops ... unable to read the requested block.  Try another key?\r\n");
    printf("\r\n");
}

void unsupportedCard(uint8_t *uid, uint8_t uidLength){
    leds = 0x9;

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
   
    //wait_ms (2000); // Need check comfortable wait time
    closeCap();
}

bool checkMaster(uint8_t data[16]){
    
    bool masterCard = false;
    uint8_t pass[] {0x31,0x32,0x33};
    for (int i = 0; i < 3; i++) {
        if(data[i] != pass[i]){
            masterCard = false;
            break;
        }
        else{
            masterCard = true;
        }
    }
    if(masterCard) {
        
        printf("Found master card\r\n");
        return true;   
    }
    return false;
}

void closeCap() {
    cap.write(0.5);
}

void openCap(){
    cap.write(0);
}

// call on authy error with *new* default Key
bool setPassword(uint8_t uid[], uint8_t uidLength, int blockNumber,  int keyType){
	uint8_t oldKeyA[] {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	bool success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, oldKeyA);
	if(!success){

		return false;
	}
	// authenticate 4th block with default manufacture Key = 6x 0xFF

	// write password to 4th block
	uint8_t password[6] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };

	// try to login with old KEY first if false return false
	// if writing new Key sucess -> return true
	
}