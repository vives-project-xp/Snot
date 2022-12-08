#include "mbed.h"
#include "pn532.h"
#include "Servo.h"


DigitalOut leds(LED1);

PN532 rfid(D11, D12, D13, D10);
Servo cap(D9);

bool firstCard = true;

void loop();

void defaultCardInfo(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
void goodCard(void);
bool setKey(uint8_t uid[], uint8_t uidLength );

void error(uint8_t *uid, uint8_t uidLength, uint32_t cardid);
void unsupportedCard(uint8_t *uid, uint8_t uidLength);
bool checkMaster(uint8_t data[]);
// servo function
void openCap(void);
void closeCap(void);

bool setMaster(uint8_t *uid, uint8_t uidLength);
bool setGood(uint8_t *uid, uint8_t uidLength);
bool setBad(uint8_t *uid, uint8_t uidLength);
uint8_t password[6] = { 0 }; // default password
int main() {
    printf("Hello!\r\n");

   
        uint32_t versiondata = rfid.getFirmwareVersion();
        if (!versiondata) {
            printf("Didn't find PN53x board\r\n");
            while(1);
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
    uint8_t keyA[6] = { 0x31, 0x32, 0x33, 0x00,0x00,0x00};
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    
    
    if (uidLength != 4) {unsupportedCard(uid, uidLength); return;} 
    // We probably have a Mifare Classic card ...
    uint32_t cardid = uid[0];
    cardid <<= 8;
    cardid |= uid[1];
    cardid <<= 8;
    cardid |= uid[2];
    cardid <<= 8;
    cardid |= uid[3];




    // setMaster(uid,uidLength);
    // setGood(uid,uidLength);
    // setBad(uid, uidLength);
    // return;



    // default user keyA
    success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 8, 0, keyA);
    if (!success) {error(uid, uidLength, cardid); return;}

    uint8_t data[16];

    // Try to read password from block 4
    success = rfid.mifareclassic_ReadDataBlock(8, data);
    if (!success) { error(uid, uidLength, cardid); return; }
    
    // Display some basic information about the card
    // defaultCardInfo(uid, uidLength, cardid);

    // check master card
    if(checkMaster(data)){
        firstCard = true;
        goodCard();
        return;
    }

    // if firstcard save password
    if(firstCard){
        for(int i = 0; i < 6; i++){
            password[i] = data[i];
        }
        printf("Saved new card\r\n");
        firstCard = false;
        ThisThread::sleep_for (500);
        return;
    }

    // check if password in block 4 is correct
    for(int i = 0; i < 6; i++){ // later check for crash
        if(data[i] != password[i]){
            printf("Wrong card\r\n");
            ThisThread::sleep_for (200);
            return;
        }
    }

    goodCard();
    
    
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
    printf("good card\r\n");
    ThisThread::sleep_for  (2000); // Need check comfortable wait time
    closeCap();
}

bool checkMaster(uint8_t data[]){
    
    uint8_t masterPass[3] { 0x34,0x35,0x36 };

    //check password
    for(int i = 0; i < 3; i++){
        if(data[i] != masterPass[i]){
            return false;
        }
    }
    printf("Found master card\r\n");
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
//    uint8_t oldKeyA[5] = { 0x31, 0x32, 0x33, 0x34, 0x35 };
   uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };

   bool success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
   if(!success){
       printf("Couldn't authenticate, try other key?\n");
       return false;
   }


   // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
   uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   // set new KeyA
   success = rfid.mifareclassic_WriteDataBlock(7, newKeyA);

   if(!success){
       printf("Something went wrong and couldn't write new Key\n");
       return false;
   }

   // write new user password to block 4
   
   success = rfid.mifareclassic_WriteDataBlock(4, password);
   if(!success){
       printf("Couldn't change password");
       return false;
   }
   printf("Succesfully set password\n");
   
   return true;
}

bool setMaster(uint8_t uid[], uint8_t uidLength){

   uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    uint8_t masterPass[3] { 0x34,0x35,0x36 };
   bool success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
   if(!success){
       printf("Couldn't authenticate, try other key?\n");
       return false;
   }


   // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
   uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   // set new KeyA
   success = rfid.mifareclassic_WriteDataBlock(11, newKeyA);

   if(!success){
       printf("Something went wrong and couldn't write new Key\n");
       return false;
   }

   // write new user password to block 4
   success = rfid.mifareclassic_WriteDataBlock(8, masterPass);
   if(!success){
       printf("Couldn't change password");
       return false;
   }
   printf("Succesfully set password\n");
   
   return true;
}

bool setGood(uint8_t uid[], uint8_t uidLength ){
   uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    uint8_t password[6] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };
   bool success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
   if(!success){
       printf("Couldn't authenticate, try other key?\n");
       return false;
   }


   // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
   uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   // set new KeyA
   success = rfid.mifareclassic_WriteDataBlock(11, newKeyA);

   if(!success){
       printf("Something went wrong and couldn't write new Key\n");
       return false;
   }

   // write new user password to block 4
   
   success = rfid.mifareclassic_WriteDataBlock(8, password);
   if(!success){
       printf("Couldn't change password");
       return false;
   }
   printf("Succesfully set password\n");
   
   return true;
}

bool setBad(uint8_t uid[], uint8_t uidLength ){
   uint8_t oldKeyA[6] = { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    uint8_t password[6] = {0x31,0x32,0x33,0x35,0x34, 0x36};
   bool success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 11, 0, oldKeyA);
   if(!success){
       printf("Couldn't authenticate, try other key?\n");
       return false;
   }


   // uint8_t defaultAcces[4] = {0xFF, 0x07, 0x80, 0x69};
   uint8_t newKeyA[16] = { 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   // set new KeyA
   success = rfid.mifareclassic_WriteDataBlock(11, newKeyA);

   if(!success){
       printf("Something went wrong and couldn't write new Key\n");
       return false;
   }

   // write new user password to block 4
   
   success = rfid.mifareclassic_WriteDataBlock(8, password);
   if(!success){
       printf("Couldn't change password");
       return false;
   }
   printf("Succesfully set password\n");
   
   return true;
}