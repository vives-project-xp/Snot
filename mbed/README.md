Here can u find code for MBED NFC reader

> Installation

1. Import project to online mbed compiler
2. Compile project
3. flash bin to NUCLEO_L476RG

> Working

The board communicates with the PN532 through the SPI protocol. On startup it enters a discovery mode in which it searches a NFC signal to pair itself to. After it's paired the cap only unlocks with the paired NFC tag. This process can be reset by presenting the master-tag configured for the system. The master tag opens the cap and puts it into the discovery mode.
