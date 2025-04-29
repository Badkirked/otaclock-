
// =============================  MyClock.cpp  =============================
#include "MyClock.h"
#include "TM1637AlphaNum.h"
#include "ToneLanguage.h"
#include <time.h>               // for time(), localtime_r()

extern ToneLanguage tones;      // chime on boot (defined in main sketch)

/* ------------------------------------------------------------------
 * ESP8266‑compatible helper: behave like ESP32 getLocalTime().
 * ----------------------------------------------------------------*/
static bool getLocalTimeCompat(struct tm *info)
{
    if(!info) return false;
    time_t nowSec = time(nullptr);        // epoch seconds after SNTP sync
    if(nowSec == 0) return false;         // SNTP not ready yet
    localtime_r(&nowSec, info);           // fill struct
    return true;
}

// -------------------------------------------------------------------------
MyClock::MyClock(TM1637Display *d) : _display(d) {}

void MyClock::begin(const char *ntp,long off,int dst)
{
    configTime(off,dst,ntp);              // start SNTP
    tones.playTone8();                    // little jingle
}

bool MyClock::update()
{
    struct tm t; return getLocalTimeCompat(&t);
}

void MyClock::showTime(bool colonVisible)
{
    struct tm t;
    if(!getLocalTimeCompat(&t)) return;   // no time yet

    int hr = t.tm_hour, mn = t.tm_min;
    if(hr==0) hr=12; else if(hr>12) hr-=12; // 12‑hour format

    char txt[5]; snprintf(txt,sizeof(txt),"%02d%02d",hr,mn);

    uint8_t segs[4];
    for(int i=0;i<4;i++) segs[i]=getAlphaNumSegment(txt[i]);

    uint8_t rev[4] = { segs[3], segs[2], segs[1], segs[0] }; // upside‑down board

    if(colonVisible) rev[1] |= 0x80;   // colon ON (bit7 of digit‑1)
    else             rev[1] &= ~0x80;  // colon OFF

    _display->setSegments(rev);
}

void MyClock::printToSerial()
{
    struct tm t;
    if(getLocalTimeCompat(&t))
        Serial.printf("%02d:%02d:%02d\n", t.tm_hour, t.tm_min, t.tm_sec);
}



