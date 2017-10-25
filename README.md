# mbed-os-example-atcmdparser #

ATCmdParser example for mbed OS

## Getting started with ATCmdParser ##

This is a example showing ATCmdParser usage with ESP8266 WiFi module.

The program retrieves the FW version of the ESP8266 module connected over UART using AT commands. 

### Required hardware for running this example ###

* [K64F](https://os.mbed.com/platforms/FRDM-K64F/)
* [Seeed-Grove-Shield-V2](https://os.mbed.com/components/Seeed-Grove-Shield-V2/)
* [ESP8266](https://os.mbed.com/components/ESP8266-01/)

#### Connecting the ESP8266 ####
To connect the ESP8266 module to your development board, look at the [ESP8266 Cookbook page](https://developer.mbed.org/users/4180_1/notebook/using-the-esp8266-with-the-mbed-lpc1768/). In general, this means hooking up the ESP8266 TX pin to `D1` and the ESP8266 RX pin to `D0` on your development board.

##  Getting started

1. Import the example

   ```
   mbed import mbed-os-example-atcmdparser
   cd mbed-os-example-atcmdparser
   ```
2. Compile and generate binary

   For example, for `ARMCC`:

   ```
   mbed compile -t ARM -m K64F
   ```
   
 5. Open a serial console session with the target platform using the following parameters:
    * **Baud rate:** 9600
    * **Data bits:** 8
    * **Stop bits:** 1
    * **Parity:** None
 
 6. Copy or drag the application `mbed-os-example-atcmdparser.bin` in the folder `mbed-os-example-atcmdparser/BUILD/<TARGET NAME>/<PLATFORM NAME>` onto the target board.
 
 7. The serial console should display a similar output to below, indicating a successful AT communication:
 ```
ATCmdParser with ESP8266 example
AT> AT+GMR
AT? SDK version:%*d%n
AT< AT+GMR
AT<
AT< AT version:1.3.0.0(Jul 14 2016 18:54:01)
AT= SDK version:2
AT? OK%n
AT< .0.0(656edbf)
AT< compile time:Jul 19 2016 18:44:44
AT= OK
ATCmdParser: Retrieving FW version
ATCmdParser: FW version: 2
ATCmdParser: Retrieving FW version success
...

```

## Documentation ##

More information on the ATCmdParser API can be found in the [mbed handbook]().
