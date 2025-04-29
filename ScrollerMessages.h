
#pragma once
/*
   ScrollerMessages – one-line helpers for the TM1637Scroller

   • scrollMsg(TxtID)       – scroll a phrase from DisplayText.cpp
   • scrollMsg(String)      – scroll an arbitrary runtime string
   • scrollerRestoreHandler – call once per loop(); when the scroll
                              ends it switches clockMode back on
*/

#include <Arduino.h>
#include "DisplayText.h"       // TxtID / copyTxt()
#include "TM1637Scroller.h"

extern TM1637Scroller scroller; // defined in main sketch
extern bool           clockMode;

void scrollMsg(TxtID id);
void scrollMsg(const String &s);
void scrollerRestoreHandler();



