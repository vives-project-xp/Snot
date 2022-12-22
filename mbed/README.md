# Here can u find code for MBED NFC reader

## Installation

1. Import project
2. cd ./Snot/mbed
3. mbed deploy
4. Connect NUCLEO_L476RG
5. > mbed compile -t GCC_ARM --flash
(You can also use make)
6. Everything is done!

## Working

The board communicates with the PN532 through the SPI protocol. On startup it enters a discovery mode in which it searches a NFC signal to pair itself to. After it's paired the cap only unlocks with the paired NFC tag. This process can be reset by presenting the master-tag configured for the system. The master tag opens the cap and puts it into the discovery mode.

## Version

Project was tested on:
compiler: 10.3.1

> mbed ls
  mbed ( revision in the current branch)
  `- mbed-os (#54e8693ef4ff, tags: mbed-os-6.16.0, mbed-os-6.16.0-rc1)

## How does the code work?

### Helper Object

Helper class contains most of the functions used in the Main loop just to keep
main.cpp cleaner and make things easier to find.

### Led Object

Contains led controls methods.

### pn532 Library

Library that contains all the RFID methods and communication between
Nucleo and pn532.

### Main Loop

Check comments <3

## Mifare classic 1k

[Download link to PDF for Mifare datasheet](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwinrZe82vv7AhWFOOwKHfq8ARMQFnoECA0QAQ&url=https%3A%2F%2Fshop.sonmicro.com%2FDownloads%2FMIFARECLASSIC-UM.pdf&usg=AOvVaw2tvONHw50oQ-4uPCvlGSDR)

On page 3 you can see memory map for this card.
As u can see this card has 16 sectors with 4 blocks in each sector.

In first sector is manufacturer data that is read-only

In first 3 blocks of each sector u can put any data u want within 2bytes.
The 4th block is serverd for keyA, Access Bits and keyB.

KeyA and KeyB are sort of "passwords" for the entire sector.
ex. If you would change KeyA in block 7, you'll have to authenticate
blocks from 4-7 with the new key.

Access bits change how they keys are read so if u don't know what you're doing
best keep these the way they or you can lock your self out of the sector.

The way we work with the card is we first use the helper.set... method to set each card with
default values for everyone of them.

By first changing every cards keyA to sector2 to other than manufacturers default key
and put password in the 8th block.

## Light Signals

Fast blinking green light = saving new card
Green light = good card

Flashing red light = bad card

Flashing green and red light = master card

Constant flashing red light = Opened cap without NFC card
To reset close cap and present master card

Constant blue light = pn532 board not found
