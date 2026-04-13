#include "leds.h"

bool ENABLE_LED5 = true;
bool ENABLE_LED6 = true;
bool ENABLE_LED7 = true;
bool ENABLE_LED8 = true;

CRGB leds[NUM_LEDS];

RgbOut pwmOuts[] = {
  { 18, 19, 21, 0, 1, 2, &ENABLE_LED5, "LED5" },
  { 22, 23, 25, 3, 4, 5, &ENABLE_LED6, "LED6" },
  { 26, 27, 32, 6, 7, 8, &ENABLE_LED7, "LED7" },
  { 4, 5, 33, 9, 10, 11, &ENABLE_LED8, "LED8" }
};

void initializeLEDs() {
  FastLED.addLeds<LED_TYPE, LED1_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED2_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED3_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED4_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void setLEDColor(int intensity, int red, int green, int blue) {
  FastLED.setBrightness(intensity);
  fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
  FastLED.show();
}

void showPattern(const char* name, const CRGB& c1, const CRGB& c2, const CRGB& c3, int ms) {
  for (int i = 0; i < NUM_LEDS; i += 3) {
    leds[i] = c1;
    if (i + 1 < NUM_LEDS) leds[i + 1] = c2;
    if (i + 2 < NUM_LEDS) leds[i + 2] = c3;
  }
  FastLED.show();
  delay(ms);
}

void setPwm(const RgbOut& o, uint8_t r, uint8_t g, uint8_t b) {
  if (!*o.enabled) r = g = b = 0;
  ledcWrite(o.rCh, r);
  ledcWrite(o.gCh, g);
  ledcWrite(o.bCh, b);
}

void showPwmColor(const char* name, uint8_t r, uint8_t g, uint8_t b, int ms) {
  for (auto& o : pwmOuts) setPwm(o, r, g, b);
  delay(ms);
}

void setupPwm() {
  for (auto& o : pwmOuts) {
    ledcSetup(o.rCh, PWM_FREQ, PWM_RES);
    ledcAttachPin(o.rPin, o.rCh);
    ledcSetup(o.gCh, PWM_FREQ, PWM_RES);
    ledcAttachPin(o.gPin, o.gCh);
    ledcSetup(o.bCh, PWM_FREQ, PWM_RES);
    ledcAttachPin(o.bPin, o.bCh);
  }
}

void LEDTest() {
  initializeLEDs();
  setupPwm();

  showPattern("RGB", CRGB::Red, CRGB::Green, CRGB::Blue, 2000);
  showPattern("White", CRGB::White, CRGB::White, CRGB::White, 2000);

  showPwmColor("Red", 255, 0, 0, 2000);
  showPwmColor("Green", 0, 255, 0, 2000);
  showPwmColor("Blue", 0, 0, 255, 2000);
}