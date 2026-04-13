#include <Arduino.h>
#include "globals.h"
#include "dmxHandler.h"
#include "wifiOta.h"
#include "logger.h"
#include "ledMode.h"
#include "leds.h"


void serialHandler(void* pvParameters);
void handleCommand(String command);
void pollCommandSource(Stream& in);


void setup() {
  Serial.begin(115200);
  logPrintln("Setup starting...");
  delay(1000);


  initWiFiOta();
  initLedMode();
  initializeLEDs();

  dmx_driver_install(dmxPort, &config, personalities, 1);
  dmx_set_pin(dmxPort, TX_PIN, RX_PIN, RTS_PIN);

  xTaskCreatePinnedToCore(
    dmxHandler,
    "DMX Task",
    10000,
    NULL,
    10,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    otaTask,
    "OTA Task",
    5000,
    NULL,
    1,
    NULL,
    1
  );

  xTaskCreatePinnedToCore(
    wifiTask,
    "WiFi Task",
    5000,
    NULL,
    1,
    NULL,
    1
  );

  xTaskCreatePinnedToCore(
    serialHandler,
    "Serial Handler",
    10000,
    NULL,
    1,
    NULL,
    1
  );

  logPrintln("Setup Complete");
}

void loop() {
  //Silience is golden, the tasks are doing the work!
}

void serialHandler(void *pvParameters) {
  while (true) {
    pollCommandSource(Serial);

    WiFiClient* c = getTelnetClient();
    if (c && c->connected()) pollCommandSource(*c);

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void pollCommandSource(Stream& in) {
  if (!in.available()) return;

  String command = in.readStringUntil('\n');
  command.trim();
  if (!command.length()) return;

  handleCommand(command);
}

void handleCommand(String command) {
  logPrintln("CMD: " + command);

  if (command == "hostname") {
    logPrintln("Hostname: " + getHostname());
  } else if (command.startsWith("hostname ")) {
    String s = command.substring(9);
    s.trim();
    if (setHostname(s)) logPrintln("Hostname set to: " + getHostname());
    else logPrintln("Invalid hostname");
  } else if (command == "LEDTest") {
    LEDTest();
  } else if (command == "mode") {
    logPrintf("Mode: %u\n", getLedMode());
  } else if (command.startsWith("mode ")) {
    int m = command.substring(5).toInt();
    if (setLedMode(m)) logPrintf("Mode set to: %u\n", getLedMode());
    else logPrintln("Invalid mode");
  } else if (command == "dmxaddr") {
    logPrintf("DMX Address: %u\n", getDmxAddress());
  } else if (command.startsWith("dmxaddr ")) {
    int a = command.substring(8).toInt();
    if (setDmxAddress(a)) logPrintf("DMX Address set to: %u\n", getDmxAddress());
    else logPrintln("Invalid DMX address");
  }else {
    logPrintln("Unknown command.");
  }
}