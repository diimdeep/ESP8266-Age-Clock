#include <ESP8266WiFi.h>
#include <NtpClientLib.h>

class TimeController
{
  public:
    TimeController();
    void update();

  private:
    WiFiEventHandler _onGotIP;
    NTPSyncEvent_t _ntpEvent;
    bool _ntpEventTriggered;
};
