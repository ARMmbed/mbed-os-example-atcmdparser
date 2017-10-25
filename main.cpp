/* ATCmdParser usage example
 * Copyright (c) 2016 ARM Limited
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
#include "platform\ATCmdParser.h"
#include "drivers\UARTSerial.h"

#define   ESP8266_DEFAULT_BAUD_RATE   115200

UARTSerial *_serial;
ATCmdParser *_parser;

int main()
{
    printf("\nATCmdParser with ESP8266 example");
    
    _serial = new UARTSerial(D1, D0, ESP8266_DEFAULT_BAUD_RATE);
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
    
    printf("\nReset ESP8266 WiFi module");
    for (int i = 0; i < 2; i++) {
        if (_parser->send("AT+RST")
            && _parser->recv("OK\r\nready")) {
            printf("\nATCmdParser: Reseting ESP8266 success");
        } else {
            printf("\nATCmdParser: Reseting ESP8266 failure");
        }
    }
    
    printf("\nStarting-up ESP8266");
    bool success = _parser->send("AT+CWMODE_CUR=%d", 1)
        && _parser->recv("OK")
        && _parser->send("AT+CIPMUX=1")
        && _parser->recv("OK");
    if( success ) {
        printf("\nATCmdParser: Starting-up ESP8266 success");
    } else {
        printf("\nATCmdParser: Starting-up ESP8266 failure");
    }      

    printf("\nScan for WiFi networks");
    unsigned cnt = 0;
    unsigned sec = 0;
    unsigned scan_limit = 5;
    nsapi_wifi_ap_t ap;
    bool ret = true;
    
    if (!_parser->send("AT+CWLAP")) {
        printf("\nScan for WiFi networks failed");
    }

    do
    {
        ret = _parser->recv("+CWLAP:(%d,\"%32[^\"]\",%hhd,\"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\",%d", 
                            &sec, 
                            ap.ssid,
                            &ap.rssi, 
                            &ap.bssid[0], 
                            &ap.bssid[1], 
                            &ap.bssid[2], 
                            &ap.bssid[3], 
                            &ap.bssid[4],
                            &ap.bssid[5], 
                            &ap.channel);
        

        printf("\nSSID: %s", ap.ssid );
        cnt++;
        if (cnt >= scan_limit) {
            break;
        }
    } while(ret);
    
    printf("\nDone\n");
}
