#ifndef LEDS_H

#include <FastLED.h>
#include <Arduino.h>

#define NUM_LEDS 1000
#define BRIGHTNESS 80
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

#define LED1_PIN 13
#define LED2_PIN 14
#define LED3_PIN 16
#define LED4_PIN 17

#define PWM_FREQ 5000
#define PWM_RES 8

extern bool ENABLE_LED5;
extern bool ENABLE_LED6;
extern bool ENABLE_LED7;
extern bool ENABLE_LED8;

extern CRGB leds[NUM_LEDS];

struct RgbOut {
  uint8_t rPin, gPin, bPin;
  uint8_t rCh, gCh, bCh;
  bool* enabled;
  const char* name;
};

extern RgbOut pwmOuts[];

void initializeLEDs();
void setLEDColor(int intensity, int red, int green, int blue);
void showPattern(const char* name, const CRGB& c1, const CRGB& c2, const CRGB& c3, int ms);
void setPwm(const RgbOut& o, uint8_t r, uint8_t g, uint8_t b);
void showPwmColor(const char* name, uint8_t r, uint8_t g, uint8_t b, int ms);
void setupPwm();
void LEDTest();

#endif