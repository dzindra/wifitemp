#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <TaskScheduler.h>
#include <OneWire.h>

#define PIN_TEMP D4
#define LOG_ENABLED 1

#define WIFI_AP_NAME "wifitemp-"

Scheduler scheduler;

String chipId = String(ESP.getChipId(), HEX);


