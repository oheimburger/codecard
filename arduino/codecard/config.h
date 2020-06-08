// -*- C++ -*-
/*
  config.h
*/
#undef USING_AXTLS
// ESP8266 Board support needed.
// Add http://arduino.esp8266.com/stable/package_esp8266com_index.json to
// Board Manager Urls (Preferences)
#include <ESP8266WiFi.h>
#if 0
#include <ESP8266HTTPClient.h>
#endif
// Search and add using Arduino Library Manager
#include <ArduinoJson.h>
#include <pgmspace.h>
#include <EEPROM.h>
// Download/Clone and put in Arduino Library https://github.com/ZinggJM/GxEPD2
#include <GxEPD2_BW.h>
#include "time.h"
#include <SPI.h>

// Also make sure you have the Adafruit GFX library

#define WAKE_PIN 16
#define BAUD_SPEED 115200

// Button pins
#define BUTTON1_PIN 10 // 10
#define BUTTON2_PIN 12 // 12

// int btn1State = LOW;
// int btn2State = LOW;
unsigned long startTime;
unsigned long currentTime;

#if 0
static const uint8_t EPD_CS = 2;
static const uint8_t EPD_DC = 0;
static const uint8_t EPD_RST = 4;
static const uint8_t EPD_BUSY = 5;

// 2.7" b/w 264x176
GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
#else
GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(2, 0, 4, 5));
#endif

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/TomThumb.h>

#define FREE_SANS_9 "FreeSans9pt"
#define FREE_SANS_BOLD_12"FreeSansBold12pt"
#define FREE_SANS_BOLD_18 "FreeSansBold18pt"
#define FREE_SANS_BOLD_9 "FreeSansBold9pt"
#define COLOR_BLACK "black"
#define COLOR_WHITE "white"

#define PROJECT_NAME "Code Card"
#define PROJECT_VERSION "1.0.1"
#define PROJECT_AUTHOR "Oracle A-Team Security"
#define PROJECT_SITE "ateam-oracle.com"

#define DEFAULT_SSID "CHANGE_TO_SSID"
#define DEFAULT_PASSWORD "CHANGE_TO_PASSWORD"
#define DEFAULT_URL "https://apex.oracle.com/pls/apex/appslab/functions/master"
#define DEFAULT_FINGERPRINT "2C C2 E9 A1 16 75 DC E5 ED 97 69 49 52 EC 9B A4 81 87 44 2A"

#define MAX_VALUE 200
#define EEPROM_SIZE 4096
#define KEYS_LEN 15

// Available keys
const char* const KEYS[KEYS_LEN] PROGMEM = {
  "ssid",
  "password",
  "buttona1",
  "buttona2",
  "buttonb1",
  "buttonb2",
  "fingerprinta1",
  "fingerprinta2",
  "fingerprintb1",
  "fingerprintb2",
  "methoda1",
  "methoda2",
  "methodb1",
  "methodb2",
  "initalsetup"
};

// Wifi image settings
// may affect performance
static const uint16_t INPUT_BUFFER_PIXELS = 640;
// for up to 7.5" display
static const uint16_t MAX_ROW_WIDTH = 640;
// for depth <= 8
static const uint16_t MAX_PALETTE_PIXELS = 256;
// up to depth 24
uint8_t input_buffer[3 * INPUT_BUFFER_PIXELS];
// buffer for at least one row of b/w bits
uint8_t output_row_mono_buffer[MAX_ROW_WIDTH / 8];
// buffer for at least one row of color bits
uint8_t output_row_color_buffer[MAX_ROW_WIDTH / 8];
// palette buffer for depth <= 8 b/w
uint8_t mono_palette_buffer[MAX_PALETTE_PIXELS / 8];
// palette buffer for depth <= 8 c/w
uint8_t color_palette_buffer[MAX_PALETTE_PIXELS / 8];
