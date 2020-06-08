// -*- C++ -*-
/*
  utils.h
*/

String parseValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String getBinCode(char str) {
  if (str == '@') {
    return String("010010100");
  }
  if (str == '0') {
    return String("000110100");
  }
  if (str == '1') {
    return String("100100001");
  }
  if (str == '2') {
    return String("001100001");
  }
  if (str == '3') {
    return String("101100000");
  }
  if (str == '4') {
    return String("000110001");
  }
  if (str == '5') {
    return String("100110000");
  }
  if (str == '6') {
    return String("001110000");
  }
  if (str == '7') {
    return String("000100101");
  }
  if (str == '8') {
    return String("100100100");
  }
  if (str == '9') {
    return String("001100100");
  }
  if (str == 'A') {
    return String("100001001");
  }
  if (str == 'B') {
    return String("001001001");
  }
  if (str == 'C') {
    return String("101001000");
  }
  if (str == 'D') {
    return String("000011001");
  }
  if (str == 'E') {
    return String("100011000");
  }
  if (str == 'F') {
    return String("001011000");
  }
  if (str == 'G') {
    return String("000001101");
  }
  if (str == 'H') {
    return String("100001100");
  }
  if (str == 'I') {
    return String("001001100");
  }
  if (str == 'J') {
    return String("000011100");
  }
  if (str == 'K') {
    return String("100000011");
  }
  if (str == 'L') {
    return String("001000011");
  }
  if (str == 'M') {
    return String("101000010");
  }
  if (str == 'N') {
    return String("000010011");
  }
  if (str == 'O') {
    return String("100010010");
  }
  if (str == 'P') {
    return String("001010010");
  }
  if (str == 'Q') {
    return String("000000111");
  }
  if (str == 'R') {
    return String("100000110");
  }
  if (str == 'S') {
    return String("001000110");
  }
  if (str == 'T') {
    return String("000010110");
  }
  if (str == 'U') {
    return String("110000001");
  }
  if (str == 'V') {
    return String("011000001");
  }
  if (str == 'W') {
    return String("111000000");
  }
  if (str == 'X') {
    return String("010010001");
  }
  if (str == 'Y') {
    return String("110010000");
  }
  if (str == 'Z') {
    return String("011010000");
  }
  if (str == '-') {
    return String("010000101");
  }
  if (str == '.') {
    return String("110000100");
  }
  if (str == ' ') {
    return String("011000100");
  }
  if (str == '*') {
    return String("010010100");
  }
  if (str == '$') {
    return String("010101000");
  }
  if (str == '/') {
    return String("010100010");
  }
  if (str == '+') {
    return String("010001010");
  }
  if (str == '%') {
    return String("000101010");
  }
  if (str == '#') {
    return String("010010100");
  }
}

//
// Wifi Images Helper
void tryToWaitForAvailable(WiFiClient& client, int32_t amount) {
  // this doesn't work as expected, but it helps for long downloads
  int32_t start = millis();
  for (int16_t t = 0, dly = 50; t < 20; t++, dly += 50) {
    if (client.available()) {
      break; // read would not recover after having returned 0
    }
    delay(dly);
  }
  for (int16_t t = 0, dly = 50; t < 10; t++, dly += 25) {
    if (amount <= client.available()) {
      break;
    }
    delay(dly);
    // stays constant
    // Serial.print("available "); Serial.println(client.available());
  }
  int32_t elapsed = millis() - start;
  if (elapsed > 225) {
    // usually 0 or 225ms
    Serial.print("  waited for available "); Serial.print(millis() - start); Serial.println(" ms");
  }
}

// BMP data is stored little-endian, same as Arduino.
uint16_t read16(WiFiClient& client) {
  uint16_t result;
  ((uint8_t *)&result)[0] = client.read(); // LSB
  ((uint8_t *)&result)[1] = client.read(); // MSB
  return result;
}

// BMP data is stored little-endian, same as Arduino.
uint32_t read32(WiFiClient& client) {
  uint32_t result;
  ((uint8_t *)&result)[0] = client.read(); // LSB
  ((uint8_t *)&result)[1] = client.read();
  ((uint8_t *)&result)[2] = client.read();
  ((uint8_t *)&result)[3] = client.read(); // MSB
  return result;
}

uint32_t skip(WiFiClient& client, int32_t bytes) {
  int32_t remain = bytes;
  int16_t retries = 3;
  uint32_t chunk = 1024;
  if (chunk > sizeof(input_buffer)) {
    chunk = sizeof(input_buffer);
  }
  while (client.connected() && (remain > chunk)) {
    // there seems an issue with long downloads on ESP8266
    tryToWaitForAvailable(client, chunk);
    uint32_t got = client.read(input_buffer, chunk);
    // Serial.print("skipped "); Serial.println(got);
    remain -= got;
    if ((0 == got) && (0 == --retries)) {
      Serial.print("Error on skip, got 0, skipped "); Serial.print(bytes - remain); Serial.print(" of "); Serial.println(bytes);
      break; // don't hang forever (retries don't help)
    }
  }
  if (client.connected() && (remain > 0) && (remain <= chunk)) {
    remain -= client.read(input_buffer, remain);
  }
  // bytes read and skipped
  return bytes - remain;
}
