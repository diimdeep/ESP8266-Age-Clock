#include "ScreenController.h"

#include <TimeLib.h>

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SH1106.h" // alias for `#include "SSD1306Wire.h"`
// or #include "SH1106.h" alis for `#include "SH1106Wire.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images
#include "images.h"

// Use the corresponding display class:

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
SH1106  display(0x3c, D1, D2);
// SH1106 display(0x3c, D3, D5);

OLEDDisplayUi ui ( &display );

void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {

}

#include <Chronos.h>
#include <Float64.h>

uint32_t update_per_ms = 70;

Chronos::DateTime dob = Chronos::DateTime(1989, 9, 11, 10, 0, 0);
uint32_t seconds_in_year = 31556900;

uint32_t last_age_in_seconds = 0;
f64 last_age = 0;
f64 sec_in_year = seconds_in_year;
f64 diff_per_second = f64(1)/sec_in_year;
f64 fps_for_ms = f64(1000)/f64(update_per_ms);
f64 diff_per_update_at_rate = diff_per_second/fps_for_ms;

uint32_t last_update = 0;

void ScreenController::updateClock() {
  if (millis() < last_update + update_per_ms)
    return;

  Chronos::Span::Delta age = Chronos::DateTime::now() - dob;
  const Chronos::EpochTime age_in_seconds = age.totalSeconds();

  if (last_age_in_seconds == age_in_seconds) {
    last_age = last_age + diff_per_update_at_rate;
    last_update = millis();
  }
  else {
    last_age_in_seconds = age_in_seconds;
    f64 sec_dur3 = f64(age_in_seconds);
    last_age = sec_dur3/sec_in_year;
    last_update = millis();
  }
}

int remainingTimeBudget = 0;

#include "fonts.h"

void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String timenow = last_age.toString(9);
  String fixed = timenow.substring(0, 2);
  String mantiss = timenow.substring(2, timenow.length());

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Condensed_20);
  display->drawString(0 + x , 11 + y, fixed);
  display->setFont(Roboto_Condensed_16);
  display->drawString(22 + x , 13 + y, String(mantiss));
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { digitalClockFrame };

// how many frames are there?
int frameCount = 1;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { clockOverlay };
int overlaysCount = 1;


ScreenController::ScreenController(){

}

void ScreenController::setup() {
  // The ESP is capable of rendering 60fps in 80Mhz mode
  // but that won't give you much time for anything else
  // run it in 160Mhz mode or just set it to 30 fps
  //ui.setTargetFPS(23);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  ui.disableAutoTransition();
  ui.disableIndicator();
  //display.flipScreenVertically();

}



void ScreenController::update() {
  remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    //delay(remainingTimeBudget);
  }

}
