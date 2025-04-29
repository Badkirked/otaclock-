
#include "ScrollerMessages.h"

static bool           restoreClock = false;
static unsigned long  lastStart    = 0;
const  uint16_t       SCROLL_MIN_MS = 1500;   // debounce / avoid flicker

// ------------------------------------------------------------------
// helpers
// ------------------------------------------------------------------
static void beginScroll(const char *p)
{
  scroller.setSpeed(130);      // default speed
  scroller.startScroll(p);
  restoreClock = true;
  lastStart    = millis();
}

// PROGMEM phrase ---------------------------------------------------
void scrollMsg(TxtID id)
{
  char buf[32];
  copyTxt(id, buf, sizeof(buf));
  beginScroll(buf);
}

// arbitrary String -------------------------------------------------
void scrollMsg(const String &s) { beginScroll(s.c_str()); }

// to be called once every loop() ----------------------------------
void scrollerRestoreHandler()
{
  if (restoreClock &&
      !scroller.isScrolling() &&
      (millis() - lastStart) > SCROLL_MIN_MS)
  {
    clockMode    = true;          // hand control back to clock
    restoreClock = false;
  }
}



