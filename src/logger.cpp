#include "logger.h"

static WiFiClient* logClient = nullptr;

void setLogClient(WiFiClient* client) {
  logClient = client;
}

size_t logPrint(const String& s) {
  size_t n = Serial.print(s);
  if (logClient && *logClient && logClient->connected()) logClient->print(s);
  return n;
}

size_t logPrintln(const String& s) {
  size_t n = Serial.println(s);
  if (logClient && *logClient && logClient->connected()) logClient->println(s);
  return n;
}

size_t logPrint(const char* s) {
  size_t n = Serial.print(s);
  if (logClient && *logClient && logClient->connected()) logClient->print(s);
  return n;
}

size_t logPrintln(const char* s) {
  size_t n = Serial.println(s);
  if (logClient && *logClient && logClient->connected()) logClient->println(s);
  return n;
}

size_t logPrint(IPAddress ip) {
  String s = ip.toString();
  return logPrint(s);
}

size_t logPrintln(IPAddress ip) {
  String s = ip.toString();
  return logPrintln(s);
}
#include <stdarg.h>

size_t logPrintf(const char* fmt, ...) {
  char buf[256];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  size_t n = Serial.print(buf);
  if (logClient && *logClient && logClient->connected()) logClient->print(buf);
  return n;
}