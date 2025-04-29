
/*  DisplayText.cpp
 *  Central store for short, scroll-safe phrases.
 *  Alphabet reduced: A-Z (no K M V W X), digits, space, ':'.
 *  -------------------------------------------------------- */

#include <Arduino.h>
#include <pgmspace.h>
#include "DisplayText.h"

/* ---------- String literals in PROGMEM ------------------ */
static const char txt_hello[]      PROGMEM = "HELLO UP";
static const char txt_starting[]   PROGMEM = "START UP";
static const char txt_ap_hint[]    PROGMEM = "AP 192.168.4.1";
static const char txt_ip[]         PROGMEM = "IP:";
static const char txt_tone_on[]    PROGMEM = "TONE ON";
static const char txt_tone_off[]   PROGMEM = "TONE OFF";
static const char txt_done[]       PROGMEM = "DONE";

/*  Table of pointers (also in PROGMEM)                     */
static const char * const gTable[] PROGMEM = {
  txt_hello,
  txt_starting,
  txt_ap_hint,
  txt_ip,
  txt_tone_on,
  txt_tone_off,
  txt_done
};

/* ---------- Accessors ----------------------------------- */
const char *txt(TxtID id)
{
  if (id >= TXT_COUNT) return "";
  return (const char*)pgm_read_ptr(&gTable[id]);
}

void copyTxt(TxtID id, char *dst, size_t len)
{
  if (!dst || !len) return;
  strncpy_P(dst, txt(id), len - 1);
  dst[len - 1] = '\0';
}



