#include <ESP8266WiFi.h>

class WiFiController
{
  public:
    WiFiController(const String hostname);
    String formatHostname(const String hostname);
    void setupClient(const char* ssid, const char* password);

  private:
    String _hostname;
    WiFiEventHandler _onGotIP;
};
