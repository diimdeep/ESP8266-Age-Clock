#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <TimeLib.h>

#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <Hash.h>

#include "Screen.h"

const String hostname = "AgeClock";


#define TRIGGER_PIN 3

AsyncWebServer server(80);
DNSServer dns;
AsyncWiFiManager wifiManager(&server,&dns);

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(TRIGGER_PIN, INPUT);

  unsigned long secsSinceStart = millis();
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSinceStart - seventyYears * SECS_PER_HOUR;
  setTime(epoch);
  setupScreen();

  wifiManager.startConfigPortalModeless("OnDemandAP", NULL);
}

void loop() {
  updateScreen();

  wifiManager.loop();
}
