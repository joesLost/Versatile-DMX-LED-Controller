#ifndef DMXHANDLER_H
#define DMXHANDLER_H

#include <Arduino.h>
#include "esp_dmx.h"

#define TX_PIN 17
#define RX_PIN 16
#define RTS_PIN 4
#define DMX_PACKET_SIZE 512
#define DMX_TIMEOUT_TICK 1000


extern byte data[DMX_PACKET_SIZE];
extern bool dmxIsConnected;
extern unsigned long lastUpdate;
extern dmx_port_t dmxPort;
extern dmx_config_t config;
extern dmx_personality_t personalities[];

// Function declarations
void dmxHandler(void* pvParameters);
void processDMXChannels();

#endif // DMXHANDLER_H


// LED Controller DMX Control Profile Overview


// Channel 1-4: RGB LED Control

    // Channel 1: LED Intensity Master 0-255: Controls the overall brightness of all LEDs (0 for off, 255 for max brightness).

    // Channel 2 (Red): 0-255 Red Led Intensity

    // Channel 3 (Green): 0-255 Green Led Intensity

    // Channel 4 (Blue): 0-255 Blue Led Intensity]

    // Channel 5 (mode): 0-255 Blue Led Intensity



