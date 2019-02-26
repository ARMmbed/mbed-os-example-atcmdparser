/* ATCmdParser usage example
 * Copyright (c) 2019 ARM Limited
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

#include "mbed.h"
#include "ATCmdParser.h"
#include "UARTSerial.h"

#define   PARSER_MODE                 CLIENT
#define   ESP8266_DEFAULT_BAUD_RATE   115200

ATCmdParser *_parser;

int client_func()
{
    printf("\nATCmdParser Client with ESP8266 example");

    UARTSerial *_serial = new UARTSerial(D1, D0, ESP8266_DEFAULT_BAUD_RATE);
    _parser = new ATCmdParser(_serial);
    _parser->debug_on( 1 );
    _parser->set_delimiter( "\r\n" );
    
    //Now get the FW version number of ESP8266 by sending an AT command 
    printf("\nATCmdParser: Retrieving FW version");
    _parser->send("AT+GMR");
    int version;
    if(_parser->recv("SDK version:%d", &version) && _parser->recv("OK")) {
        printf("\nATCmdParser: FW version: %d", version);
        printf("\nATCmdParser: Retrieving FW version success");
    } else { 
        printf("\nATCmdParser: Retrieving FW version failed");
        return -1;
    }

    printf("\nDone\n");
    return 0;
}

void atcmd_server_cb_test()
{
    int val;
    _parser->scanf(",%d;", &val);
    _parser->send("\r\n%s: val=%d\n", __func__, val);
}

void atcmd_server_cb_run()
{
    _parser->send("\r\n%s: OK\n", __func__);
}

int server_func()
{
    Serial *_serial = new Serial(D1, D0, NULL, ESP8266_DEFAULT_BAUD_RATE);
    _parser = new ATCmdParser(_serial);
    _parser->debug_on(1);
    // This will be added at the end of send().
    _parser->set_delimiter("\r\n");
    _parser->set_timeout(5000);

    // Register AT commands.
    // ex. +TEST,3;
    _parser->oob("+TEST", atcmd_server_cb_test);
    // ex. +RUN;
    _parser->oob("+RUN", atcmd_server_cb_run);

    _parser->send("READY: %s:%s\n", __DATE__, __TIME__);
    for (;;)
    {
        // This internally calls getc().
        // In order to flush the internal buffer, press enter.
        _parser->process_oob();
    }

    return -1;
}

int main()
{
#if   PARSER_MODE == SERVER
    return server_func();
#elif PARSER_MODE == CLIENT
    return client_func();
#else
    return 0;
#endif
}

