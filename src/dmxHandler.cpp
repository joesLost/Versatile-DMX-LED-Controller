#include "dmxHandler.h"
#include "ledMode.h"
#include "leds.h"
#include "logger.h"

byte data[DMX_PACKET_SIZE];
bool dmxIsConnected = false;
unsigned long lastUpdate = 0;
dmx_port_t dmxPort = 1;
dmx_config_t config = DMX_CONFIG_DEFAULT;
dmx_personality_t personalities[] = {
  {9, "9ch Tree"}
};

void processTreeDmx() {
  int a = getDmxAddress();
  if (a < 1 || a + 8 > 512) return;
  applyDmxTreeColors(&data[a], 300);
}

void dmxHandler(void *pvParameters) {
  while (true) {
    if (getLedMode() == MODE_TEST) {
      LEDTest();
      continue;
    }

    dmx_packet_t packet;
    if (dmx_receive(dmxPort, &packet, DMX_TIMEOUT_TICK)) {
      unsigned long now = millis();

      if (!packet.err) {
        if (!dmxIsConnected) {
          logPrintln("DMX is connected!");
          dmxIsConnected = true;
        }

        dmx_read(dmxPort, data, packet.size);

        if (now - lastUpdate > 1000) {
          logPrintf("Start code is 0x%02X and slot 1 is 0x%02X\n", data[0], data[1]);
          for (int i = 1; i <= 32; i++) logPrintf("DMX Channel %d: %d\n", i, data[i]);
          lastUpdate = now;
        }

        processTreeDmx();
      } else {
        logPrintln("A DMX error occurred.");
      }
    } else {
      if (dmxIsConnected) {
        logPrintln("No DMX packet received.");
        dmxIsConnected = false;
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void processDMXChannels() {
  processTreeDmx();
}