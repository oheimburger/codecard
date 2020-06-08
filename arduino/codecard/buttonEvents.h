// -*- C++ -*-
/*
  buttonEvents.h
*/

void pushButton(String btnLabel, int btnFunction) {
  String button = "button" + btnLabel + String(btnFunction);
  String typeOfPress = String(" - ") + ((btnFunction == 1) ? "short" : "long") + " pressed";
  String upperCaseBtnLabel = btnLabel;
  upperCaseBtnLabel.toUpperCase();
  InputData data;
  data.title = upperCaseBtnLabel + typeOfPress;
  data.subtitle = "please wait...";
  data.backgroundColor = "white";
  template9(data);
  Serial.println("Button " + btnLabel + typeOfPress);
  Serial.println(F(">>>"));
  if (wifiConnect() == false) {
    data.title = "No Internet";
    data.subtitle = "";
    data.body = "Connect your card to the Code Card Terminal to configure your wifi settings.";
    data.icon = "wifi";
    data.backgroundColor = "white";
    template3(data);
    return;
  }
  String url = getFromMemory(button);
  String protocol = parseValue(url, '/', 0);
  String host = parseValue(url, '/', 2);
  String portString = parseValue(host, ':', 1);
  host = parseValue(host, ':', 0);
#if 1
  if (protocol == "https:") {
    int port = (portString.length() != 0) ? portString.toInt() : 443;
    String response = secureRequest(host, port, url, btnLabel, btnFunction);
    if (response != "") {
      parseJson(response);
    }
  } else {
    int port = (portString.length() != 0) ? portString.toInt() : 80;
    String response = request(host, port, url, btnLabel, btnFunction);
    if (response != "") {
      parseJson(response);
    }
  }
#else
  if (protocol == "https:") {
    int port = (portString.length() != 0) ? portString.toInt() : 443;
    secureRequest(host, port, url, btnLabel, btnFunction);
  } else {
    int port = (portString.length() != 0) ? portString.toInt() : 80;
    request(host, port, url, btnLabel, btnFunction);
  }
#endif
}
