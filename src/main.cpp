#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <Time.h>
#include <TimeLib.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <Hash.h>

#include "pinout.h"
#include "ScreenController.h"
#include "WiFiController.h"
#include "TimeController.h"
#include "WebController.h"
#include "ConfigController.h"

// const char* ssid = "";
// const char* password = "";
#include "Secret.h"

const String hostname = "AgeClock";

WiFiController wifiController(hostname);
TimeController timeController;
ScreenController screenController;
WebController webController;
ConfigController configController;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();

  screenController.setup();
  wifiController.setupClient(ssid, password);
  webController.setup();

  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }
    Serial.printf("\n");
  }

  pinMode(D3, INPUT_PULLUP);
}

void loop() {
  screenController.update();
  timeController.update();
  screenController.updateClock();
}
