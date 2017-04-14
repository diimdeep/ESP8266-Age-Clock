#include "WebController.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
//#include <user_interface.h>

AsyncWebServer _server(80);
WiFiEventHandler _onGotIP;

WebController::WebController() {
}

void WebController::setup() {
  _server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
_server.on("/status", HTTP_GET, [this](AsyncWebServerRequest *request){ this->getStatus(request); });
  _server.on("/wifi_endpoints", HTTP_GET, [this](AsyncWebServerRequest *request){ this->getWiFiEndpoints(request); });
  _server.on("/wifi_connect", HTTP_POST, [this](AsyncWebServerRequest *request){ this->connectWiFi(request); });
  _server.on("/restart", HTTP_POST, [this](AsyncWebServerRequest *request){ this->restart(request); });
  _server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  _server.onNotFound([](AsyncWebServerRequest *request){
    Serial.printf("NOT_FOUND: ");
    if(request->method() == HTTP_GET)
      Serial.printf("GET");
    else if(request->method() == HTTP_POST)
      Serial.printf("POST");
    else if(request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if(request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if(request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if(request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if(request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if(request->contentLength()){
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }

    int params = request->params();
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

    request->send(404);
  });

  _onGotIP = WiFi.onStationModeGotIP([&](const WiFiEventStationModeGotIP &ipInfo)
    {
      _server.begin();
    });
}


void WebController::getStatus(AsyncWebServerRequest *request) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["mode"] = WiFi.getMode();
  root["phy"] = WiFi.getPhyMode();
  root["phy"] = WiFi.channel();
  // root["station_ap_id"] = wifi_station_get_current_ap_id();
  // root["station_connect_status"] = wifi_station_get_connect_status();
  // struct station_config conf;
  // wifi_station_get_config(&conf);
  //root["station_ssid"] =

  String response;
  root.printTo(response);
  request->send(200, "text/json", response);
}

void WebController::restart(AsyncWebServerRequest *request) {
  request->send(200, "text/html", "Will reset in 2 seconds.");
  delay(5000);
  ESP.reset();
  delay(2000);
}

void WebController::connectWiFi(AsyncWebServerRequest *request) {
  String ssid = request->arg("ssid");
  String password = request->arg("password");
  String mode = request->arg("mode");
  WiFi.mode((WiFiMode)mode.toInt());
}

void WebController::getWiFiEndpoints(AsyncWebServerRequest *request) {
  WiFi.scanNetworksAsync([request](int count){
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["mode"] = WiFi.getMode();
    JsonArray &networks = root.createNestedArray("networks");

    for (size_t i = 0; i < count; i++) {
      JsonObject &network = jsonBuffer.createObject();

      String ssid;
      uint8_t encryptionType;
      int32_t RSSI;
      uint8_t* BSSID;
      int32_t channel;
      bool isHidden;

      WiFi.getNetworkInfo(i, ssid, encryptionType, RSSI, BSSID, channel, isHidden);

      network["SSID"] = ssid;
      network["encryptionType"] = encryptionType;
      network["RSSI"] = encryptionType;
      network["BSSID"] = encryptionType;
      network["channel"] = channel;
      network["isHidden"] = isHidden;

      networks.add(network);
    }

    String response;
    root.printTo(response);
    request->send(200, "text/json", response);
  }, true);
}
