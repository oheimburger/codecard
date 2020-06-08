// -*- C++ -*-
/*
  data.h
*/

struct InputData {
  String name;
  String title;
  String subtitle;
  String body;
  String icon;
  String badge;
  String fingerprint;
  String barcode;
  String titleFont;
  int titleX;
  int titleY;
  String subtitleFont;
  int subtitleX;
  int subtitleY;
  String bodyFont;
  int bodyX;
  int bodyY;
  int iconSize;
  int iconX;
  int iconY;
  String backgroundImage;
  String backgroundColor;
  InputData() : titleX(-1), titleY(-1), subtitleX(-1), subtitleY(-1), bodyX(-1), bodyY(-1), iconX(-1), iconY(-1) {
    // Empty
  }
};
