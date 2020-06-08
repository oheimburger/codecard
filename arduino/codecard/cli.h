// -*- C++ -*-
/*
  cli.h
*/

void help() {
  Serial.println(F("***************************************************************************************"));
  Serial.println("  " + String(PROJECT_NAME) + " " + String(PROJECT_VERSION));
  Serial.println("  " + String(PROJECT_AUTHOR));
  Serial.println("  " + String(PROJECT_SITE));
  Serial.println(F("***************************************************************************************"));
  Serial.println(F("Commands:"));
  Serial.println(F("  ls                Show all stored key/values"));
  Serial.println(F("  help              Show this help"));
  Serial.println(F("  shortpress[a|b]   Simulate the press of a button"));
  Serial.println(F("  longpress[a|b]    Simulate the long press of a button"));
  Serial.println(F("  connect           Connect to wifi"));
  Serial.println(F("  disconnect        Disconnect wifi"));
  Serial.println(F("  restart           Restart wifi"));
  Serial.println(F("  status            Show wifi status"));
  Serial.println(F("  home              Show home screen"));
  Serial.println(F("  battery           Show battery status"));
  Serial.println(F("  reset             Reset to factory settings"));
  Serial.println();
  Serial.println(F("Usage:"));
  Serial.println(F("  Read saved key value:"));
  Serial.println(F("    key"));
  Serial.println(F("  Save new key value:"));
  Serial.println(F("    key=[value]"));
  Serial.println();
  Serial.println(F("Available keys:"));
  Serial.print(F("  "));
  for (int i = 0; i < KEYS_LEN; i++) {
    if (i == 8) {
      Serial.println(F(""));
      Serial.print(F("  "));
    }
    if (KEYS[i] == "initalsetup") {
      continue;
    }
    Serial.print(KEYS[i]);
    if (i != KEYS_LEN - 1) {
      Serial.print(F(", "));
    }
  }
  Serial.println(F(""));
  Serial.println(F(">>>"));
}

void showStatus() {
  Serial.print(F("IP address: ")); Serial.println(WiFi.localIP());
  Serial.print(F("MAC address: ")); Serial.println(WiFi.macAddress());
  WiFi.printDiag(Serial);
}

void batteryStatus() {
  Serial.print(F("Battery status: ")); Serial.println(ESP.getVcc());
}

void listAll() {
  for (int i = 0; i < KEYS_LEN; i++) {
    String key = KEYS[i];
    if (key.indexOf("initalsetup") > -1) {
      continue;
    }
    Serial.print(key); Serial.print(F(": "));
    String val = getFromMemory(i);
    if (key.indexOf("method") > -1 && val == "") {
      val = "GET";
    }
    // if (key.indexOf("initalsetup") > -1 && val == "") {
    //   val = "true";
    // }
    Serial.println(val);
  }
}

void eraseAll() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  WiFi.disconnect(true);
  Serial.println(F("All values have been erased!"));
  listAll();
}

void loadDefaults() {
  for (int i = 0; i < KEYS_LEN; i++) {
    String key = KEYS[i];
    String val = "";
    if (key.indexOf("ssid") > -1 && val == "") {
      saveToMemory(i, DEFAULT_SSID);
    }
    if (key.indexOf("password") > -1 && val == "") {
      saveToMemory(i, DEFAULT_PASSWORD);
    }
    if (key.indexOf("button") > -1 && val == "") {
      saveToMemory(i, DEFAULT_URL);
    }
    if (key.indexOf("fingerprint") > -1 && val == "") {
      saveToMemory(i, DEFAULT_FINGERPRINT);
    }
  }
  saveToMemory(getKeyIndex("initalsetup"), "false");
  Serial.println(F("Default urls and sha fingerprints loaded."));
}

void evalInput(String input) {
  String key = parseValue(input, '=', 0);
  String val = parseValue(input, '=', 1);
  if (key == "ls" || key == "ll") {
    listAll();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "help") {
    help();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "eraseall") {
    eraseAll();  // not displayed under help
    Serial.println(F(">>>"));
    return;
  }
  if (key == "status") {
    showStatus();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "connect") {
    connectWifi();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "disconnect") {
    disconnectWifi();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "restart") {
    restartWifi();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "heap") {
    Serial.println("Free HEAP: " + String(ESP.getFreeHeap()));
    Serial.println(F(">>>"));
    return;
  }
  if (key == "home") {
    defaultScreen();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "battery") {
    batteryStatus();
    Serial.println(F(">>>"));
    return;
  }
  if (key == "reset") {
    loadDefaults();
    Serial.println(F(">>>"));
    return;
  }
  if (key.indexOf("longpress") > -1) {
    String btnLabel = key.substring(9);
    if (btnLabel == "a" || btnLabel == "b") {
      pushButton(btnLabel, 2);
    } else {
      Serial.println(F("Not a valid button!"));
    }
    Serial.println(F(">>>"));
    return;
  }
  if (key.indexOf("shortpress") > -1) {
    String btnLabel = key.substring(10);
    if (btnLabel == "a" || btnLabel == "b") {
      pushButton(btnLabel, 1);
    } else {
      Serial.println(F("Not a valid button!"));
    }
    Serial.println(F(">>>"));
    return;
  }
  int keyIndex = getKeyIndex(key);
  if (keyIndex == -1) {
    Serial.print(F("'")); Serial.print(key); Serial.print(F("'")); Serial.println(F(" is not a valid key or command! Type 'help' or 'ls' for more info."));
    Serial.println(F(">>>"));
    return;
  }
  if (val != "") {
    if (key.indexOf("method") > -1) {
      val.toUpperCase();
    }
    if (val == "\"\"" || val == "''") {
      val = "";
    }
    saveToMemory(keyIndex, val);
    Serial.print(F("Value saved for ")); Serial.print(key); Serial.print(F(": ")); Serial.println(val);
    Serial.println(F(">>>"));
    if (key == "ssid" || key == "password") {
      Serial.println(F("Use the command 'restart' for changes to take effect."));
      Serial.println(F(">>>"));
      return;
    }
  } else {
    Serial.print(key); Serial.print(F(": "));
    String val = getFromMemory(keyIndex);
    if (key.indexOf("method") > -1 && val == "") {
      val = "GET";
    }
    Serial.println(val);
    Serial.println(F(">>>"));
  }
}

void configure(String key, String value) {
  if (key == "") {
    return;
  }
  int keyIndex = getKeyIndex(key);
  if (keyIndex == -1) {
    return;
  }
  if (value != "") {
    if (key.indexOf("method") > -1) {
      value.toUpperCase();
    }
    saveToMemory(keyIndex, value);
    Serial.print(F("Value saved for ")); Serial.print(key); Serial.print(F(": ")); Serial.println(value);
  }
}
