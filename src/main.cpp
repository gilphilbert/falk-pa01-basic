#include <Arduino.h>
#include <Wire.h>

#include <EEPROMex.h>

#include "falk-pre-conf.h"

short butState = LOW;

unsigned long commitTimer = 0;

int settingsAddress;

void restoreSettings() {
  EEPROM.setMaxAllowedWrites(5);
  int savedAddress = EEPROM.getAddress(sizeof(int));
  settingsAddress = EEPROM.getAddress(sizeof(DeviceSettings));

  int yn = EEPROM.readInt(savedAddress);

  if(yn > 0) {
    EEPROM.readBlock<DeviceSettings>(settingsAddress, sysSettings);
  } else {
    for(int i = INP_MIN; i <= INP_MAX; i++) {
      sysSettings.inputs[i - 1].enabled = true;
      sysSettings.inputs[i - 1].icon = "";
      sysSettings.inputs[i - 1].name = String("Input ") + String(i);;
    }
    EEPROM.writeBlock<DeviceSettings>(settingsAddress, sysSettings);
    EEPROM.writeInt(savedAddress, 1);
  }
}

void writeSettings() { 
  EEPROM.updateBlock<DeviceSettings>(settingsAddress, sysSettings);
}

void readInput() {
  short newState = digitalRead(INPUT_BUTTON);
  if (newState != butState) {
    if (newState == HIGH) {
      sysSettings.input += 1;
      if (sysSettings.input > INP_MAX) {
        sysSettings.input = INP_MIN;
      }
      relays.setInput(sysSettings.input);
      display.updateScreen();
    }
    butState = newState;
  }
}

void readVolume() {
  int16_t vol = analogRead(VOL_POT);
  vol = 255 - map(vol, 0, 1023, 0, 255);
  if (vol != sysSettings.volume) {
    sysSettings.volume = vol;
    relays.setVolume(sysSettings.volume);
    display.updateScreen();
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

  restoreSettings();
  relays.setInput(sysSettings.input);

  display.begin();
  readVolume();
  //in the event that volume really is zero, write to the screen
  if (sysSettings.volume == 0) {
    display.updateScreen();
  }
}

void loop() {
  readVolume();
  readInput();

  if (commitTimer > 0 and commitTimer > millis() + COMMIT_TIMEOUT) {
    writeSettings();
    commitTimer = 0; 
  }
}