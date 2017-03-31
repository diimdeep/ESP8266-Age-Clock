#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <TimeLib.h>

#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <Hash.h>

#include "ScreenController.h"
#include "WiFiController.h"
#include "TimeController.h"

// const char* ssid = "";
// const char* password = "";
#include "Secret.h"

const String hostname = "AgeClock";

const bool useDHCP = true;

#define TRIGGER_PIN 3

WiFiController wifiController(hostname);
TimeController timeController;
ScreenController screenController;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(TRIGGER_PIN, INPUT);

  screenController.setup();
  wifiController.setupClient(ssid, password);
}

void loop() {
  timeController.update();
  screenController.update();
}
