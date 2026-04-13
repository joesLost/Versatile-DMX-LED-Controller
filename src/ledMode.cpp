#include <Preferences.h>
#include "ledMode.h"
#include "leds.h"

static Preferences prefs;
static uint16_t dmxAddress = 1;
static uint8_t ledMode = MODE_DMX_9CH;

void initLedMode() {
  prefs.begin("device", false);
  dmxAddress = prefs.getUShort("dmxAddr", 1);
  ledMode = prefs.getUChar("mode", MODE_DMX_9CH);
  if (dmxAddress < 1 || dmxAddress > 512) dmxAddress = 1;
  if (ledMode < MODE_DMX_9CH || ledMode > MODE_TEST) ledMode = MODE_DMX_9CH;
}

uint16_t getDmxAddress() {
  return dmxAddress;
}

bool setDmxAddress(uint16_t addr) {
  if (addr < 1 || addr > 512) return false;
  dmxAddress = addr;
  prefs.putUShort("dmxAddr", dmxAddress);
  return true;
}

uint8_t getLedMode() {
  return ledMode;
}

bool setLedMode(uint8_t mode) {
  if (mode < MODE_DMX_9CH || mode > MODE_TEST) return false;
  ledMode = mode;
  prefs.putUChar("mode", ledMode);
  return true;
}


void applyDmxTreeColors(const uint8_t* data, int ledCount) {
  if (ledCount < 1) return;
  if (ledCount > 300) ledCount = 300;

  CRGB c1 = CRGB(data[0], data[1], data[2]);
  CRGB c2 = CRGB(data[3], data[4], data[5]);
  CRGB c3 = CRGB(data[6], data[7], data[8]);

  for (int i = 0; i < ledCount; i++) {
    switch ((i / 2) % 3) {
      case 0: leds[i] = c1; break;
      case 1: leds[i] = c2; break;
      case 2: leds[i] = c3; break;
    }
  }

  FastLED.show();
}