#include "EEPROMDriver.h"
#include "m24sr_driver.h"

mbed::nfc::NFCEEPROMDriver& get_eeprom_driver(events::EventQueue&)
{
    static mbed::nfc::vendor::ST::M24srDriver eeprom_driver;
    return eeprom_driver;
}