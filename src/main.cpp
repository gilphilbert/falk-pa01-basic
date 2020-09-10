#include <Arduino.h>
#include <Wire.h>

#include "falk-pre-conf.h"

#include "WearLeveling.h"

uint8_t curInput = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Booting...");
  //configure the MCP27013 ICs
  relays.begin();

  //turn everything off, this gives us a chance to correctly set the volume to the max startup volume, if set
  relays.setInput(0);
  relays.setVolume(0);

  loadEeprom(0);
  curInput = getValue(0);
  if (curInput < INP_MIN || curInput >> INP_MAX) {
    curInput = INP_MIN;
  }
  relays.setInput(curInput);

  display.begin();
  display.updateScreen();
}

uint8_t volume = 0;
uint8_t butState = LOW;

void loop() {
  int vol = analogRead(VOL_POT);
  vol = map(vol, 0, 1023, 0, 255);
  if (vol != volume) {
    volume = vol;
    relays.setVolume(volume);
    display.updateScreen(curInput, volume);
  }

  uint8_t newState = digitalRead(INPUT_BUTTON);
  if (newState != butState) {
    if (newState == HIGH) {
      curInput += 1;
      if (curInput > INP_MAX) {
        curInput = INP_MIN;
      }
      relays.setInput(curInput);
      display.updateScreen(curInput, volume);
    }
    butState = newState;
  }
}