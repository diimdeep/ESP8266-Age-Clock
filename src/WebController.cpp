#include "WebController.h"
#include <ESPAsyncWebServer.h>

AsyncWebServer _server(80);
WiFiEventHandler _onGotIP;

WebController::WebController() {
}

void WebController::setup() {
  _server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  
  _onGotIP = WiFi.onStationModeGotIP([&](const WiFiEventStationModeGotIP &ipInfo)
    {
      _server.begin();
    });
}
