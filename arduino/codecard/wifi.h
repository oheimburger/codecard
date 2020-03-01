/*
  wifi.h
*/

bool wifiConnect() {
  char arrayToStore[100];
  String ssid = EEPROM.get(0 * maxValue, arrayToStore);
  String password = EEPROM.get(1 * maxValue, arrayToStore);
  if (ssid == "") {
    ssid = defaultSSID;
    password = defaultPassword;
    Serial.println();
    Serial.println(F("Connecting to default SSID and Password"));
    Serial.println(F(">>>"));
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print(F("Connecting to '"));
  Serial.print(ssid);
  Serial.print(F("' "));
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if ((millis() - start) > 10000) { // try to connect for 10 seconds.
      Serial.println();
      Serial.print(F("Could not connect to '")); Serial.print(ssid);
      Serial.println(F("'. Please check your ssid and password values. Type 'help' for more info."));
      Serial.println(F(">>>"));
      return false;
    }
  }
  Serial.println(F("connected!"));
  Serial.print(F("IP address: ")); Serial.println(WiFi.localIP());
  Serial.print(F("MAC address: ")); Serial.println(WiFi.macAddress());
  Serial.println(F(">>>"));
  return true;
}

void connectWifi() {
  WiFi.disconnect(true);
  wifiConnect();
}

void disconnectWifi() {
  WiFi.disconnect(true);
  Serial.println(F("Wifi disconnected."));
  Serial.println(F(">>>"));
}

void restartWifi() {
  Serial.println(F("Restarting..."));
  WiFi.disconnect(true);
  wifiConnect();
}
