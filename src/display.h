#ifndef FALK_DISP
#define FALK_DISP

class Display {
  public:
    void begin();
    void loop();
    void updateScreen();
    void updateScreen(uint8_t input, uint8_t vol);
    void wifiScreen(const char* ssid);
    void dimScreen();
};

#endif