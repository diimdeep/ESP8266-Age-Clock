#include <ESP8266WiFi.h>
#include "WiFiController.h"

WiFiController::WiFiController(const String hostname) {
  _hostname = formatHostname(hostname);
}

String WiFiController::formatHostname(const String hostname) {
  // From: https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-goodnight-thing-sleep-mode
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  // String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                //  String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  return hostname + macID;
}

void WiFiController::setupClient(const char* ssid, const char* password) {
  WiFi.printDiag(Serial);
  
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.hostname(_hostname);
  WiFi.begin(ssid, password);

  _onGotIP = WiFi.onStationModeGotIP([&](const WiFiEventStationModeGotIP &ipInfo)
    {
      Serial.printf("Connected.\n");
      Serial.print("http://");
      Serial.print(WiFi.localIP());
      Serial.print("  http://");
      Serial.print(_hostname);
      Serial.println();
      Serial.println();
    });
}
