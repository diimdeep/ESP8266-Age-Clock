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

int screenW = 128;
int screenH = 64;
int clockCenterX = screenW/2;
int clockCenterY = ((screenH-16)/2)+16;   // top yellow part is 16 px height
int clockRadius = 23;


// utility function for digital clock display: prints leading 0
String twoDigits(int digits){
  if(digits < 10) {
    String i = '0'+String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {

}

#include <Chronos.h>
#include <Time.h>
#include <avr_f64.h>

int update_per_ms = 70;

Chronos::DateTime dob = Chronos::DateTime(1989, 9, 11, 10, 0, 0);
float seconds_in_year = 31556900;

uint32_t last_age_in_seconds = 0;
float64_t last_age = 0;
float64_t sec_in_year = f_long_to_float64(31556900);
float64_t diff_per_second = f_div(f_long_to_float64(1), f_long_to_float64(31556900));
float32_t fps_for_ms = 1000.0/update_per_ms;
float64_t diff_per_update_at_rate = f_div(diff_per_second, f_sd(fps_for_ms));

uint32_t last_update = 0;

void ScreenController::updateClock() {
  if (millis() < last_update + update_per_ms)
    return;

  Chronos::Span::Delta age = Chronos::DateTime::now() - dob;
  const Chronos::EpochTime age_in_seconds = age.totalSeconds();

  if (last_age_in_seconds == age_in_seconds) {
    last_age = f_add(last_age, diff_per_update_at_rate);
    last_update = millis();
  }
  else {
    last_age_in_seconds = age_in_seconds;
    float64_t sec_dur3 = f_long_to_float64(age_in_seconds);
    last_age = f_div(sec_dur3, sec_in_year);
    last_update = millis();
  }
}

int remainingTimeBudget = 0;

void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String timenow = f_to_string(last_age, 13,10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(0 + x , 11 + y, timenow);
  display->drawString(0 + x , 40 + y, String(remainingTimeBudget));
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
