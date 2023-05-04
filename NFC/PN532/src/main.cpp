#include "PN532.h"

PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);

void setup(void)
{
	nfc.begin();
	//...
}