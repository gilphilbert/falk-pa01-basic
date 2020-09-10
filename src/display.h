#ifndef FALK_DISP
#define FALK_DISP

class Display {
  public:
    void begin();
    void loop();
    void updateScreen(unsigned short input, unsigned short vol);
    void updateScreen();
    void wifiScreen(const char* ssid);
    void dimScreen();
};

#endif