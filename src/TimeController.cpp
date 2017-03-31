#include "TimeController.h"
#include <ESP8266WiFi.h>
#include <TimeLib.h>

TimeController::TimeController() {
  NTP.onNTPSyncEvent([&](NTPSyncEvent_t event) {
      _ntpEvent = event;
      _ntpEventTriggered = true;
  });

  _onGotIP = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP &ipInfo)
  {
    NTP.begin("pool.ntp.org", 3, false);
    NTP.setInterval(63);
  });
}

void TimeController::update() {
  if(_ntpEventTriggered) {
      if (_ntpEvent) {
  		    Serial.print("Time Sync error: ");
  		if (_ntpEvent == noResponse)
  			Serial.println("NTP server not reachable");
  		else if (_ntpEvent == invalidAddress)
  			Serial.println("Invalid NTP server address");
    	}
    	else {
    		Serial.print("Got NTP time: ");
    		Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
    	}

    _ntpEventTriggered = false;
  }
}
