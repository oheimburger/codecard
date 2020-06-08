// -*- C++ -*-
/*
  template.h
*/

void imageFromUrl(String url, int16_t x, int16_t y, String fingerprint ="", bool with_color =true);

// void custom(String title, String titleFont, int titleX, int titleY, String subtitle, String subtitleFont, int subtitleX, int subtitleY, String body, String  bodyFont, int bodyX, int bodyY, String icon, String badge, int iconX, int iconY, int iconSize, String backgroundColor, String backgroundImage, String fingerprint);
void custom(InputData& data);

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
      // c = c + bw;
      c += bw;
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

void template1(InputData& data) {
  data.iconX = 0;
  data.iconY = 0;
  data.iconSize = 64;
  data.title = textFits(data.title, 16);
  data.subtitle = textFits(data.subtitle, 20);
  data.body = textFits(data.body, 170);
  data.titleFont = FREE_SANS_BOLD_12;
  data.titleX = 72;
  data.titleY = 25;
  data.subtitleFont = FREE_SANS_9;
  data.subtitleX = 73;
  data.subtitleY = 45;
  data.bodyFont = FREE_SANS_9;
  data.bodyX = 0;
  data.bodyY = 80;
  custom(data);
}

void template2(InputData& data) {
  data.title = textFits(data.title, 22);
  data.subtitle = textFits(data.subtitle, 26);
  data.body = textFits(data.body, 105);
  data.iconSize = 64;
  data.iconX = 0;
  data.iconY = 0;
  int middle = display.width() / 2 - 38;
  if (data.badge != "") {
    data.iconX = middle;
    data.iconY = 40;
  } else if (isLocalIcon(data.icon)) {
    data.iconX = middle;
    data.iconY = 42;
  } else {
    data.iconX = display.height() / 2 - 42;
    data.iconY = middle;
  }
  data.titleFont = FREE_SANS_BOLD_12;
  data.titleX = 0;
  data.titleY = 20;
  data.subtitleFont = FREE_SANS_9;
  data.subtitleX = 0;
  data.subtitleY = 38;
  data.bodyFont = FREE_SANS_9;
  data.bodyX = 0;
  data.bodyY = 125;
  custom(data);
}

void template3(InputData& data) {
  data.title = textFits(data.title, 16);
  data.subtitle = textFits(data.subtitle, 24);
  data.body = textFits(data.body, 24);
  data.iconX = display.width() - 68;
  data.iconY = 0;
  data.iconSize = 64;
  if (!isLocalIcon(data.icon)) {
    data.iconX = 0;
    data.iconY = -display.width() + 64;
  }
  data.titleFont = FREE_SANS_BOLD_12;
  data.titleX = 0;
  data.titleY = 25;
  data.subtitleFont = FREE_SANS_9;
  data.subtitleX = 1;
  data.subtitleY = 43;
  data.bodyFont = FREE_SANS_9;
  data.bodyX = 0;
  data.bodyY = 80;
  custom(data);
}

void template4(InputData& data) {
  data.title = textFits(data.title, 20);
  data.subtitle = textFits(data.subtitle, 32);
  data.body = textFits(data.body, 170);
  data.iconX = display.width() - 68;
  data.iconY = display.height() - 68;
  data.iconSize = 64;
  if (!isLocalIcon(data.icon)) {
    data.iconX = display.height() - 64;
    data.iconY = -display.width() + 64;
  }
  data.titleFont = FREE_SANS_BOLD_12;
  data.titleX = 0;
  data.titleY = 25;
  data.subtitleFont = FREE_SANS_9;
  data.subtitleX = 0;
  data.subtitleY = 43;
  data.bodyFont = FREE_SANS_9;
  data.bodyX = 0;
  data.bodyY = 70;
  custom(data);
}

void template5(InputData& data) {
  data.title = textFits(data.title, 15);
  data.subtitle = textFits(data.subtitle, 25);
  data.body = textFits(data.body, 170);
  data.titleFont = FREE_SANS_BOLD_18;
  data.titleX = (display.width() / 2 - data.title.length() / 2 * 18) - 10;
  data.titleY = 25;
  data.subtitleFont = FREE_SANS_BOLD_9;
  data.subtitleX = (display.width() / 2 - data.subtitle.length() / 2 * 9) - 12;
  data.subtitleY = 45;
  data.bodyFont = FREE_SANS_9;
  data.bodyX = 0;
  data.bodyY = 72;
  custom(data);
}

void template6(InputData& data) {
  data.title = textFits(data.title, 15);
  data.subtitle = textFits(data.subtitle, 25);
  data.body = textFits(data.body, 80);
  data.titleFont = FREE_SANS_BOLD_18;
  data.titleX = (display.width() / 2 - data.title.length() / 2 * 18) - 10;
  data.titleY = 80;
  data.subtitleFont = FREE_SANS_BOLD_9;
  data.subtitleX = (display.width() / 2 - data.subtitle.length() / 2 * 9) - 12;
  data.subtitleY = 100;
  data.bodyFont = FREE_SANS_9;
  data.bodyX = 1;
  data.bodyY = 146;
  custom(data);
}

// Minimalist - No Icon
void template7(InputData& data) {
  if (data.backgroundImage == "") {
    data.backgroundImage = "oracle";
  }
  data.title = textFits(data.title, 22);
  data.titleFont = FREE_SANS_BOLD_12;
  data.titleX = 0;
  data.titleY = 20;
  data.subtitle = textFits(data.subtitle, 105);
  data.subtitleFont = FREE_SANS_9;
  data.subtitleX = display.width() / 2 - data.subtitle.length() / 2 * 9;
  data.subtitleY = display.height() - 15;
  custom(data);
}

// Minimalist - No Icon
void template8(InputData& data) {
  long bgColor = (data.backgroundColor == COLOR_WHITE) ? GxEPD_WHITE : GxEPD_BLACK;
  long txtColor = (data.backgroundColor == COLOR_WHITE) ? GxEPD_BLACK : GxEPD_WHITE;
  if (data.backgroundImage == "") {
    data.backgroundImage = "oracle";
  }
  display.setFullWindow();
  display.firstPage();
  do {
    if (data.backgroundImage.indexOf("http") == -1) {
      display.fillScreen(txtColor);
      drawBackground(0, 0, data.backgroundImage, bgColor);
    }
  } while (display.nextPage());
  if (data.backgroundImage.indexOf("http") > -1) {
    imageFromUrl(data.backgroundImage, 0, 0, data.fingerprint, false);
  }
}

// status
void template9(InputData& data) {
  long bgColor = (data.backgroundColor == COLOR_WHITE) ? GxEPD_WHITE : GxEPD_BLACK;
  long txtColor = (data.backgroundColor == COLOR_WHITE) ? GxEPD_BLACK : GxEPD_WHITE;
  display.setFullWindow();
  // display.setPartialWindow(0, 0, display.width(), display.height());
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(25, 80);
    display.println(textFits(data.title, 50));
    display.setFont(&FreeMono9pt7b);
    display.setCursor(25, 100);
    display.println(textFits(data.subtitle, 50));
  } while (display.nextPage());
}

// barcode template 43 max chars
void template10(InputData& data) {
  long bgColor = (data.backgroundColor == COLOR_WHITE) ? GxEPD_WHITE : GxEPD_BLACK;
  long txtColor = (data.backgroundColor == COLOR_WHITE) ? GxEPD_BLACK : GxEPD_WHITE;
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(bgColor);
    display.setTextColor(txtColor);
    display.setFont(&FreeSansBold18pt7b);
    display.setCursor(0, 30);
    display.println(textFits(data.title, 15));
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(1, 50);
    display.println(textFits(data.subtitle, 26));
    display.setFont(&FreeSans9pt7b);
    display.setCursor(0, 77);
    display.println(textFits(data.body, 65));
    String formattedBarcode = String("@" + data.barcode + "#");
    drawBarcode39(50, 110, display.width(), 50, 1.9, formattedBarcode);
    display.setFont(&FreeMono9pt7b);
    int barcodeX = display.width() / 2 - data.barcode.length() / 2 * 5;
    display.setCursor(12, 172);
    display.println(data.barcode);
  } while (display.nextPage());
}

void template11(InputData& data) {
  data.titleX = 126 - (data.title.length() / 2 * 12);
  data.titleY = 150;
  data.subtitleX = 126 - (data.subtitle.length() / 2 * 9);
  data.subtitleY = 170;
  data.iconX = 0;
  data.iconY = 0;
  if (!isLocalIcon(data.icon) && data.badge == "") {
    data.iconX = display.height() / 2 - 64;
    data.iconY = display.width() / 2 - 68;
  }
  data.title = textFits(data.title, 22);
  data.titleFont = FREE_SANS_BOLD_12;
  data.subtitle = textFits(data.subtitle, 22);
  data.subtitleFont = FREE_SANS_9;
  data.iconSize = 128;
  custom(data);
}

void custom(InputData& data) {
  long bgColor = (data.backgroundColor == COLOR_BLACK) ? GxEPD_BLACK : GxEPD_WHITE;
  long txtColor = (data.backgroundColor == COLOR_BLACK) ? GxEPD_WHITE : GxEPD_BLACK;
  display.setFullWindow();
  display.firstPage();
  do {
    if (data.backgroundImage != "" && data.backgroundImage.indexOf("http") == -1) {
      display.fillScreen(txtColor);
      drawBackground(0, 0, data.backgroundImage, bgColor);
    } else {
      display.fillScreen(bgColor);
    }
    display.setTextColor(txtColor);
    if (data.badge != "") {
      drawBadge(data.iconX, data.iconY, data.iconSize / 2, data.badge, bgColor);
    } else if (isLocalIcon(data.icon) && data.badge == "") {
      if (data.iconSize == 64) {
	drawIcon64(data.iconX, data.iconY, data.icon, txtColor);
      } else if (data.iconSize == 128) {
	drawIcon128(data.iconX, data.iconY, data.icon, txtColor);
      }
    }
    // title
    if (data.title != "") {
      if (data.titleFont == FREE_SANS_BOLD_12) {
	display.setFont(&FreeSansBold12pt7b);
      } else if (data.titleFont == FREE_SANS_BOLD_18) {
	display.setFont(&FreeSansBold18pt7b);
      }
      display.setCursor(data.titleX, data.titleY);
      display.println(data.title);
    }
    // subtitle
    if (data.subtitle != "") {
      if (data.subtitleFont == FREE_SANS_9) {
	display.setFont(&FreeSans9pt7b);
      } else if (data.subtitleFont == FREE_SANS_BOLD_9) {
	display.setFont(&FreeSansBold9pt7b);
      }
      display.setCursor(data.subtitleX, data.subtitleY);
      display.println(data.subtitle);
    }
    // body
    if (data.body != "") {
      if (data.bodyFont == FREE_SANS_9) {
	display.setFont(&FreeSans9pt7b);
      }
      display.setCursor(data.bodyX, data.bodyY);
      display.println(data.body);
    }
  } while (display.nextPage());
  if (!isLocalIcon(data.icon) && data.badge == "") {
    imageFromUrl(data.icon, data.iconX, data.iconY, data.fingerprint, false);
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
    display.drawInvertedBitmap(0, 0, oracle264, display.width(),
			       display.height(), GxEPD_BLACK);
  } while (display.nextPage());
  InputData data;
  data.title = String(PROJECT_NAME) + " " + String(PROJECT_VERSION);
  data.subtitle = PROJECT_AUTHOR;
  data.body = PROJECT_SITE;
  data.backgroundColor = "white";
  data.barcode = mac;
  template10(data);
}
