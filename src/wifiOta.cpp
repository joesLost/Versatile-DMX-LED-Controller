#include "wifiOta.h"
#include "logger.h"
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Preferences.h>
#include <WiFiServer.h>
#include <WiFiClient.h>

static WiFiServer telnetServer(23);
static WiFiClient telnetClient;
static Preferences prefs;
static String hostname;
static bool otaStarted = false;
static unsigned long lastWiFiAttempt = 0;
static const char* ssid = "Joseph-travel";
static const char* password = "F4NQHDM49Q";

#define LOG_PRINT(x) logPrint(x)
#define LOG_PRINTLN(x) logPrintln(x)

WiFiClient* getTelnetClient() {
  return &telnetClient;
}

String getHostname() {
  return hostname;
}

bool setHostname(const String& newHostname) {
  String s = newHostname;
  s.trim();
  if (!s.length() || s.length() > 15) return false;
  if (!prefs.putString("hostname", s)) return false;
  hostname = s;
  WiFi.setHostname(hostname.c_str());
  otaStarted = false;
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  return true;
}

void logBoth(const String& s) {
  Serial.println(s);
  if (telnetClient && telnetClient.connected()) telnetClient.println(s);
}

void initWiFiOta() {
  prefs.begin("device", false);
  hostname = prefs.isKey("hostname") ? prefs.getString("hostname") : "dmx-led-ctlr";
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(ssid, password);
}

void otaTask(void *pvParameters) {
  while (true) {
    if (otaStarted && WiFi.status() == WL_CONNECTED) ArduinoOTA.handle();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void wifiTask(void *pvParameters) {
  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      if (!otaStarted) {
        LOG_PRINTLN("");
        LOG_PRINT("WiFi connected, IP: ");
        LOG_PRINTLN(WiFi.localIP());
        ArduinoOTA.setHostname(hostname.c_str());
        ArduinoOTA.begin();
        otaStarted = true;
        LOG_PRINTLN("OTA Ready");
        telnetServer.begin();
        telnetServer.setNoDelay(true);
      }
    } else {
      if (millis() - lastWiFiAttempt >= 10000) {
        LOG_PRINTLN("WiFi not connected, retrying...");
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        lastWiFiAttempt = millis();
        otaStarted = false;
      }
    }
    if (telnetServer.hasClient()) {
        if (!telnetClient || !telnetClient.connected()) {
            telnetClient = telnetServer.available();
            setLogClient(&telnetClient);
            logPrintln("Telnet connected");
        } else {
            telnetServer.available().stop();
        }
    }
    // if (telnetClient && telnetClient.connected()) telnetClient.println("alive");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}