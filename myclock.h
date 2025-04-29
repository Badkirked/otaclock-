
// ==============================  MyClock.h  ==============================
#pragma once
// ACME – 4‑digit TM1637 clock helper (ESP8266‑ready)
// -------------------------------------------------------------------------
#include <Arduino.h>
#include <TM1637Display.h>

class MyClock {
public:
    explicit MyClock(TM1637Display *display);

    // ntpServer defaults to pool.ntp.org, offset/dst are *seconds*
    void begin(const char *ntpServer = "pool.ntp.org",
               long        gmtOffsetSec = 0,
               int         daylightOffsetSec = 0);

    bool update();                         // pull fresh time from SNTP
    void showTime(bool colonVisible);      // draw HH:MM (12‑h) on TM1637
    void printToSerial();                  // debug helper

private:
    TM1637Display *_display;
};



