# Here can u find code for MBED NFC reader

> Installation

1. Import project
2. cd ./Snot/mbed
3. mbed deploy
4. Connect NUCLEO_L476RG
5. >mbed compile -t GCC_ARM --flash
(You can also use make)
6. Everything is done!

> Working

The board communicates with the PN532 through the SPI protocol. On startup it enters a discovery mode in which it searches a NFC signal to pair itself to. After it's paired the cap only unlocks with the paired NFC tag. This process can be reset by presenting the master-tag configured for the system. The master tag opens the cap and puts it into the discovery mode.

Project was tested on:
compiler: 10.3.1

> mbed ls
  mbed ( revision in the current branch)
  `- mbed-os (#54e8693ef4ff, tags: mbed-os-6.16.0, mbed-os-6.16.0-rc1)
