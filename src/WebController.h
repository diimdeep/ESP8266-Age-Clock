#include <ESPAsyncWebServer.h>

class WebController {
  public:
    WebController();
    void setup();

  private:
    void getWiFiEndpoints(AsyncWebServerRequest *request);
    void restart(AsyncWebServerRequest *request);
    void connectWiFi(AsyncWebServerRequest *request);
    void getStatus(AsyncWebServerRequest *request);
};
