#include <Arduino.h>
#include "dmxHandler.h"
#include "leds.h"
#include "globals.h"

void serialHandler(void* pvParameters);


void setup() {
  Serial.begin(115200);
  Serial.println("Setup starting...");
  delay(1000); //Needed but I forgot why...

  pinMode(PUL_PIN_HR, OUTPUT);
  pinMode(PUL_PIN_MIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  // Initialize LEDs
  initializeLEDs();

  dmx_driver_install(dmxPort, &config, personalities, 1);
  dmx_set_pin(dmxPort, TX_PIN, RX_PIN, RTS_PIN);

  // Create tasks
  xTaskCreatePinnedToCore(
    dmxHandler, // Function to implement the task
    "DMX Task", // Name of the task
    10000,      // Stack size in words
    NULL,       // Task input parameter
    10,          // Priority of the task
    NULL,       // Task handle
    0)
  ;         // Core where the task should run

  // xTaskCreatePinnedToCore(
  //   motorControlTask, // Function to implement the task
  //   "Motor Control Task", // Name of the task
  //   10000,      // Stack size in words
  //   NULL,       // Task input parameter
  //   20,         // Priority of the task
  //   NULL,       // Task handle
  //   1           // Core where the task should run
  // );         

  //Enable Serial Handler for debugging only, There is some bug inside that will trigger the watchdog 🤷🏻‍♂️
  // xTaskCreate(
  //   serialHandler, // Function to implement the task
  //   "Serial Handler", // Name of the task
  //   10000,      // Stack size in words
  //   NULL,       // Task input parameter
  //   1,          // Priority of the task
  //   NULL
  // ); 
  Serial.println("Setup Complete");
}

void loop() {
 // Silence is golden
}

void serialHandler(void *pvParameters) {
  while (true) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      Serial.print("Received command: ");
      Serial.println(command);

      if (command == "LEDTest") {
        LEDTest();
      } else {
        Serial.println("Unknown command.");
      }
    }
    taskYIELD(); //Feed the watchdog
  }
}