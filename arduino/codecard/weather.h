// -*- C++ -*-
// weather.h

#include "weatherlang.h"

#define SCREEN_WIDTH display.width()
#define SCREEN_HEIGHT display.height()
#define MAX_READINGS 6
#define LARGE 7 // For best results use odd numbers
#define SMALL 3 // For best results use odd numbers
#define UNITS_METRIC 1

boolean updateLocalTime();

enum Alignment {LEFT, RIGHT, CENTER};

static const bool LARGE_ICON = true, SMALL_ICON = false;
// Strings to hold time and date
String timeStr, dateStr;
int wifiSignal, currentHour = 0, currentMin = 0, currentSec = 0;

#define HEMISPHERE_NORTH 1
const int HEMISPHERE = HEMISPHERE_NORTH;
const int UNITS = UNITS_METRIC;
// Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv 
const char* TIMEZONE = "GMT0BST,M3.5.0/01,M10.5.0/02";
// See below for examples
const char* NTP_SERVER = "0.uk.pool.ntp.org";
// Or, choose a time server close to you, but in most cases it's best to use pool.ntp.org to find an NTP server
// then the NTP system decides e.g. 0.pool.ntp.org, 1.pool.ntp.org as the NTP syem tries to find  the closest available servers
// EU "0.europe.pool.ntp.org"
// US "0.north-america.pool.ntp.org"
// See: https://www.ntppool.org/en/

const int GMT_OFFSET_SEC = 0;
// UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically
// -18000, AU is typically (+8hrs) 28800
const int DAYLIGHT_OFFSET_SEC = 3600;
// In the UK DST is +1hr or 3600-secs, other countries may use 2hrs 7200 or 30-mins 1800 or 5.5hrs 19800 Ahead of GMT use + offset behind - offset

// Example time zones
// const char* Timezone = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
// const char* Timezone = "CET-1CEST,M3.5.0,M10.5.0/3";       // Central Europe
// const char* Timezone = "EST-2METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
// const char* Timezone = "EST5EDT,M3.2.0,M11.1.0";           // EST USA  
// const char* Timezone = "CST6CDT,M3.2.0,M11.1.0";           // CST USA
// const char* Timezone = "MST7MDT,M4.1.0,M10.5.0";           // MST USA
// const char* Timezone = "NZST-12NZDT,M9.5.0,M4.1.0/3";      // Auckland
// const char* TIMEZONE = "EET-2EEST,M3.5.5/0,M10.5.5/0";     // Asia
// const char* TIMEZONE = "ACST-9:30ACDT,M10.1.0,M4.1.0/3":   // Australia

#if 0
// For current Day and Day 1, 2, 3, etc
typedef struct {
  String city;
  String country;
  String dt;
  String period;
  String icon;
  String trend;
  String main0;
  String forecast0;
  String forecast1;
  String forecast2;
  String description;
  String time;
  float lat;
  float lon;
  float temperature;
  float humidity;
  float high;
  float low;
  float winddir;
  float windspeed;
  float rainfall;
  float snowfall;
  float pressure;
  int cloudcover;
  int visibility;
  int sunrise;
  int sunset;
  int timezone;
} Forecast;

#else

typedef struct {
  String city;
  String country;
  float lat;
  float lon;
  int time;
} Location;

// For current Day and Day 1, 2, 3, etc
typedef struct {
  String period;
  String icon;
  String trend;
  String main0;
  String forecast0;
  String forecast1;
  String forecast2;
  String description;
  String time;
  float temperature;
  float humidity;
  float high;
  float low;
  float winddir;
  float windspeed;
  float rainfall;
  float snowfall;
  float pressure;
  int cloudcover;
  int visibility;
  int sunrise;
  int sunset;
} Forecast;

Location location;
#endif

Forecast weather;
Forecast forecast[MAX_READINGS];

float pressureReadings[MAX_READINGS] = {0};
float temperatureReadings[MAX_READINGS] = {0};
float humidityReadings[MAX_READINGS] = {0};
float rainReadings[MAX_READINGS] = {0};
float snowReadings[MAX_READINGS] = {0};

inline float mmToInches(float value) {
  return 0.0393701 * value;
}

inline float hPaToInHg(float value) {
  return 0.02953 * value;
}

int julianDate(int d, int m, int y) {
  int mm, yy, k1, k2, k3, j;
  yy = y - (int)((12 - m) / 10);
  mm = m + 9;
  if (mm >= 12) {
    mm = mm - 12;
  }
  k1 = (int)(365.25 * (yy + 4712));
  k2 = (int)(30.6001 * mm + 0.5);
  k3 = (int)((int)((yy / 100) + 49) * 0.75) - 38;
  // 'j' for dates in Julian calendar:
  j = k1 + k2 + d + 59 + 1;
  if (j > 2299160) {
    // 'j' is the Julian date at 12h UT (Universal Time) For Gregorian calendar:
    j = j - k3;
  }
  return j;
}

float sumOfPrecip(float dataArray[], int readings) {
  float sum = 0;
  for (int i = 0; i <= readings; i++) {
    sum += dataArray[i];
  }
  return sum;
}

String titleCase(String text) {
  if (text.length() > 0) {
    String tmp = text.substring(0, 1);
    tmp.toUpperCase();
    return tmp + text.substring(1); // Title-case the string
  }
  return text;
}

double normalizedMoonPhase(int d, int m, int y) {
  int j = julianDate(d, m, y);
  // Calculate the approximate phase of the moon
  double phase = (j + 4.867) / 29.53059;
  return (phase - (int)phase);
}

void convertReadingsToImperial() {
  weather.pressure = hPaToInHg(weather.pressure);
  forecast[1].rainfall = mmToInches(forecast[1].rainfall);
  forecast[1].snowfall = mmToInches(forecast[1].snowfall);
}

void drawString(int x, int y, String text, Alignment alignment) {
  // the bounds of x, y and w and h of the variable 'text' in pixels.
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  Serial.println("drawString "+text+ " " + x + " " + y + " " + x1 + " " +y1 + " " + w + " " + h);
  if (alignment == RIGHT) {
    x = x - w;
  }
  if (alignment == CENTER) {
    x = x - w / 2;
  }
  display.setCursor(x, y + h);
  display.print(text);
}

void drawStringMaxWidth(int x, int y, unsigned int text_width, String text, Alignment alignment) {
  // the bounds of x,y and w and h of the variable 'text' in pixels.
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (alignment == RIGHT) {
    x = x - w;
  }
  if (alignment == CENTER) {
    x = x - w / 2;
  }
  // HERE
  display.setCursor(x, y);
  if (text.length() > text_width * 2) {
    display.setFont(&FreeSansBold9pt7b); // Bold 10 pixel
    text_width = 42;
    y = y - 3;
  }
  // HERE
  display.println(text.substring(0, text_width));
  if (text.length() > text_width) {
    // HERE
    display.setCursor(x, y + h + 15);
    String secondLine = text.substring(text_width);
    secondLine.trim(); // Remove any leading spaces
    // HERE
    display.println(secondLine);
  }
}

#if 0

void printData(Forecast& f) {
  Serial.println(" lon: " + String(f.lon));
  Serial.println(" lat: " + String(f.lat));
  Serial.println("main: " + String(f.main0));
  Serial.println("CCov: " + String(f.cloudcover)); // in % of cloud cover
  Serial.println("Ctry: " + String(f.country));
  Serial.println("Desc: " + String(f.description));
  Serial.println("For0: " + String(f.forecast0));
  Serial.println("For1: " + String(f.forecast1));
  Serial.println("For2: " + String(f.forecast2));
  Serial.println("Humi: " + String(f.humidity));
  Serial.println("Icon: " + String(f.icon));
  Serial.println("Peri: " + String(f.period));
  Serial.println("Pres: " + String(f.pressure));
  Serial.println("Rain: " + String(f.rainfall));
  Serial.println("SRis: " + String(f.sunrise));
  Serial.println("SSet: " + String(f.sunset));
  Serial.println("Snow: " + String(f.snowfall));
  Serial.println("THig: " + String(f.high));
  Serial.println("TLow: " + String(f.low));
  Serial.println("TZon: " + String(f.timezone));
  Serial.println("Temp: " + String(f.temperature));
  Serial.println("Visi: " + String(f.visibility)); // in metres
  Serial.println("WDir: " + String(f.winddir));
  Serial.println("WSpd: " + String(f.windspeed));
}

void decodeWeather(JsonVariant doc) {
  if (doc.containsKey("current")) {
    JsonObject weatherDoc = doc["current"];
    weather.lon = weatherDoc["coord"]["lon"].as<float>();
    weather.lat = weatherDoc["coord"]["lat"].as<float>();
    weather.main0 = weatherDoc["weather"][0]["main"].as<char*>();
    weather.forecast0 = weatherDoc["weather"][0]["description"].as<char*>();
    weather.forecast1 = weatherDoc["weather"][1]["description"].as<char*>();
    weather.forecast2 = weatherDoc["weather"][2]["description"].as<char*>();
    weather.icon = weatherDoc["weather"][0]["icon"].as<char*>();
    weather.temperature = weatherDoc["main"]["temp"].as<float>();
    weather.pressure = weatherDoc["main"]["pressure"].as<float>();
    weather.humidity = weatherDoc["main"]["humidity"].as<float>();
    weather.low = weatherDoc["main"]["temp_min"].as<float>();
    weather.high = weatherDoc["main"]["temp_max"].as<float>();
    weather.windspeed = weatherDoc["wind"]["speed"].as<float>();
    weather.winddir = weatherDoc["wind"]["deg"].as<float>();
    // in % of cloud cover
    weather.cloudcover = weatherDoc["clouds"]["all"].as<int>();
    weather.visibility = weatherDoc["visibility"].as<int>();
    weather.rainfall = weatherDoc["rain"]["1h"].as<float>();
    weather.snowfall = weatherDoc["snow"]["1h"].as<float>();
    weather.country = weatherDoc["sys"]["country"].as<char*>();
    weather.sunrise = weatherDoc["sys"]["sunrise"].as<int>();
    weather.sunset = weatherDoc["sys"]["sunset"].as<int>();
    weather.timezone = weatherDoc["timezone"].as<int>();
    weather.city = weatherDoc["name"].as<char*>();
    printData(weather);
  }
  if (doc.containsKey("forecast")) {
    JsonArray list = doc["forecast"]["list"];
    for (byte r = 0; r < MAX_READINGS; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      forecast[r].dt = list[r]["dt"].as<char*>();
      forecast[r].temperature = list[r]["main"]["temp"].as<float>();
      forecast[r].low = list[r]["main"]["temp_min"].as<float>();
      forecast[r].high = list[r]["main"]["temp_max"].as<float>();
      forecast[r].pressure = list[r]["main"]["pressure"].as<float>();
      forecast[r].humidity = list[r]["main"]["humidity"].as<float>();
      forecast[r].forecast0 = list[r]["weather"][0]["main"].as<char*>();
      forecast[r].forecast1 = list[r]["weather"][1]["main"].as<char*>();
      forecast[r].forecast2 = list[r]["weather"][2]["main"].as<char*>();
      forecast[r].icon = list[r]["weather"][0]["icon"].as<char*>();
      forecast[r].description = list[r]["weather"][0]["description"].as<char*>();
      forecast[r].cloudcover = list[r]["clouds"]["all"].as<int>();
      forecast[r].windspeed = list[r]["wind"]["speed"].as<float>();
      forecast[r].winddir = list[r]["wind"]["deg"].as<float>();
      forecast[r].rainfall = list[r]["rain"]["3h"].as<float>();
      forecast[r].snowfall = list[r]["snow"]["3h"].as<float>();
      forecast[r].period = list[r]["dt_txt"].as<char*>();
      printData(forecast[r]);
    }
    // Measure pressure slope between ~now and later
    float pressureTrend = forecast[0].pressure - forecast[2].pressure;
    // Remove any small variations less than 0.1
    pressureTrend = ((int)(pressureTrend * 10)) / 10.0;
    weather.trend = "0";
    if (pressureTrend > 0) {
      weather.trend = "+";
    }
    if (pressureTrend < 0) {
      weather.trend = "-";
    }
    if (pressureTrend == 0) {
      weather.trend = "0";
    }
    if (UNITS != UNITS_METRIC) {
      convertReadingsToImperial();
    }
  }
}

#else

void printLocationData(Location& f) {
  Serial.println(" lon: " + String(f.lon));
  Serial.println(" lat: " + String(f.lat));
  Serial.println("City: " + String(f.city));
  Serial.println("Ctry: " + String(f.country));
  Serial.println("Time: " + String(f.time));
}

void printData(Forecast& f) {
  Serial.println("main: " + String(f.main0));
  Serial.println("CCov: " + String(f.cloudcover)); // in % of cloud cover
  Serial.println("Desc: " + String(f.description));
  Serial.println("For0: " + String(f.forecast0));
  Serial.println("For1: " + String(f.forecast1));
  Serial.println("For2: " + String(f.forecast2));
  Serial.println("Humi: " + String(f.humidity));
  Serial.println("Icon: " + String(f.icon));
  Serial.println("Peri: " + String(f.period));
  Serial.println("Pres: " + String(f.pressure));
  Serial.println("Rain: " + String(f.rainfall));
  Serial.println("SRis: " + String(f.sunrise));
  Serial.println("SSet: " + String(f.sunset));
  Serial.println("Snow: " + String(f.snowfall));
  Serial.println("THig: " + String(f.high));
  Serial.println("TLow: " + String(f.low));
  Serial.println("Temp: " + String(f.temperature));
  Serial.println("Visi: " + String(f.visibility)); // in metres
  Serial.println("WDir: " + String(f.winddir));
  Serial.println("WSpd: " + String(f.windspeed));
}

void decodeWeather(JsonVariant doc) {
  if (doc.containsKey("location")) {
    JsonObject locDoc = doc["location"];
    location.lon = locDoc["lon"].as<float>();
    location.lat = locDoc["lat"].as<float>();
    location.country = locDoc["country"].as<char*>();
    location.time = locDoc["time"].as<int>();
    location.city = locDoc["city"].as<char*>();
  }
  if (doc.containsKey("current")) {
    JsonObject weatherDoc = doc["current"];
    weather.main0 = weatherDoc["main"].as<char*>();
    weather.forecast0 = weatherDoc["forecast0"].as<char*>();
    weather.forecast1 = weatherDoc["forecast1"].as<char*>();
    weather.forecast2 = weatherDoc["forecast2"].as<char*>();
    weather.icon = weatherDoc["icon"].as<char*>();
    weather.temperature = weatherDoc["temperature"]["current"].as<float>();
    weather.low = weatherDoc["temperature"]["low"].as<float>();
    weather.high = weatherDoc["temperature"]["high"].as<float>();
    weather.pressure = weatherDoc["pressure"].as<float>();
    weather.humidity = weatherDoc["humidity"].as<float>();
    weather.windspeed = weatherDoc["wind"]["speed"].as<float>();
    weather.winddir = weatherDoc["wind"]["deg"].as<float>();
    // in % of cloud cover
    weather.cloudcover = weatherDoc["clouds"].as<int>();
    weather.visibility = weatherDoc["visibility"].as<int>();
    weather.rainfall = weatherDoc["rain"].as<float>();
    weather.snowfall = weatherDoc["snow"].as<float>();
    weather.sunrise = weatherDoc["sunrise"].as<int>();
    weather.sunset = weatherDoc["sunset"].as<int>();
    printData(weather);
  }
  if (doc.containsKey("forecast")) {
    JsonArray list = doc["forecast"]["list"];
    for (byte r = 0; r < MAX_READINGS; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      forecast[r].temperature = list[r]["main"]["temp"].as<float>();
      forecast[r].low = list[r]["main"]["temp_min"].as<float>();
      forecast[r].high = list[r]["main"]["temp_max"].as<float>();
      forecast[r].pressure = list[r]["main"]["pressure"].as<float>();
      forecast[r].humidity = list[r]["main"]["humidity"].as<float>();
      forecast[r].forecast0 = list[r]["weather"][0]["main"].as<char*>();
      forecast[r].forecast1 = list[r]["weather"][1]["main"].as<char*>();
      forecast[r].forecast2 = list[r]["weather"][2]["main"].as<char*>();
      forecast[r].icon = list[r]["weather"][0]["icon"].as<char*>();
      forecast[r].description = list[r]["weather"][0]["description"].as<char*>();
      forecast[r].cloudcover = list[r]["clouds"]["all"].as<int>();
      forecast[r].windspeed = list[r]["wind"]["speed"].as<float>();
      forecast[r].winddir = list[r]["wind"]["deg"].as<float>();
      forecast[r].rainfall = list[r]["rain"]["3h"].as<float>();
      forecast[r].snowfall = list[r]["snow"]["3h"].as<float>();
      forecast[r].period = list[r]["dt_txt"].as<char*>();
      printData(forecast[r]);
    }
    // Measure pressure slope between ~now and later
    float pressureTrend = forecast[0].pressure - forecast[2].pressure;
    // Remove any small variations less than 0.1
    pressureTrend = ((int)(pressureTrend * 10)) / 10.0;
    weather.trend = "0";
    if (pressureTrend > 0) {
      weather.trend = "+";
    }
    if (pressureTrend < 0) {
      weather.trend = "-";
    }
    if (pressureTrend == 0) {
      weather.trend = "0";
    }
    if (UNITS != UNITS_METRIC) {
      convertReadingsToImperial();
    }
  }
}

#endif

String convertUnixTime(int unixTime) {
  Serial.println("convertUnixTime");
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  time_t tm = unixTime;
  struct tm *nowTm = localtime(&tm);
  char output[40];
  char* outputFormat =
    (UNITS == UNITS_METRIC) ? "%H:%M %d.%m.%y" : "%I:%M%P %m/%d/%y";
  strftime(output, sizeof(output), outputFormat, nowTm);
  return output;
}

void arrow(int x, int y, int aSize, float aAngle, int pWidth, int pLength) {
  Serial.println("arrow");
  // x,y is the centre poistion of the arrow and aSize is the radius out from
  // the x,y position
  // aAngle is angle to draw the pointer at e.g. at 45° for NW
  // pWidth is the pointer width in pixels
  // pLength is the pointer length in pixels
  // calculate X position
  float dx = (aSize - 10) * cos((aAngle - 90) * PI / 180) + x;
  // calculate Y position
  float dy = (aSize - 10) * sin((aAngle - 90) * PI / 180) + y;
  float x1 = 0;
  float y1 = pLength;
  float x2 = pWidth / 2;
  float y2 = pWidth / 2;
  float x3 = -pWidth / 2;
  float y3 = pWidth / 2;
  float angle = aAngle * PI / 180 - 135;
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
  display.fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, GxEPD_BLACK);
}

String moonPhase(int d, int m, int y, int hemisphere) {
  Serial.println("moonPhase");
  if (m < 3) {
    y--;
    m += 12;
  }
  ++m;
  int c = 365.25 * y;
  int e = 30.6  * m;
  /* jd is total days elapsed */
  double jd = c + e + d - 694039.09;
  /* divide by the moon cycle (29.53 days) */
  jd /= 29.53059;
  /* int(jd) -> b, take integer part of jd */
  int b = jd;
  /* subtract integer part to leave fractional part of original jd */
  jd -= b;
  /* scale fraction from 0-8 and round by adding 0.5 */
  b = jd * 8 + 0.5;
  /* 0 and 8 are the same phase so modulo 8 for 0 */
  b = b & 7;
  if (hemisphere != HEMISPHERE_NORTH) {
    b = 7 - b;
  }
  switch (b) {
  case 0:
    return TXT_MOON_NEW;
  case 1:
    return TXT_MOON_WAXING_CRESCENT;
  case 2:
    return TXT_MOON_FIRST_QUARTER;
  case 3:
    return TXT_MOON_WAXING_GIBBOUS;
  case 4:
    return TXT_MOON_FULL;
  case 5:
    return TXT_MOON_WANING_GIBBOUS;
  case 6:
    return TXT_MOON_THIRD_QUARTER;
  case 7:
    return TXT_MOON_WANING_CRESCENT;
  }
  return "";
}

String windDegToDirection(float windDirection) {
  Serial.println("windDegToDirection");
  if (windDirection >= 348.75 || windDirection < 11.25) {
    return TXT_N;
  }
  if (windDirection >= 11.25 && windDirection < 33.75) {
    return TXT_NNE;
  }
  if (windDirection >= 33.75 && windDirection < 56.25) {
    return TXT_NE;
  }
  if (windDirection >= 56.25 && windDirection < 78.75) {
    return TXT_ENE;
  }
  if (windDirection >= 78.75 && windDirection < 101.25) {
    return TXT_E;
  }
  if (windDirection >= 101.25 && windDirection < 123.75) {
    return TXT_ESE;
  }
  if (windDirection >= 123.75 && windDirection < 146.25) {
    return TXT_SE;
  }
  if (windDirection >= 146.25 && windDirection < 168.75) {
    return TXT_SSE;
  }
  if (windDirection >= 168.75 && windDirection < 191.25) {
    return TXT_S;
  }
  if (windDirection >= 191.25 && windDirection < 213.75) {
    return TXT_SSW;
  }
  if (windDirection >= 213.75 && windDirection < 236.25) {
    return TXT_SW;
  }
  if (windDirection >= 236.25 && windDirection < 258.75) {
    return TXT_WSW;
  }
  if (windDirection >= 258.75 && windDirection < 281.25) {
    return TXT_W;
  }
  if (windDirection >= 281.25 && windDirection < 303.75) {
    return TXT_WNW;
  }
  if (windDirection >= 303.75 && windDirection < 326.25) {
    return TXT_NW;
  }
  if (windDirection >= 326.25 && windDirection < 348.75) {
    return TXT_NNW;
  }
  return "?";
}

void drawMoon(int x, int y, int dd, int mm, int yy, int hemisphere) {
  Serial.println("drawMoon");
  const int DIAMETER = 38;
  const int NUMBER_OF_LINES = 90;
  double phase = normalizedMoonPhase(dd, mm, yy);
  if (hemisphere != HEMISPHERE_NORTH) {
    phase = 1 - phase;
  }
  // Draw dark part of moon
  display.fillCircle(x + DIAMETER - 1, y + DIAMETER, DIAMETER / 2 + 1, GxEPD_BLACK);
  for (double yPos = 0; yPos <= NUMBER_OF_LINES / 2; yPos++) {
    double xPos = sqrt(NUMBER_OF_LINES / 2 * NUMBER_OF_LINES / 2 - yPos * yPos);
    // Determine the edges of the lighted part of the moon
    double rPos = 2 * xPos;
    double xPos1, xPos2;
    if (phase < 0.5) {
      xPos1 = -xPos;
      xPos2 = rPos - 2 * phase * rPos - xPos;
    } else {
      xPos1 = xPos;
      xPos2 = xPos - 2 * phase * rPos + rPos;
    }
    // Draw light part of moon
    double pW1x = (xPos1 + NUMBER_OF_LINES) / NUMBER_OF_LINES * DIAMETER + x;
    double pW1y = (NUMBER_OF_LINES - yPos)  / NUMBER_OF_LINES * DIAMETER + y;
    double pW2x = (xPos2 + NUMBER_OF_LINES) / NUMBER_OF_LINES * DIAMETER + x;
    double pW2y = (NUMBER_OF_LINES - yPos)  / NUMBER_OF_LINES * DIAMETER + y;
    double pW3x = (xPos1 + NUMBER_OF_LINES) / NUMBER_OF_LINES * DIAMETER + x;
    double pW3y = (yPos + NUMBER_OF_LINES)  / NUMBER_OF_LINES * DIAMETER + y;
    double pW4x = (xPos2 + NUMBER_OF_LINES) / NUMBER_OF_LINES * DIAMETER + x;
    double pW4y = (yPos + NUMBER_OF_LINES)  / NUMBER_OF_LINES * DIAMETER + y;
    display.drawLine(pW1x, pW1y, pW2x, pW2y, GxEPD_WHITE);
    display.drawLine(pW3x, pW3y, pW4x, pW4y, GxEPD_WHITE);
  }
  display.drawCircle(x + DIAMETER - 1, y + DIAMETER, DIAMETER / 2, GxEPD_BLACK);
}

void drawWind(int x, int y, float angle, float windspeed) {
  Serial.println("drawWind");
#define CRADIUS 15
  // calculate X position
  float dx = CRADIUS * cos((angle - 90) * PI / 180) + x;
  // calculate Y position
  float dy = CRADIUS * sin((angle - 90) * PI / 180) + y;
  // Show wind direction on outer circle
  arrow(x, y, CRADIUS - 3, angle, 10, 12);
  display.drawCircle(x, y, CRADIUS + 2, GxEPD_BLACK);
  display.drawCircle(x, y, CRADIUS + 3, GxEPD_BLACK);
  for (int m = 0; m < 360; m = m + 45) {
    // calculate X position
    dx = CRADIUS * cos(m * PI / 180);
    // calculate Y position
    dy = CRADIUS * sin(m * PI / 180);
    display.drawLine(x + dx, y + dy, x + dx * 0.8, y + dy * 0.8, GxEPD_BLACK);
  }
  display.setFont(&FreeSansBold9pt7b); // Bold 10 pixel
  drawString(x - 2, y + CRADIUS + 10, windDegToDirection(angle), CENTER);
  display.setFont(&FreeSansBold9pt7b); // Bold 8 pixel
  drawString(x, y - CRADIUS - 14, String(windspeed, 1) + (UNITS == UNITS_METRIC ? " m/s" : " mph"), CENTER);
}

void drawPressureTrend(int x, int y, float pressure, String slope) {
  Serial.println("drawPressureTrend");
  drawString(x, y, String(pressure, (UNITS == UNITS_METRIC ? 0 : 1)) + (UNITS == UNITS_METRIC ? "hPa" : "in"), LEFT);
  x = x + 48 - (UNITS == UNITS_METRIC? 0: 15); y = y + 3;
  if (slope == "+") {
    display.drawLine(x, y, x + 4, y - 4, GxEPD_BLACK);
    display.drawLine(x + 4, y - 4, x + 8, y, GxEPD_BLACK);
  } else if (slope == "0") {
    display.drawLine(x + 3, y - 4, x + 8, y, GxEPD_BLACK);
    display.drawLine(x + 3, y + 4, x + 8, y, GxEPD_BLACK);
  } else if (slope == "-") {
    display.drawLine(x,  y, x + 4, y + 4, GxEPD_BLACK);
    display.drawLine(x + 4, y + 4, x + 8, y, GxEPD_BLACK);
  }
}

bool getLocalTime(struct tm * info, uint32_t ms) {
  Serial.println("getLocalTime");
  uint32_t count = ms / 10;
  time_t now;
  time(&now);
  localtime_r(&now, info);
  if (info->tm_year > (2016 - 1900)) {
    return true;
  }
  while (count--) {
    delay(10);
    time(&now);
    localtime_r(&now, info);
    if (info->tm_year > (2016 - 1900)) {
      return true;
    }
  }
  return false;
}

boolean updateLocalTime() {
  Serial.println("updateLocalTime");
  struct tm timeinfo;
  char timeOutput[30], dayOutput[30], updateTime[30];
  // Wait for 5-sec for time to synchronise
  while (!getLocalTime(&timeinfo, 5000)) {
    Serial.println("Failed to obtain time");
    return false;
  }
  currentHour = timeinfo.tm_hour;
  currentMin = timeinfo.tm_min;
  currentSec = timeinfo.tm_sec;
  if (UNITS == UNITS_METRIC) {
    if ((LANGUAGE == "CZ") || (LANGUAGE == "DE") || (LANGUAGE == "NL") || (LANGUAGE == "PL")) {
      sprintf(dayOutput, "%s, %02u. %s %04u", WEEKDAY[timeinfo.tm_wday], timeinfo.tm_mday, MONTH[timeinfo.tm_mon], (timeinfo.tm_year) + 1900); // dayOutput >> So., 23. Juni 2019 <<
    } else {
      sprintf(dayOutput, "%s  %02u-%s-%04u", WEEKDAY[timeinfo.tm_wday], timeinfo.tm_mday, MONTH[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    }
    strftime(updateTime, sizeof(updateTime), "%H:%M:%S", &timeinfo);  // Creates: '@ 14:05:49'   and change from 30 to 8 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sprintf(timeOutput, "%s", updateTime);
  } else {
    // Creates  'Sat May-31-2019'
    strftime(dayOutput, sizeof(dayOutput), "%a %b-%d-%Y", &timeinfo);
    // Creates: '@ 02:05:49pm'
    strftime(updateTime, sizeof(updateTime), "%r", &timeinfo);
    sprintf(timeOutput, "%s", updateTime);
  }
  dateStr = dayOutput;
  timeStr = timeOutput;
  return true;
}

boolean setupTime() {
  Serial.println("setupTime");
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER, "time.nist.gov");
  setenv("TZ", TIMEZONE, 1);
  tzset();
  delay(100);
  return updateLocalTime();
}

void drawBattery(int x, int y) {
  Serial.println("drawBattery");
  uint8_t percentage = 100;
  float voltage = analogRead(35) / 4096.0 * 7.46;
  if (voltage > 1 ) {
    // Only display if there is a valid reading
    Serial.println("Voltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20) {
      percentage = 100;
    }
    if (voltage <= 3.50) {
      percentage = 0;
    }
    display.drawRect(x + 15, y - 12, 19, 10, GxEPD_BLACK);
    display.fillRect(x + 34, y - 10, 2, 5, GxEPD_BLACK);
    display.fillRect(x + 17, y - 10, 15 * percentage / 100.0, 6, GxEPD_BLACK);
    drawString(x + 60, y - 11, String(percentage) + "%", RIGHT);
    //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
  }
}

// Symbols are drawn on a relative 10x10grid and 1 scale unit = 1 drawing unit
void addCloud(int x, int y, int scale, int linesize) {
  Serial.println("addCloud");
  // Draw cloud outer
  // Left most circle
  display.fillCircle(x - scale * 3, y, scale, GxEPD_BLACK);
  // Right most circle
  display.fillCircle(x + scale * 3, y, scale, GxEPD_BLACK);
  // Left middle upper circle
  display.fillCircle(x - scale, y - scale, scale * 1.4, GxEPD_BLACK);
  // Right middle upper circle
  display.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75, GxEPD_BLACK);
  // Upper and lower lines
  display.fillRect(x - scale * 3 - 1, y - scale, scale * 6, scale * 2 + 1, GxEPD_BLACK);
  // Clear cloud inner
  // Clear left most circle
  display.fillCircle(x - scale * 3, y, scale - linesize, GxEPD_WHITE);
  // Clear right most circle
  display.fillCircle(x + scale * 3, y, scale - linesize, GxEPD_WHITE);
  // Left middle upper circle
  display.fillCircle(x - scale, y - scale, scale * 1.4 - linesize, GxEPD_WHITE);
  // Right middle upper circle
  display.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75 - linesize, GxEPD_WHITE);
  // Upper and lower lines
  display.fillRect(x - scale * 3 + 2, y - scale + linesize - 1, scale * 5.9, scale * 2 - linesize * 2 + 2, GxEPD_WHITE);
}

void addRaindrop(int x, int y, int scale) {
  Serial.println("addRaindrop");
  display.fillCircle(x, y, scale / 2, GxEPD_BLACK);
  display.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
  x = x + scale * 1.6; y = y + scale / 3;
  display.fillCircle(x, y, scale / 2, GxEPD_BLACK);
  display.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
}

void addRain(int x, int y, int scale, bool iconSize) {
  Serial.println("addRain");
  if (iconSize == SMALL_ICON) {
    scale *= 1.34;
  }
  for (int d = 0; d < 4; d++) {
    addRaindrop(x + scale * (7.8 - d * 1.95) - scale * 5.2, y + scale * 2.1 - scale / 6, scale / 1.6);
  }
}

void addSnow(int x, int y, int scale, bool iconSize) {
  Serial.println("addSnow");
  int dxo, dyo, dxi, dyi;
  for (int flakes = 0; flakes < 5; flakes++) {
    for (int i = 0; i < 360; i = i + 45) {
      dxo = 0.5 * scale * cos((i - 90) * 3.14 / 180); dxi = dxo * 0.1;
      dyo = 0.5 * scale * sin((i - 90) * 3.14 / 180); dyi = dyo * 0.1;
      display.drawLine(dxo + x + flakes * 1.5 * scale - scale * 3, dyo + y + scale * 2, dxi + x + 0 + flakes * 1.5 * scale - scale * 3, dyi + y + scale * 2, GxEPD_BLACK);
    }
  }
}

void addTstorm(int x, int y, int scale) {
  Serial.println("addTstorm");
  y = y + scale / 2;
  for (int i = 0; i < 5; i++) {
    display.drawLine(x - scale * 4 + scale * i * 1.5 + 0, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 0, y + scale, GxEPD_BLACK);
    if (scale != SMALL) {
      display.drawLine(x - scale * 4 + scale * i * 1.5 + 1, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 1, y + scale, GxEPD_BLACK);
      display.drawLine(x - scale * 4 + scale * i * 1.5 + 2, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 2, y + scale, GxEPD_BLACK);
    }
    display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 0, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 0, GxEPD_BLACK);
    if (scale != SMALL) {
      display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 1, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 1, GxEPD_BLACK);
      display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 2, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 2, GxEPD_BLACK);
    }
    display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 0, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5, GxEPD_BLACK);
    if (scale != SMALL) {
      display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 1, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 1, y + scale * 1.5, GxEPD_BLACK);
      display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 2, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 2, y + scale * 1.5, GxEPD_BLACK);
    }
  }
}

void addSun(int x, int y, int scale, bool iconSize) {
  Serial.println("addSun");
  int linesize = 3;
  if (iconSize == SMALL_ICON) {
    linesize = 1;
  }
  display.fillRect(x - scale * 2, y, scale * 4, linesize, GxEPD_BLACK);
  display.fillRect(x, y - scale * 2, linesize, scale * 4, GxEPD_BLACK);
  display.drawLine(x - scale * 1.3, y - scale * 1.3, x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
  display.drawLine(x - scale * 1.3, y + scale * 1.3, x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  if (iconSize == LARGE_ICON) {
    display.drawLine(1 + x - scale * 1.3, y - scale * 1.3, 1 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(2 + x - scale * 1.3, y - scale * 1.3, 2 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(3 + x - scale * 1.3, y - scale * 1.3, 3 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(1 + x - scale * 1.3, y + scale * 1.3, 1 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    display.drawLine(2 + x - scale * 1.3, y + scale * 1.3, 2 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    display.drawLine(3 + x - scale * 1.3, y + scale * 1.3, 3 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  }
  display.fillCircle(x, y, scale * 1.3, GxEPD_WHITE);
  display.fillCircle(x, y, scale, GxEPD_BLACK);
  display.fillCircle(x, y, scale - linesize, GxEPD_WHITE);
}

void addFog(int x, int y, int scale, int linesize, bool iconSize) {
  Serial.println("addFog");
  if (iconSize == SMALL_ICON) {
    linesize = 1;
    y -= 1;
  } else {
    y -= 3;
  }
  for (int i = 0; i < 6; i++) {
    display.fillRect(x - scale * 3, y + scale * 1.5, scale * 6, linesize, GxEPD_BLACK);
    display.fillRect(x - scale * 3, y + scale * 2.0, scale * 6, linesize, GxEPD_BLACK);
    display.fillRect(x - scale * 3, y + scale * 2.6, scale * 6, linesize, GxEPD_BLACK);
  }
}

void addMoon(int x, int y, int scale, bool iconSize) {
  Serial.println("addMoon");
  if (iconSize == LARGE_ICON) {
    x -= 5;
    y += 5;
    display.fillCircle(x - 21, y - 23, scale, GxEPD_BLACK);
    display.fillCircle(x - 14, y - 23, scale * 1.7, GxEPD_WHITE);
  } else {
    display.fillCircle(x - 16, y - 11, scale, GxEPD_BLACK);
    display.fillCircle(x - 11, y - 11, scale * 1.7, GxEPD_WHITE);
  }
}

void sunny(int x, int y, bool iconSize, String iconName) {
  Serial.println("sunny");
  int scale = SMALL;
  if (iconSize == LARGE_ICON) {
    scale = LARGE;
    y -= 4; // Shift up large sun
  } else {
    y += 2; // Shift down small sun icon
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y + 3, scale, iconSize);
  }
  scale *= 1.6;
  addSun(x, y, scale, iconSize);
}

void mostlySunny(int x, int y, bool iconSize, String iconName) {
  Serial.println("mostlySunny");
  int scale = SMALL, linesize = 3, offset = 5;
  if (iconSize == LARGE_ICON) {
    scale = LARGE;
    offset = 10;
  }
  if (scale == SMALL) {
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y + offset + (iconSize ? -8 : 0), scale, iconSize);
  }
  addCloud(x, y + offset, scale, linesize);
  addSun(x - scale * 1.8, y - scale * 1.8 + offset, scale, iconSize);
}

void mostlyCloudy(int x, int y, bool iconSize, String iconName) {
  Serial.println("mostlyCloudy");
  int scale = SMALL, linesize = 3;
  if (iconSize == LARGE_ICON) {
    scale = LARGE;
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y, scale, iconSize);
  }
  addCloud(x, y, scale, linesize);
  addSun(x - scale * 1.8, y - scale * 1.8, scale, iconSize);
  addCloud(x, y, scale, linesize);
}

void cloudy(int x, int y, bool iconSize, String iconName) {
  Serial.println("cloudy");
  int scale = LARGE, linesize = 3;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    if (iconName.endsWith("n")) {
      addMoon(x, y, scale, iconSize);
    }
    linesize = 1;
    addCloud(x, y, scale, linesize);
  } else {
    y += 12;
    if (iconName.endsWith("n")) {
      addMoon(x - 5, y - 15, scale, iconSize);
    }
    addCloud(x + 15, y - 25, 5, linesize); // Cloud top right
    addCloud(x - 15, y - 10, 7, linesize); // Cloud top left
    addCloud(x, y, scale, linesize);       // Main cloud
  }
}

void rain(int x, int y, bool iconSize, String iconName) {
  Serial.println("rain");
  int scale = LARGE, linesize = 3;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y, scale, iconSize);
  }
  addCloud(x, y, scale, linesize);
  addRain(x, y, scale, iconSize);
}

void expectRain(int x, int y, bool iconSize, String iconName) {
  Serial.println("expectRain");
  int scale = LARGE, linesize = 3;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y, scale, iconSize);
  }
  addCloud(x, y, scale, linesize);
  addRain(x, y, scale, iconSize);
}

void chanceRain(int x, int y, bool iconSize, String iconName) {
  Serial.println("chanceRain");
  int scale = LARGE, linesize = 3;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x - (iconSize ? 8 : 0), y, scale, iconSize);
  }
  addSun(x - scale * 1.8, y - scale * 1.8, scale, iconSize);
  addCloud(x, y, scale, linesize);
  addRain(x, y, scale, iconSize);
}

void tstorms(int x, int y, bool iconSize, String iconName) {
  Serial.println("tstorms");
  int scale = LARGE, linesize = 3;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y, scale, iconSize);
  }
  addCloud(x, y, scale, linesize);
  addTstorm(x, y, scale);
}

void snow(int x, int y, bool iconSize, String iconName) {
  Serial.println("snow");
  int scale = LARGE, linesize = 3;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y, scale, iconSize);
  }
  addCloud(x, y, scale, linesize);
  addSnow(x, y, scale, iconSize);
}

void fog(int x, int y, bool iconSize, String iconName) {
  Serial.println("fog");
  int linesize = 3, scale = LARGE;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    linesize = 1;
    y += 5;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y, scale, iconSize);
  }
  addCloud(x, y - 5, scale, linesize);
  addFog(x, y - 2, scale, linesize, iconSize);
}

void haze(int x, int y, bool iconSize, String iconName) {
  Serial.println("haze");
  int linesize = 3, scale = LARGE;
  if (iconSize == SMALL_ICON) {
    scale = SMALL;
    linesize = 1;
  }
  if (iconName.endsWith("n")) {
    addMoon(x, y, scale, iconSize);
  }
  addSun(x, y - 2, scale * 1.4, iconSize);
  addFog(x, y + 3 - (iconSize ? 12 : 0), scale * 1.4, linesize, iconSize);
}

void cloudCover(int x, int y, int cCover) {
  Serial.println("cloudCover");
  addCloud(x - 9, y - 3, SMALL * 0.6, 2); // Cloud top left
  addCloud(x + 3, y - 3, SMALL * 0.6, 2); // Cloud top right
  addCloud(x, y, SMALL * 0.6, 2); // Main cloud
  display.setFont(&FreeSansBold9pt7b); // Bold 8 pixel
  drawString(x + 15, y - 5, String(cCover) + "%", LEFT);
}

void visibility(int x, int y, String visi) {
  Serial.println("visibility");
  y -= 3; //
  float startAngle = 0.52, endAngle = 2.61;
  int r = 10;
  for (float i = startAngle; i < endAngle; i = i + 0.05) {
    display.drawPixel(x + r * cos(i), y - r / 2 + r * sin(i), GxEPD_BLACK);
    display.drawPixel(x + r * cos(i), 1 + y - r / 2 + r * sin(i), GxEPD_BLACK);
  }
  startAngle = 3.61; endAngle = 5.78;
  for (float i = startAngle; i < endAngle; i = i + 0.05) {
    display.drawPixel(x + r * cos(i), y + r / 2 + r * sin(i), GxEPD_BLACK);
    display.drawPixel(x + r * cos(i), 1 + y + r / 2 + r * sin(i), GxEPD_BLACK);
  }
  display.fillCircle(x, y, r / 4, GxEPD_BLACK);
  display.setFont(&FreeSansBold9pt7b); // Bold 8 pixel
  drawString(x + 12, y - 3, visi, LEFT);
}

void nodata(int x, int y, bool iconSize, String iconName) {
  Serial.println("nodata");
  if (iconSize == LARGE_ICON) {
    display.setFont(&FreeSansBold24pt7b); // Bold 24 pixel
  } else {
    display.setFont(&FreeSansBold9pt7b); // Bold 10 pixel
  }
  drawString(x - 3, y - 8, "?", CENTER);
  display.setFont(&FreeSansBold9pt7b); // Bold 8 pixel
}

void displayIcon(int x, int y, String iconName, bool iconSize) {
  Serial.println("displayIcon: " + iconName);
  if (iconName == "01d" || iconName == "01n") {
    sunny(x, y, iconSize, iconName);
  } else if (iconName == "02d" || iconName == "02n") {
    mostlySunny(x, y, iconSize, iconName);
  } else if (iconName == "03d" || iconName == "03n") {
    cloudy(x, y, iconSize, iconName);
  } else if (iconName == "04d" || iconName == "04n") {
    mostlySunny(x, y, iconSize, iconName);
  } else if (iconName == "09d" || iconName == "09n") {
    chanceRain(x, y, iconSize, iconName);
  } else if (iconName == "10d" || iconName == "10n") {
    rain(x, y, iconSize, iconName);
  } else if (iconName == "11d" || iconName == "11n") {
    tstorms(x, y, iconSize, iconName);
  } else if (iconName == "13d" || iconName == "13n") {
    snow(x, y, iconSize, iconName);
  } else if (iconName == "50d") {
    haze(x, y, iconSize, iconName);
  } else if (iconName == "50n") {
    fog(x, y, iconSize, iconName);
  } else {
    nodata(x, y, iconSize, iconName);
  }
}

#if 0

void drawHeadingSection() {
  display.setFont(&FreeSansBold9pt7b); // Bold 8 Pixel
  drawString(27, 15, weather.city, CENTER);
  drawString(2, 1, timeStr, LEFT);
  drawString(SCREEN_WIDTH, 1, dateStr, RIGHT);
  display.drawLine(0, 11, SCREEN_WIDTH, 11, GxEPD_BLACK);
}

#else

void drawHeadingSection() {
  Serial.println("drawHeadingSection");
  display.setFont(&FreeSansBold9pt7b); // Bold 8 Pixel
  drawString(27, 15, location.city, CENTER);
  drawString(2, 1, convertUnixTime(location.time), LEFT);
  drawString(SCREEN_WIDTH, 1, "XXX", RIGHT);
  display.drawLine(0, 11, SCREEN_WIDTH, 11, GxEPD_BLACK);
}

#endif

void drawMainWeatherSection() {
  Serial.println("drawMainWeatherSection");
  displayIcon(205, 45, weather.icon, LARGE_ICON);
  display.setFont(&FreeSansBold12pt7b); // Bold 14 Pixel
  drawString(3, 35, String(weather.temperature, 1) + "° / " + String(weather.humidity, 0) + "%", LEFT);
  display.setFont(&FreeSansBold9pt7b); // Bold 10 Pixel
  drawWind(275, 42, weather.winddir, weather.windspeed);
  if (weather.rainfall > 0.005 || weather.snowfall > 0.005) {
    String unit = (UNITS == UNITS_METRIC ? "mm " : "in ");
    if (weather.rainfall > 0.005) {
      drawString(170, 66, String(weather.rainfall, 1) + unit + TXT_PRECIPITATION_SOON, LEFT);
    } else {
      // Rain has precedence over snow if both reported!
      drawString(170, 66, String(weather.snowfall, 1) + unit + TXT_PRECIPITATION_SOON, LEFT);
    }
  }
  drawPressureTrend(3, 52, weather.pressure, weather.trend);
  display.setFont(&FreeSansBold12pt7b); // Bold 12 pixel
  String description = weather.forecast0;
  if (weather.forecast1 != "") {
    description += " & " +  weather.forecast1;
  }
  if (weather.forecast2 != "" && weather.forecast1 != weather.forecast2) {
    description += " & " +  weather.forecast2;
  }
  drawString(2, 67, titleCase(description), LEFT);
  display.drawLine(0, 77, 296, 77, GxEPD_BLACK);
}

void draw3hrForecast(int x, int y, int index) {
  Serial.println("display3hrForecast");
  displayIcon(x + 2, y, forecast[index].icon, SMALL_ICON);
  display.setFont(&FreeSansBold9pt7b); // Bold 8 pixel
  drawString(x + 4, y - 22, forecast[index].period.substring(11, 16), CENTER);
  drawString(x - 15, y + 15, String(forecast[index].high, 0) + "°/" + String(forecast[index].low, 0) + "°", LEFT);
  display.drawLine(x + 28, 77, x + 28, 129, GxEPD_BLACK);
}

void displayAstronomySection(int x, int y) {
  Serial.println("displayAstronomySection");
  display.drawRect(x, y + 13, 173, 52, GxEPD_BLACK);
  display.setFont(&FreeSansBold9pt7b); // Bold 8 pixel
  drawString(x + 5, y + 18, convertUnixTime(weather.sunrise).substring(0, (UNITS == UNITS_METRIC ? 5 : 7)) + " " + TXT_SUNRISE, LEFT);
  drawString(x + 5, y + 34, convertUnixTime(weather.sunset).substring(0, (UNITS == UNITS_METRIC ? 5 : 7)) + " " + TXT_SUNSET, LEFT);
  time_t now = time(NULL);
  struct tm *nowUtc = gmtime(&now);
  const int dayUtc = nowUtc->tm_mday;
  const int monthUtc = nowUtc->tm_mon + 1;
  const int yearUtc = nowUtc->tm_year + 1900;
  drawString(x + 5, y + 50, moonPhase(dayUtc, monthUtc, yearUtc, HEMISPHERE), LEFT);
  drawMoon(x + 90, y, dayUtc, monthUtc, yearUtc, HEMISPHERE);
}

void displayWeather(JsonVariant doc) {
  Serial.println("displayWeather");
  decodeWeather(doc);
  // HERE
#if 0
  updateLocalTime();
#endif
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    // Top line of the display
    drawHeadingSection();
    // Centre section of display for Location, temperature, Weather report,
    // Wx Symbol and wind direction
    drawMainWeatherSection();
    // First 3hr forecast box
    draw3hrForecast(20, 102, 1);
    // Second 3hr forecast box
    draw3hrForecast(70, 102, 2);
    // Third 3hr forecast box
    draw3hrForecast(120, 102, 3);
    // Astronomy section Sun rise/set and Moon phase plus icon
    displayAstronomySection(148, 64);
    if (weather.visibility > 0) {
      visibility(110, 40, String(weather.visibility) + "M");
    }
    if (weather.cloudcover > 0) {
      cloudCover(110, 55, weather.cloudcover);
    }
    drawBattery(55, 12);
  } while (display.nextPage());
}
