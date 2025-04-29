
#ifndef DISPLAYTEXT_H
#define DISPLAYTEXT_H

#include <Arduino.h>

/*  TxtID — keep it **un-scoped** so old ESP8266 GCC accepts plain
 *  comparisons and array indexing.  Must stay in sync with the
 *  table in DisplayText.cpp.                                             */
enum TxtID : uint8_t {
  TXT_HELLO = 0,
  TXT_STARTING,
  TXT_AP_HINT,
  TXT_IP,
  TXT_TONE_ON,
  TXT_TONE_OFF,
  TXT_DONE,
  TXT_COUNT               // always last
};

/*  Return a PROGMEM string for a given ID (lives in DisplayText.cpp) */
const char *txt(TxtID id);

/*  Copy a PROGMEM text into a caller-supplied buffer (helper)          */
void copyTxt(TxtID id, char *dst, size_t len);

#endif  /* DISPLAYTEXT_H */



