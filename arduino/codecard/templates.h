/*
  template.h
*/

void imageFromUrl(String url, int16_t x, int16_t y, String fingerprint ="", bool with_color =true);

void custom(String title, String titleFont, int titleX, int titleY, String subtitle, String subtitleFont, int subtitleX, int subtitleY, String body, String  bodyFont, int bodyX, int bodyY, String icon, String badge, int iconX, int iconY, int iconSize, String backgroundColor, String backgroundImage, String fingerprint);

inline bool isLocalIcon(String icon) {
  return (icon.length() > 0 && icon.indexOf("http") == -1);
}

inline String textFits(String text, int length) {
  return (text.length() > length) ? text.substring(0, length) : text;
}

void drawBarcode39(int x, int y, int width, int height, int pitch, String barcode) {
  int c = 0;
  display.fillRect(x, y, width, height, GxEPD_WHITE);
  c = (pitch * 10);
  for (int i = 0; i < barcode.length(); i++) {
    String binCode = getBinCode(barcode.charAt(i));
    bool bk = true;
    for (int j = 0; j < 9; j++) {
      int color = (bk) ? GxEPD_BLACK : GxEPD_WHITE;
      bk = !bk;
      int bw = (binCode.charAt(j) == '0') ? pitch : pitch * 2;
      display.fillRect(c, y, bw, height, color);
      c = c + bw;
    }
    // Gap
    display.fillRect(c, y, pitch, height, GxEPD_WHITE);
    // c = c + pitch;
    c += pitch;
  }
}

void drawIcon64(int x, int y, String icon, long color) {
  const uint8_t *iconMap = weather01;
  if (icon == "01d" || icon == "01n") {
    iconMap = weather01;
  } else if (icon == "02d" || icon == "02n") {
    iconMap = weather02;
  } else if (icon == "03d" || icon == "03n") {
    iconMap = weather03;
  } else if (icon == "04d" || icon == "04n") {
    iconMap = weather04;
  } else if (icon == "09d" || icon == "09n") {
    iconMap = weather09;
  } else if (icon == "10d" || icon == "10n") {
    iconMap = weather10;
  } else if (icon == "11d" || icon == "11n") {
    iconMap = weather11;
  } else if (icon == "13d" || icon == "13n") {
    iconMap = weather13;
  } else if (icon == "50d" || icon == "50n") {
    iconMap = weather50;
  } else if (icon == "twitter") {
    iconMap = twitter64;
  } else if (icon == "mail") {
    iconMap = mail64;
  } else if (icon == "fail") {
    iconMap = fail64;
  } else if (icon == "ace") {
    iconMap = ace64;
  } else if (icon == "duke") {
    iconMap = duke64;
  } else if (icon == "oracle") {
    iconMap = oracle64;
  } else if (icon == "champion") {
    iconMap = champion64;
  } else if (icon == "jduchess") {
    iconMap = jduchess64;
  } else if (icon == "db") {
    iconMap = db64;
  } else if (icon == "java") {
    iconMap = java64;
  } else if (icon == "containers") {
    iconMap = containers64;
  } else if (icon == "microservices") {
    iconMap = microservices64;
  } else if (icon == "opensource") {
    iconMap = opensource64;
  } else if (icon == "chatbots") {
    iconMap = chatbots64;
  } else if (icon == "blockchain") {
    iconMap = blockchain64;
  } else if (icon == "javascript") {
    iconMap = javascript64;
  } else if (icon == "nighthacker") {
    iconMap = nighthacker64;
  } else if (icon == "wifi") {
    iconMap = wifi64;
  }
  display.drawInvertedBitmap(x, y, iconMap, 64, 64, color);
}

void drawIcon128(int x, int y, String icon, long color) {
  const uint8_t *iconMap = oracle128;
  if (icon == "oracle") {
    iconMap = oracle128;
  } else if (icon == "champion") {
    iconMap = champion128;
  } else if (icon == "jduchess") {
    iconMap = jduchess128;
  } else if (icon == "db") {
    iconMap = db128;
  } else if (icon == "java") {
    iconMap = java128;
  } else if (icon == "containers") {
    iconMap = containers128;
  } else if (icon == "microservices") {
    iconMap = microservices128;
  } else if (icon == "opensource") {
    iconMap = opensource128;
  } else if (icon == "chatbots") {
    iconMap = chatbots128;
  } else if (icon == "blockchain") {
    iconMap = blockchain128;
  } else if (icon == "javascript") {
    iconMap = javascript128;
  } else if (icon == "nighthacker") {
    iconMap = nighthacker128;
  } else if (icon == "wifi") {
    iconMap = wifi128;
  }
  display.drawInvertedBitmap(x, y, iconMap, 128, 128, color);
}

void drawBackground(int x, int y, String backgroundImage, long color) {
  const uint8_t *iconMap = codeone264;
  if (backgroundImage == "codeone") {
    iconMap = codeone264;
  }
  if (backgroundImage == "oracle") {
    iconMap = oracle264;
  }
  display.drawInvertedBitmap(x, y, iconMap, display.width(), display.height(), color);
}

void drawBadge(int x, int y, int radius, String badge, long color) {
  int circleColor = (color == GxEPD_BLACK) ?  GxEPD_WHITE : GxEPD_BLACK;
  int badgeText = (color == GxEPD_BLACK) ? GxEPD_BLACK : GxEPD_WHITE;
  // int radius = 32;
  bool isDegree = false;
  if (badge.indexOf("u00B0") > -1 ) {
    badge.replace("u00B0", "");
    isDegree = true;
  }
  display.fillCircle(x + radius, y + radius, radius - 2, circleColor);
  display.setTextColor(badgeText);
  if (badge.length() > 1 && badge.length() < 3) {
    // Two characters
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(x  + 11, y + 42);
  } else if (badge.length() == 1 ) {
    // One character
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(x + 21, y + 44);
  } else if (badge.length() > 2 ) {
    // Three characters
    display.setFont(&FreeMonoBold12pt7b);
    display.setCursor(x + 11, y + 40);
  }
  display.print(badge);
  if (isDegree == true) {
    display.setFont(&TomThumb);
    display.setTextSize(2);
    // display.setCursor(x + radius + radius / 2 + 5, y + radius - radius / 3 + 5);
    display.setCursor(x + radius + radius / 2, y + radius - radius / 3);
    display.print("o");
  }
  display.setTextSize(1);
  display.setTextColor(circleColor);
}

void template1(String title, String subtitle, String body, String icon, String badge, String backgroundColor, String fingerprint) {
  int iconX = 0;
  int iconY = 0;
  int iconSize = 64;
  title = textFits(title, 16);
  subtitle = textFits(subtitle, 20);
  body = textFits(body, 170);
  custom(title, "FreeSansBold12pt", 72, 25, subtitle, "FreeSans9pt", 73, 45, body, "FreeSans9pt", 0, 80, icon, badge, iconX, iconY, iconSize, backgroundColor, "", fingerprint);
}

void template2(String title, String subtitle, String body, String icon, String badge, String backgroundColor, String fingerprint) {
  title = textFits(title, 22);
  subtitle = textFits(subtitle, 26);
  body = textFits(body, 105);
  int iconSize = 64;
  int iconX = 0;
  int iconY = 0;
  int middle = display.width() / 2 - 38;
  if (badge != "") {
    iconX = middle;
    iconY = 40;
  } else if (isLocalIcon(icon)) {
    iconX = middle;
    iconY = 42;
  } else {
    iconX = display.height() / 2 - 42;
    iconY = middle;
  }
  custom(title, "FreeSansBold12pt", 0, 20, subtitle, "FreeSans9pt", 0, 38, body, "FreeSans9pt", 0, 125, icon, badge, iconX, iconY, iconSize, backgroundColor, "", fingerprint);
}

void template3(String title, String subtitle, String body, String icon, String badge, String backgroundColor, String fingerprint) {
  title = textFits(title, 16);
  subtitle = textFits(subtitle, 24);
  body = textFits(body, 24);
  int iconX = display.width() - 68;
  int iconY = 0;
  int iconSize = 64;
  if (!isLocalIcon(icon)) {
    iconX = 0;
    iconY = -display.width() + 64;
  }
  custom(title, "FreeSansBold12pt", 0, 25, subtitle, "FreeSans9pt", 1, 43, body, "FreeSans9pt", 0, 80, icon, badge, iconX, iconY, iconSize, backgroundColor, "", fingerprint);
}

void template4(String title, String subtitle, String body, String icon, String badge, String backgroundColor, String fingerprint) {
  title = textFits(title, 20);
  subtitle = textFits(subtitle, 32);
  body = textFits(body, 170);
  int iconX = display.width() - 68;
  int iconY = display.height() - 68;
  int iconSize = 64;
  if (!isLocalIcon(icon)) {
    iconX = display.height() - 64;
    iconY = -display.width() + 64;
  }
  custom(title, "FreeSansBold12pt", 0, 25, subtitle, "FreeSans9pt", 0, 43, body, "FreeSans9pt", 0, 70, icon, badge, iconX, iconY, iconSize, backgroundColor, "", fingerprint);
}

void template5(String title, String subtitle, String body, String backgroundColor) {
  title = textFits(title, 15);
  subtitle = textFits(subtitle, 25);
  body = textFits(body, 170);
  int titleX = (display.width() / 2 - title.length() / 2 * 18) - 10;
  int subtitleX = (display.width() / 2 - subtitle.length() / 2 * 9) - 12;
  custom(title, "FreeSansBold18pt", titleX, 25, subtitle, "FreeSansBold9pt", subtitleX, 45, body, "FreeSans9pt", 0, 72, "", "", -1, -1, -1, backgroundColor, "", "");
}

void template6(String title, String subtitle, String body, String backgroundColor) {
  title = textFits(title, 15);
  subtitle = textFits(subtitle, 25);
  body = textFits(body, 80);
  int titleX = (display.width() / 2 - title.length() / 2 * 18) - 10;
  int subtitleX = (display.width() / 2 - subtitle.length() / 2 * 9) - 12;
  custom(title, "FreeSansBold18pt", titleX, 80, subtitle, "FreeSansBold9pt", subtitleX, 100, body, "FreeSans9pt", 1, 146, "", "", -1, -1, -1, backgroundColor, "", "");
}

// Minimalist - No Icon
void template7(String title, String subtitle, String backgroundColor, String backgroundImage, String fingerprint) {
  if (backgroundImage == "") {
    backgroundImage = "oracle";
  }
  title = textFits(title, 22);
  subtitle = textFits(subtitle, 105);
  int subtitleX = display.width() / 2 - subtitle.length() / 2 * 9;
  int subtitleY = display.height() - 15;
  custom(title, "FreeSansBold12pt", 0, 20, subtitle, "FreeSans9pt", subtitleX, subtitleY, "", "", -1, -1, "", "", -1, -1, -1, backgroundColor, backgroundImage, fingerprint);
}

// Minimalist - No Icon
void template8(String backgroundColor, String backgroundImage, String fingerprint) {
  long bgColor = (backgroundColor == "white") ? GxEPD_WHITE : GxEPD_BLACK;
  long txtColor = (backgroundColor == "white") ? GxEPD_BLACK : GxEPD_WHITE;
  if (backgroundImage == "") {
    backgroundImage = "oracle";
  }
  display.setFullWindow();
  display.firstPage();
  do {
    if (backgroundImage.indexOf("http") == -1) {
      display.fillScreen(txtColor);
      drawBackground(0, 0, backgroundImage, bgColor);
    }
  } while (display.nextPage());
  if (backgroundImage.indexOf("http") > -1) {
    imageFromUrl(backgroundImage, 0, 0, fingerprint, false);
  }
}

// status
void template9(String title, String subtitle, String backgroundColor) {
  long bgColor = (backgroundColor == "white") ? GxEPD_WHITE : GxEPD_BLACK;
  long txtColor = (backgroundColor == "white") ? GxEPD_BLACK : GxEPD_WHITE;
  display.setFullWindow();
  // display.setPartialWindow(0, 0, display.width(), display.height());
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(25, 80);
    display.println(textFits(title, 50));
    display.setFont(&FreeMono9pt7b);
    display.setCursor(25, 100);
    display.println(textFits(subtitle, 50));
  } while (display.nextPage());
}

// barcode template 43 max chars
void template10(String title, String subtitle, String body, String backgroundColor, String barcode) {
  long bgColor = (backgroundColor == "white") ? GxEPD_WHITE : GxEPD_BLACK;
  long txtColor = (backgroundColor == "white") ? GxEPD_BLACK : GxEPD_WHITE;
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(bgColor);
    display.setTextColor(txtColor);
    display.setFont(&FreeSansBold18pt7b);
    display.setCursor(0, 30);
    display.println(textFits(title, 15));
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(1, 50);
    display.println(textFits(subtitle, 26));
    display.setFont(&FreeSans9pt7b);
    display.setCursor(0, 77);
    display.println(textFits(body, 65));
    String formattedBarcode = String("@" + barcode + "#");
    drawBarcode39(50, 110, display.width(), 50, 1.9, formattedBarcode);
    display.setFont(&FreeMono9pt7b);
    int barcodeX = display.width() / 2 - barcode.length() / 2 * 5;
    display.setCursor(12, 172);
    display.println(barcode);
  } while (display.nextPage());
}

void template11(String title, String subtitle, String icon, String badge, String backgroundColor, String fingerprint) {
  int titleX = 126 - (title.length() / 2 * 12);
  int titleY = 150;
  int subtitleX = 126 - (subtitle.length() / 2 * 9);
  int subtitleY = 170;
  int iconX = 0;
  int iconY = 0;
  if (!isLocalIcon(icon) && badge == "") {
    iconX = display.height() / 2 - 64;
    iconY = display.width() / 2 - 68;
  }
  title = textFits(title, 22);
  subtitle = textFits(subtitle, 22);
  custom(title, "FreeSansBold12pt", titleX, titleY, subtitle, "FreeSans9pt", subtitleX, subtitleY, "", "", -1, -1, icon, badge, iconX, iconY, 128, backgroundColor, "", fingerprint);
}

void custom(String title, String titleFont, int titleX, int titleY, String subtitle, String subtitleFont, int subtitleX, int subtitleY, String body, String  bodyFont, int bodyX, int bodyY, String icon, String badge, int iconX, int iconY, int iconSize, String backgroundColor, String backgroundImage, String fingerprint) {
  long bgColor = (backgroundColor == "black") ? GxEPD_BLACK : GxEPD_WHITE;
  long txtColor = (backgroundColor == "black") ? GxEPD_WHITE : GxEPD_BLACK;
  display.setFullWindow();
  display.firstPage();
  do {
    if (backgroundImage != "" && backgroundImage.indexOf("http") == -1) {
      display.fillScreen(txtColor);
      drawBackground(0, 0, backgroundImage, bgColor);
    } else {
      display.fillScreen(bgColor);
    }
    display.setTextColor(txtColor);
    if (badge != "") {
      drawBadge(iconX, iconY, iconSize / 2, badge, bgColor);
    } else if (isLocalIcon(icon) && badge == "") {
      if (iconSize == 64) {
	drawIcon64(iconX, iconY, icon, txtColor);
      } else if (iconSize == 128) {
	drawIcon128(iconX, iconY, icon, txtColor);
      }
    }
    // title
    if (titleFont == "FreeSansBold12pt") {
      display.setFont(&FreeSansBold12pt7b);
    } else if (titleFont == "FreeSansBold18pt") {
      display.setFont(&FreeSansBold18pt7b);
    }
    display.setCursor(titleX, titleY);
    display.println(title);
    // subtitle
    if (subtitleFont == "FreeSans9pt") {
      display.setFont(&FreeSans9pt7b);
    } else if (subtitleFont == "FreeSansBold9pt") {
      display.setFont(&FreeSansBold9pt7b);
    }
    display.setCursor(subtitleX, subtitleY);
    display.println(subtitle);
    // body
    if (body != "") {
      if (bodyFont == "FreeSans9pt") {
	display.setFont(&FreeSans9pt7b);
      }
      display.setCursor(bodyX, bodyY);
      display.println(body);
    }
  } while (display.nextPage());
  if (!isLocalIcon(icon) && badge == "") {
    imageFromUrl(icon, iconX, iconY, fingerprint, false);
  }
}

void defaultScreen() {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  mac.toUpperCase();
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawInvertedBitmap(0, 0, oracle264, display.width(), display.height(), GxEPD_BLACK);
  } while (display.nextPage());
  template10(String(projectName) + " v" + String(projectVersion), projectAuthor, projectSite, "white", mac);
}
