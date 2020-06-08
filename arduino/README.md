## Code Card Arduino
![](images/arduino.png)

The Code Card runs on an [ESP8266](https://en.wikipedia.org/wiki/ESP8266) Wi-fi microcontroller.  

The ESP8266 is a low-cost Wi-Fi microchip with full TCP/IP stack and microcontroller capability, produced by manufacturer Espressif Systems.  
The processor is an L106 32-bit RISC microprocessor core running at 80 MHz, with 4 MiB external QSPI flash. The ESP8266 supports IEEE 802.11 b/g/n Wi-Fi, WEP or WPA/WPA2 authentication, and also supports open networks.  

We have included the source code here so you can modify you Code Card however you want!  

### Software
In order to customise the Code Card firmware, you need to download the [Arduino IDE](https://www.arduino.cc/en/Main/Software) and configure it to use the Arduino core for ESP8266 WiFi chip.  
The Arduino core for ESP8266 is a C++ based firmware. With this core, the ESP8266 CPU and its Wi-Fi components can be programmed like any other Arduino device using the Arduino IDE.

### Input Formats
```
{
"template": "template1",
"title": "",
"subtitle": "",
"body": "",
"icon": "",
"badge": "",
"backgroundColor": "",
"fingerprint": ""
}
```
```
{
"template": "template5|template6",
"title": "",
"subtitle": "",
"body": "",
"backgroundColor": ""
}
```
```
{
"template": "template7",
"title": "",
"subtitle": "",
"backgroundColor": "",
"backgroundImage": "",
"fingerprint": ""
}
```
```
{
"template": "template8",
"backgroundColor": "",
"backgroundImage": "",
"fingerprint": ""
}
```
```
{
"template": "template9",
"title": "",
"subtitle": "",
"backgroundColor": ""
}
```
```
{
"template": "template10",
"title": "",
"subtitle": "",
"body": "",
"backgroundColor": "",
"barcode": ""
}
```
```
{
"template": "template11",
"title": "",
"subtitle": "",
"icon": "",
"badge": "",
"backgroundColor": "",
"fingerprint": ""
}
```
```
{
"template": "custom",
"title": "",
"titleFont": "",
"titleX": "",
"titleY": "",
"subtitle": "",
"subtitleFont": "",
"subtitleX": "",
"subtitleY": "",
"body": "",
"bodyFont": "",
"bodyX": "",
"bodyY": "",
"icon": "",
"badge": "",
"iconX": "",
"iconY": "",
"iconSize": "",
"backgroundColor": "",
"backgroundImage": "",
"fingerprint": ""
}
```
Template configure
```
{
"template": "configure",
"buttona1": "URL",
"methoda1": "GET|POST",
"fingerprinta1": "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX",
"buttona2": "URL",
"methoda2": "GET|POST",
"fingerprinta2": "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX",
"buttonb1": "URL",
"methodb1": "GET|POST",
"fingerprintb1": "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX",
"buttonb2": "URL",
"methodb2": "GET|POST",
"fingerprintb2": "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX",
}
```
Template weather
```
{
  "template": "weather",
  "current": {
    // response of current weather
  },
  "forecast": {
   // response of forecast api
  }
}
```

### Predefined Icon Names
#### Icons Size 64
```
  "oracle", "champion", "duke", "jduchess", "ace", "fail", "mail", "twitter",
  "01d", "02d", "03d", "04d", "09d", "10d", "11d", "13d", "50d",
  "db", "java", "containers", "microservices", "opensource", "chatbots",
  "blockchain", "javascript", "nighthacker"
```
#### Icons Size 128
```
  "oracle", "champion", "jduchess", "db", "java", "containers", "microservices",
  "opensource", "chatbots", "blockchain", "javascript", "nighthacker"
```
#### Backgrounds
```
  "oracle", "codeone"
```

### Code Card Source Code
Download or `git clone https://github.com/cameronsenese/codecard.git` this project, and open the Arduino main file ([codecard.ino](https://github.com/cameronsenese/codecard/blob/master/arduino/codecard/codecard.ino)) to get started.

- /arduino contains the source code for the Code Card
- /arduino/bin contains a precompiled image of the Code Card firmware

## Instructions
The following instruction describes the setup and configuration of the Arduino IDE, and the process to upload firmware to your Code Card using the Arduino IDE:

*Note:*
- *Versions per component are detailled below, if you chose to deviate from the recommended - your mileage may vary!*
- *Ensure your Code Card can connect to Wi-Fi before completing step #12*

1. Install the Arduino IDE version 1.8.8: https://www.arduino.cc/en/Main/Software
2. Install the serial driver for the ESP-12F Wi-Fi chip: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
3. Clone the `codecard` GitHub repository to obtain the source code: https://github.com/cameronsenese/codecard
4. In the Arduino IDE, go to Preferences and set the “Additional Board Managers URLs to http://arduino.esp8266.com/stable/package_esp8266com_index.json
5. Install esp8266 support: Tools | Board | Board Manager
   - esp8266 by ESP8266 Community 2.6.3
6. Install the required modules: Sketch | Include Libraries | Manage Libraries
   - ArduinoJson by Benoit Blanchon version 6.14.1+
   - GxEPD2 by Jean-Marc Zingg version 1.2.6+
   - Adafruit GFX Library by Adafruit 1.7.5+
7. Under Tools set the following settings:
   - Board: “Generic ESP8266 Module”
   - Upload Speed “115200”
   - CPU Frequency: “80 MHz”
   - Crystal Frequency: “26 MHz”
   - Flash Size: “4M (3M SPIFFS)”
   - Flash Mode: “DIO”
   - Flash Frequency: “40MHz”
   - Reset Method: “nodemcu”
   - Debug port: “Disabled”
   - Debug Level: “None”
   - IwIP Variant: “v2 Lower Memory”
   - VTables: “Flash”
   - Builtin Led: “2”
   - Erase Flash: “Only Sketch”
   - Port: This is the port that shows up once you turn on the CodeCard and press a button
   - Programmer: “ArduinoISP”
8.  Open the codecard.ino sketch located in the /arduino directory downloaded in step 3 above
   - File | Open | codecard.ino
9. Connect the CodeCard via USB to your computer
10. Establish serial connection via Arduino IDE:
   - Tools | Serial Monitor
11. Turn on the Code Card
12. Press and hold button `A` until 5 seconds after the following output is displayed:
```
Shuting down...
Shuting down...
```
13. From the Arduino IDE choose Sketch | Upload
    - The Arduino IDE may compile the skectch before uploading to the Code Card.
    - The following output indicates completed firmware upload:
```
Sketch uses 447692 bytes (89%) of program storage space. Maximum is 499696 bytes.
Global variables use 43260 bytes (52%) of dynamic memory, leaving 38660 bytes for local variables. Maximum is 81920 bytes.
Uploading 451840 bytes from ...\arduino_build_548415/codecard.ino.bin to flash at 0x00000000
................................................................................ [ 18% ]
................................................................................ [ 36% ]
................................................................................ [ 54% ]
................................................................................ [ 72% ]
................................................................................ [ 90% ]
..........................................                                       [ 100% ]
```
