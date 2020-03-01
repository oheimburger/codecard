/*
  httpClient.h
*/

String httpRequest(WiFiClient& client, String httpMethod, String url, String host, String btnLabel, int btnFunction) {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String contentType;
  String responseString;
  client.print(httpMethod + " " + url + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" +
               "X-CODECARD-ID: " + mac + "\r\n" +
               "X-CODECARD-BUTTON-LABEL: " + btnLabel + "\r\n" +
               "X-CODECARD-BUTTON-FUNCTION: " + String(btnFunction) + "\r\n" +
               "Accept: */*\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(F("  Request timeout!"));
      Serial.println(F(">>>"));
      return "";
    }
  }
  //  if (client.println() == 0) {
  //    Serial.println(F("  Failed to send request"));
  //    Serial.println(F(">>>"));
  //    return "";
  //  }
  //  char status[32] = {0};
  //  client.readBytesUntil('\r', status, sizeof(status));
  //  if (strcmp(status, "HTTP/1.0 200 OK") != 0) {
  //    Serial.print(F("  Unexpected response: "));
  //    Serial.println(status);
  //    client.stop();
  //    return "";
  //  }
  char findConntentType[] = "ype: ";
  if (client.find(findConntentType)) {
    contentType = client.readStringUntil('\n');
    Serial.println(contentType);
  }
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("  Invalid response"));
    return "";
  }
  if (contentType.indexOf("json") > -1) {
    responseString = client.readStringUntil('}');
    responseString = responseString + "}";
  } else {
    responseString = client.readStringUntil('\r\n');
  }
  client.stop();
  return responseString;
}

String secureRequest(String host, int port, String url, String btnLabel, int btnFunction) {
  WiFiClientSecure client;
  String methodKey =  "method" + btnLabel + String(btnFunction);
  String httpMethod = getFromMemory(methodKey);
  String shaKey = "fingerprint" + btnLabel + String(btnFunction);
  String fingerprint = getFromMemory(shaKey);
  Serial.println(F("Request:"));
  Serial.print(F("  host: ")); Serial.println(host);
  Serial.print(F("  port: ")); Serial.println(port);
  Serial.print(F("  url: ")); Serial.println(url);
  Serial.print(F("  fingerprint: ")); Serial.println(fingerprint);
  Serial.print(F("  method: ")); Serial.println(httpMethod);
  if (fingerprint == "") {
    Serial.println(F(""));
    Serial.println(F("  This SSL/TLS url needs to have a corresponding SHA fingerprint."));
    Serial.println(F("  Add one by typing 'fingerprint[btn][fn]=XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX'."));
    Serial.println("  To learn how to retrieve an SSL/TLS SHA fingerprint go to " + String(projectSite));
    Serial.println(F(">>>"));
    return "";
  }
  client.setTimeout(10000);
  if (!client.connect(host, port)) {
    Serial.println(F("  Connection failed"));
    Serial.println(F(">>>"));
    return "";
  }
  if (!client.verify(fingerprint.c_str(), host.c_str())) {
    Serial.println(F("  Connection insecure! Halting execution."));
    Serial.println(F("  Ensure that the configured fingerprint matches the SHA fingerprint for the SSL/TLS URL."));
    Serial.println("  To learn how to retrieve an SSL/TLS SHA fingerprint go to " + String(projectSite));
    Serial.println(F(">>>"));
    return "";
  }
  return httpRequest(client, httpMethod, url, host, btnLabel, btnFunction);
}

String request(String host, int port, String url, String btnLabel, int btnFunction) {
  WiFiClient client;
  String methodKey = "method" + btnLabel + String(btnFunction);
  String httpMethod = getFromMemory(methodKey);
  Serial.println(F("Request:"));
  Serial.print(F("  host: ")); Serial.println(host);
  Serial.print(F("  port: ")); Serial.println(port);
  Serial.print(F("  url: ")); Serial.println(url);
  Serial.print(F("  method: ")); Serial.println(httpMethod);
  client.setTimeout(10000);
  if (!client.connect(host, port)) {
    Serial.println(F("  Connection failed"));
    Serial.println(F(">>>"));
    return "";
  }
  return httpRequest(client, httpMethod, url, host, btnLabel, btnFunction);
}

void httpsImage(String host, int port, String url, int16_t x, int16_t y, String fingerprint, bool with_color) {
  WiFiClientSecure secureClient;
  bool connection_ok = false;
  uint32_t startTime = millis();
  if ((x >= display.width()) || (y >= display.height())) {
    return;
  }
  Serial.println(F("Request:"));
  Serial.print(F("  host: ")); Serial.println(host);
  Serial.print(F("  port: ")); Serial.println(port);
  Serial.print(F("  url: ")); Serial.println(url);
  Serial.print(F("  fingerprint: ")); Serial.println(fingerprint);
  Serial.println(F("  method: GET"));
  // const char* fingerprintChar = fingerprint.c_str();
  // secureClient.setFingerprint(fingerprintChar);
  // secureClient.setInsecure();
  // const uint8_t fp[20] = {0x6F, 0x9C, 0xC2, 0xBF, 0x5B, 0xBD, 0xF0, 0x50, 0xEA, 0x6E, 0x70, 0x10, 0x19, 0xDF, 0x32, 0xCD, 0x79, 0x01, 0x4C, 0x67};
  // secureClient.setFingerprint(fp);
  secureClient.setTimeout(10000);
  if (!secureClient.connect(host, port)) {
    Serial.println("  SSL connection failed");
    Serial.println(">>>");
    return;
  }
  if (!secureClient.verify(fingerprint.c_str(), host.c_str())) {
    Serial.println(F("  Connection insecure! Halting execution."));
    Serial.println(F("  Ensure that the configured fingerprint matches the SHA fingerprint for the SSL/TLS URL."));
    Serial.println("  To learn how to retrieve an SSL/TLS SHA fingerprint go to " + String(projectSite));
    Serial.println(F(">>>"));
    return;
  }
  secureClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "User-Agent: CodeCard\r\n" +
                     "Connection: close\r\n\r\n");
  while (secureClient.connected()) {
    String line = secureClient.readStringUntil('\n');
    if (!connection_ok) {
      connection_ok = line.startsWith("HTTP/1.1 200 OK"); // || line.startsWith("HTTP/1.1 30");
      if (connection_ok) {
        Serial.println(F("Response:"));
        Serial.print(F("  ")); Serial.println(line);
      }
      // if (!connection_ok) {
      //   Serial.println(line);
      // }
    }
    if (!connection_ok) {
      Serial.println(line);
    }
    if (line == "\r") {
      break;
    }
  }
  if (!connection_ok) {
    return;
  }
  displayImageFromUrl(secureClient, x, y, connection_ok, with_color);
}

void httpImage(String host, int port, String url, int16_t x, int16_t y, bool with_color) {
  WiFiClient client;
  bool connection_ok = false;
  if ((x >= display.width()) || (y >= display.height())) {
    return;
  }
  Serial.println(F("Request:"));
  Serial.print(F("  host: ")); Serial.println(host);
  Serial.print(F("  port: ")); Serial.println(port);
  Serial.print(F("  url: ")); Serial.println(url);
  Serial.println(F("  method: GET"));
  if (!client.connect(host, port)) {
    Serial.println("  Connection failed");
    return;
  }
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: CodeCard\r\n" +
               "Connection: close\r\n\r\n");
  // Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (!connection_ok) {
      connection_ok = line.startsWith("HTTP/1.1 200 OK"); // || line.startsWith("HTTP/1.1 30");
      if (connection_ok) {
        Serial.println(line);
      }
      // if (!connection_ok) {
      //   Serial.println(line);
      // }
    }
    if (!connection_ok) {
      Serial.println(line);
    }
    // Serial.println(line);
    if (line == "\r") {
      // Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) {
    return;
  }
  displayImageFromUrl(client, x, y, connection_ok, with_color);
}

void imageFromUrl(String url, int16_t x, int16_t y, String fingerprint, bool with_color) {
  String protocol = parseValue(url, '/', 0);
  String host = parseValue(url, '/', 2);
  String portString = parseValue(host, ':', 1) ;
  host = parseValue(host, ':', 0);
  if (protocol == "https:") {
    int port = (portString.length() != 0) ? portString.toInt() : 443;
    httpsImage(host, port, url, x, y, fingerprint, with_color);
  } else {
    int port = (portString.length() != 0) ? portString.toInt() : 80;
    httpImage(host, port, url, x, y, with_color);
  }
}
