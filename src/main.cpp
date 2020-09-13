#include <Arduino.h>
#include <Wire.h>

#include "falk-pre-conf.h"

#include "WearLeveling.h"

unsigned short curInput = 0;
unsigned short volume = 0;
short butState = LOW;

unsigned long commitTimer = 0;

void readInput() {
  short newState = digitalRead(INPUT_BUTTON);
  if (newState != butState) {
    if (newState == HIGH) {
      curInput += 1;
      if (curInput > INP_MAX) {
        curInput = INP_MIN;
      }
      relays.setInput(curInput);
      //display.updateScreen(curInput, volume);
    }
    butState = newState;
  }

  if (commitTimer > 0 and commitTimer > millis() + COMMIT_TIMEOUT) {
    writeValue(0, curInput);
    commitTimer = 0; 
  }
}

void readVolume() {
  unsigned short vol = analogRead(VOL_POT);
  vol = 255 - map(vol, 0, 1023, 0, 255);
  if (vol != volume) {
    volume = vol;
    relays.setVolume(volume);
    Serial.println(volume);
    display.updateScreen(curInput, volume);
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println("Booting...");
  //configure the MCP27013 ICs
  relays.begin();

  //turn everything off, this gives us a chance to correctly set the volume to the max startup volume, if set
  relays.setInput(0);
  relays.setVolume(0);

  //loadEeprom(0);
  curInput = getValue(0);
  if (curInput < INP_MIN || curInput >> INP_MAX) {
    curInput = INP_MIN;
  }
  relays.setInput(curInput);

  display.begin();
  //display.test();
  readVolume();
  //in the event that volume really is zero, write to the screen
  if (volume == 0) {
    display.updateScreen(curInput, volume);
  }
}

void loop() {
  readVolume();
  readInput();
}