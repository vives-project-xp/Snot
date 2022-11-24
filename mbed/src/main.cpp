#include "mbed.h"
#include "pn532.h"

#define NOTE_A4  440
#define NOTE_G7  3136

DigitalOut leds(LED1);
PN532 rfid(D11, D12, D13, D10);
Timer readTimer;
//PwmOut speaker(p21);

void loop();

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

//void beep(uint16_t note, uint16_t duration) {
//    speaker.period(1.0/float(note));
//    speaker = 0.25;
//    wait_ms(duration);
//    speaker = 0;
//}

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

        if (uidLength != lastUIDLength) {
            newCardFound = 1;
        } else {
            for (i = 0; i < uidLength; i++) {
                if (uid[i] != lastUID[i]) {
                    newCardFound = 1;
                    break;
                }
            }
        }
    }

    if (newCardFound) {
        for (i = 0; i < uidLength; i++) {
            lastUID[i] = uid[i];
        }
        lastUIDLength = uidLength;

        leds = 1;

        if (uidLength == 4) {
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
            uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

            // Start with block 4 (the first block of sector 1) since sector 0
            // contains the manufacturer data and it's probably better just
            // to leave it alone unless you know what you're doing
            success = rfid.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

            if (success) {
                uint8_t data[16];

                // If you want to write something to block 4 to test with, uncomment
                // the following line and this text should be read back in a minute
                // memcpy(data, (const uint8_t[]){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xE, 0xC, 0xE, 4, 1, 8, 0 }, sizeof data);
                // success = rfid.mifareclassic_WriteDataBlock (4, data);

                // Try to read the contents of block 4
                success = rfid.mifareclassic_ReadDataBlock(4, data);

                // printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
                if (success) {
                    // Data seems to have been read ... spit it out
                    leds = 0x6;
                    //////////////////////////////beep(NOTE_G7, 200);

                    // Display some basic information about the card
                    printf("Found an ISO14443A card\r\n");
                    printf("  UID Length: %d bytes\r\n", uidLength);
                    printf("  UID Value: ");
                    rfid.PrintHex(uid, uidLength);

                    printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);

                    printf("Reading Block 4: ");
                    rfid.PrintHexChar(data, 16);
                    printf("\r\n");
                } else {
                    leds = 0x9;
                    ////////////beep(NOTE_A4, 800);

                    printf("Found an ISO14443A card\r\n");
                    printf("  UID Length: %d bytes\r\n", uidLength);
                    printf("  UID Value: ");
                    rfid.PrintHex(uid, uidLength);

                    printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
                    printf("Ooops ... unable to read the requested block.  Try another key?\r\n");
                    printf("\r\n");
                }
            } else {
                leds = 0x9;
                //////////beep(NOTE_A4, 800);

                printf("Found an ISO14443A card\r\n");
                printf("  UID Length: %d bytes\r\n", uidLength);
                printf("  UID Value: ");
                rfid.PrintHex(uid, uidLength);

                printf("Seems to be a Mifare Classic card #%lu\r\n", cardid);
                printf("Ooops ... authentication failed: Try another key?\r\n");
                printf("\r\n");
            }
        } else {
            leds = 0x9;
            ////////beep(NOTE_A4, 800);

            printf("Found an ISO14443A card\r\n");
            printf("  UID Length: %d bytes\r\n", uidLength);
            printf("  UID Value: ");
            rfid.PrintHex(uid, uidLength);

            printf("Unsupported card type\r\n");
            printf("\r\n");
        }

        wait_ms(200);
        leds = 0;

        readTimer.reset();
        readTimer.start();
    }
}
