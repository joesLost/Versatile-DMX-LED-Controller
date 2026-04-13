#pragma once
#include <Arduino.h>

enum LedMode {
  MODE_DMX_9CH = 1,
  MODE_TEST = 2
};

void initLedMode();
uint16_t getDmxAddress();
bool setDmxAddress(uint16_t addr);
uint8_t getLedMode();
bool setLedMode(uint8_t mode);
void runLedMode();
void applyDmxTreeColors(const uint8_t* data, int ledCount);