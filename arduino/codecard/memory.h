// -*- C++ -*-
/*
  memory.h
*/

int getKeyIndex(String val) {
  int ret = -1;
  for (int i = 0; i < KEYS_LEN; i++) {
    if (String(KEYS[i]) == val) {
      ret = i;
      break;
    }
  }
  return ret;
}

String getFromMemory(int addr) {
  char arrayToStore[100];
  if (EEPROM.read(addr * MAX_VALUE) == 255) {
    return "";
  }
  EEPROM.get(addr * MAX_VALUE, arrayToStore);
  String ret(arrayToStore);
  return ret;
}

String getFromMemory(String key) {
  String ret = "";
  for (int i = 0; i < KEYS_LEN; i++) {
    if (String(KEYS[i]) == key) {
      String key = KEYS[i];
      ret = getFromMemory(i);
      if (key.indexOf("method") > -1 && ret == "") {
        ret = "GET";
      }
      if (key.indexOf("initalsetup") > -1 && ret == "") {
        ret = "true";
      }
      break;
    }
  }
  return ret;
}

void saveToMemory(int addr, String val) {
  char arrayToStore[100];
  val.toCharArray(arrayToStore, val.length() + 1) ;
  EEPROM.put(addr * MAX_VALUE, arrayToStore);
  EEPROM.commit();
}
