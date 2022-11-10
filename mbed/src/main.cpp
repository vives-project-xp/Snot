/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "events/EventQueue.h"

#include "nfc/ndef/MessageBuilder.h"
#include "nfc/ndef/common/URI.h"
#include "nfc/ndef/common/util.h"

#include "NFCEEPROM.h"

// #include "EEPROMDriver.h"
#include <cstdio>

using events::EventQueue;

using mbed::Span;
using mbed::nfc::NFCEEPROM;
using mbed::nfc::NFCEEPROMDriver;

using mbed::nfc::ndef::MessageBuilder;
using mbed::nfc::ndef::common::span_from_cstr;
using mbed::nfc::ndef::common::URI;



#ifndef SOURCE_EEPROMDRIVER_H_
#define SOURCE_EEPROMDRIVER_H_

#include "nfc/NFCEEPROMDriver.h"
#include "m24sr_driver.h"
#include "events/EventQueue.h"

/**
 * Factory function that returns the NFC EEPROM driver.
 *
 * @param[in] queue The event queue that may be used by the driver.
 *
 * @return The NFC EEPROM driver to use.
 */
mbed::nfc::NFCEEPROMDriver& get_eeprom_driver(events::EventQueue& queue) {
    static mbed::nfc::vendor::ST::M24srDriver eeprom_driver;
    return eeprom_driver;
}

#endif /* SOURCE_EEPROMDRIVER_H_ */



/* URL that will be written into the tag */
const char url_string[] = "mbed.com";

class EEPROMExample : mbed::nfc::NFCEEPROM::Delegate
{
public:
    EEPROMExample(events::EventQueue &queue, NFCEEPROMDriver &eeprom_driver) : _ndef_buffer(),
                                                                               _eeprom(&eeprom_driver, &queue, _ndef_buffer),
                                                                               _queue(queue)
    {
    }

    void run()
    {
        if (_eeprom.initialize() != NFC_OK)
        {
            printf("failed to initialise\r\n");
            _queue.break_dispatch();
        }
        _eeprom.set_delegate(this);
        _queue.call(&_eeprom, &NFCEEPROM::read_ndef_message);
    }

private:
    virtual void on_ndef_message_written(nfc_err_t result)
    {
        if (result == NFC_OK)
        {
            printf("message written successfully\r\n");
        }
        else
        {
            printf("failed to write (error: %d)\r\n", result);
        }

        _queue.call(&_eeprom, &NFCEEPROM::read_ndef_message);
    }

    virtual void on_ndef_message_read(nfc_err_t result)
    {
        if (result == NFC_OK)
        {
            printf("message read successfully\r\n");
            /*
            if(!nfcLinked) {
                pairedID = _ndef_buffer;
            } else {
                if(pairedID == _ndef_buffer || _ndef_buffer == barID) {
                    unlock();
                }
            }
            */
        }
        else
        {
            printf("failed to read (error: %d)\r\n", result);
            //need to find a way to log resault in memory
        }
    }

    virtual void parse_ndef_message(const Span<const uint8_t> &buffer)
    {
        printf("Received an ndef message of size %d\r\n", buffer.size());
    }

    // virtual size_t build_ndef_message(const Span<uint8_t> &buffer)
    // {
    //     printf("Building an ndef message\r\n");

    //     /* create a message containing the URL */

    //     MessageBuilder builder(buffer);

    //     /* URI expected a non-null terminated string  so we use a helper function that casts
    //      * the pointer into a Span of size smaller by one */
    //     URI uri(URI::HTTPS_WWW, span_from_cstr(url_string));

    //     uri.append_as_record(builder, true);

    //     return builder.get_message().size();
    // }

private:
    uint8_t _ndef_buffer[1024];
    NFCEEPROM _eeprom;
    EventQueue &_queue;
};

int main()
{
    EventQueue queue;
    NFCEEPROMDriver &eeprom_driver = get_eeprom_driver(queue);

    EEPROMExample example(queue, eeprom_driver);

    example.run();
    queue.dispatch_forever();
    return 0;
}
