#include "display.h"

#include "falk-pre-conf.h"

// THIS BLOCK NEEDED FOR THE DISPLAY
#include <SPI.h>
#include <U8g2lib.h>

//So you're here because the first few/last few lines of your display are missing, huh? You need to edit the display library, U8g2
// browse to:
//      <project path>/.pio/libdeps/ATmega328P/U8g2/src/clib/u8g2.h
// uncomment the following line (it'll be somewhere around line 71...)
// #define U8G2_16BIT

#ifdef __AVR_ATmega328P__
  //U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI u8g2(U8G2_R2, 10, 9, 8);
  U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI u8g2(U8G2_R2, SCREEN_CS, SCREEN_DC, SCREEN_RST);
#else
  U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2(U8G2_R2, SCREEN_CS, SCREEN_DC, SCREEN_RST);
#endif

uint32_t screenTimer = 0;

void Display::begin() {
//start the screen object
  u8g2.begin();
}

void Display::loop() {
  if ((screenTimer > 0) && (millis() > screenTimer + SCREEN_TIMEOUT)) {
    Display::dimScreen();
  }
}

//function that puts the input and volume on the screen
void Display::updateScreen() {
  char volume [5];
  if (sysSettings.volume > 0) {
    uint16_t v = round(((float)sysSettings.volume / (float)VOL_MAX) * 100);
    itoa(v, volume, 10);
    strcat(volume, "%");
  } else {
    String s = "MUTE";
    strcpy(volume, s.c_str());
  }

  u8g2.setFont(u8g2_font_fub25_tf);
  //vertically centers the text
  uint16_t y = 49;
  //left starting position for the volume (so it's right-aligned)
  uint16_t x = 256 - u8g2.getStrWidth(volume);

  u8g2.setFont(u8g2_font_crox1h_tr);
  uint16_t x2 = 256 - u8g2.getStrWidth("VOLUME");

  //atmega328p needs paging, but the ESP32 has enough RAM to dump the whole thing...
#ifdef __AVR_ATmega328P__
  u8g2.firstPage();
  do {
#else
  u8g2.clearBuffer();					// clear the internal memory
#endif

    u8g2.setFont(u8g2_font_fub25_tf);
    u8g2.drawStr(0, y, sysSettings.inputs[sysSettings.input - 1].name.c_str());
    u8g2.drawStr(x, y, volume);

    u8g2.setFont(u8g2_font_crox1h_tr);
    u8g2.drawStr(0, 9, "INPUT");
    u8g2.drawStr(x2, 9, "VOLUME");

  //atmega328p needs paging, but the ESP32 has enough RAM to dump the whole thing...
#ifdef __AVR_ATmega328P__
  } while ( u8g2.nextPage() );
#else
  //write data to screen
  u8g2.sendBuffer();
#endif

  //max brightness
  u8g2.setContrast(255);
  //dim the display in 10s
  if (sysSettings.dim == 1) {
    screenTimer = millis();
  }
}

//no wifi on the ATMega...
void Display::wifiScreen(const char* ssid) {
  u8g2.clearBuffer();					// clear the internal memory

  u8g2.setFont(u8g2_font_open_iconic_www_2x_t);
  u8g2.drawGlyph(120,19,0x51);

  u8g2.setFont(u8g2_font_crox1h_tr);
  String string = "Search for this WiFi network";
  const char* str = string.c_str();
  uint16_t x = 128 - (u8g2.getStrWidth(str) / 2);
  uint16_t y = 45;
  u8g2.drawStr(x, y, str);

  u8g2.setFont(u8g2_font_tenthinnerguys_tr);
  x = 128 - (u8g2.getStrWidth(ssid) / 2);
  y = 61;
  u8g2.drawStr(x, y, ssid);

  u8g2.setContrast(255);
  u8g2.sendBuffer();
}

//called after the timeout elapses, drops screen brightness
void Display::dimScreen() {
  u8g2.setContrast(1);
  screenTimer = 0;
}

void Display::test() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0,24,"Hello World!");
  } while ( u8g2.nextPage() );
  delay(5000);
}