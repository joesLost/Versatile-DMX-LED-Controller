#pragma once
#include <Arduino.h>
#include <WiFiClient.h>

void initWiFiOta();
void wifiTask(void *pvParameters);
void otaTask(void *pvParameters);
String getHostname();
bool setHostname(const String& newHostname);
WiFiClient* getTelnetClient();