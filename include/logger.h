#pragma once
#include <Arduino.h>
#include <WiFiClient.h>

void setLogClient(WiFiClient* client);
size_t logPrint(const String& s);
size_t logPrintln(const String& s);
size_t logPrint(const char* s);
size_t logPrintln(const char* s);
size_t logPrint(IPAddress ip);
size_t logPrintln(IPAddress ip);
size_t logPrintf(const char* fmt, ...);