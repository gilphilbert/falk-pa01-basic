#ifndef INITIAL_SETUP
#define INITIAL_SETUP

#define __ATMEGA328p__

#include "Arduino.h"

// ====== MINIMUM AND MAXIMUM VOLUME ====== //

#define VOL_MIN                 0
#define VOL_MAX                 255

// ====== HOW MANY INPUTS ====== //

#define INP_MIN                 1
#define INP_MAX                 4

// ====== DISPLAY BEHAVIOR ====== //

#define SCREEN_TIMEOUT          10000

// ====== BUTTON SETTINGS ====== //

#define BUTTON_DEBOUNCE_DELAY   50
#define BUTTON_LONG_PRESS       2000

// ====== FLASH WEAR OUT PREVENTION ====== //

#define COMMIT_TIMEOUT          400

// ====== DEFINE PINS ====== //

#define SCREEN_CS               10//16
#define SCREEN_DC               9//15
#define SCREEN_RST              8//14

#define INPUT_BUTTON            6//12
#define VOL_POT                 A0//23

// ====== SETTINGS (INCL. DEFAULTS) ====== //
typedef struct {
  String name;
  String icon = "disc";
  int enabled = 1;
} DeviceInput;

typedef struct {
  int16_t volume = 26;
  int16_t input = 1;
  DeviceInput inputs [INP_MAX];
  uint16_t saved = 0;
  int dim = 1;
  int icon = 0;
  int maxVol = VOL_MAX;
  int maxStartVol = -1;
} DeviceSettings;

extern DeviceSettings sysSettings;

// FIRMWARE VERSION (THIS SW)
extern String fw_version;

#include "relays.h"
extern RelayController relays;

#include "display.h"
extern Display display;

extern int muteState;

#endif