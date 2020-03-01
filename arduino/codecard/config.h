/*
  config.h
*/
#undef USING_AXTLS
#include <ESP8266WiFi.h> // ESP8266 Board support needed. Add http://arduino.esp8266.com/stable/package_esp8266com_index.json to Board Manager Urls (Preferences)
#include <ArduinoJson.h> // Search and add using Arduino Library Manager
#include <pgmspace.h>
#include <EEPROM.h>
#include <GxEPD2_BW.h> // Download/Clone and put in Arduino Library https://github.com/ZinggJM/GxEPD2

// Also make sure you have the Adafruit GFX library

#define WAKE_PIN 16
#define BAUD_SPEED 115200

// Button pins
#define BUTTON1_PIN 10 // 10
#define BUTTON2_PIN 12 // 12

int btn1State = LOW;
int btn2State = LOW;
unsigned long startTime;
unsigned long currentTime;

GxEPD2_BW<GxEPD2_270, GxEPD2_270::HEIGHT> display(GxEPD2_270(/*CS=D8*/ 2, /*DC=D3*/ 0, /*RST=D4*/ 4, /*BUSY=D2*/ 5)); // 2.7" b/w 264x176

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/TomThumb.h>

const char* projectName = "Code Card";
const char* projectVersion = "1.0";
const char* projectAuthor = "Oracle A-Team Security";
const char* projectSite = "developer.oracle.com/codecard";

const char* defaultSSID = "CHANGE_TO_SSID";
const char* defaultPassword = "CHANGE_TO_PASSWORD";
const char* defaultUrl = "https://apex.oracle.com/pls/apex/appslab/functions/master";
const char* defaultFingerprint = "2C C2 E9 A1 16 75 DC E5 ED 97 69 49 52 EC 9B A4 81 87 44 2A";

// char arrayToStore[100];
const int maxValue = 200;
const int eepromSize = 4096;

const int keysLen = 15;

// Available keys
const char* const keys[keysLen] PROGMEM = {
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
static const uint16_t input_buffer_pixels = 640; // may affect performance
static const uint16_t max_row_width = 640; // for up to 7.5" display
static const uint16_t max_palette_pixels = 256; // for depth <= 8
uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t output_row_color_buffer[max_row_width / 8]; // buffer for at least one row of color bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t color_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 c/w
