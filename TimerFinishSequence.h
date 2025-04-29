
#ifndef TIMERFINISHSEQUENCE_H
#define TIMERFINISHSEQUENCE_H

#include <Arduino.h>
#include <TM1637Display.h>
#include "TM1637AlphaNum.h"
#include "ToneLanguage.h"

class TimerFinishSequence {
public:
    // Constructor: pass pointers to your display and ToneLanguage objects.
    TimerFinishSequence(TM1637Display *display, ToneLanguage *tones);

    // Runs the finish sequence:
    // 1. Flash "00:00" (all digits static) twice.
    // 2. Play Tone 37 (siren).
    // 3. Flash "BANG" (static 4-letter display) three times.
    void run();

private:
    TM1637Display *_display;
    ToneLanguage   *_tones;

    // Helper: displays a 4-character text (static) for the specified duration, then clears the display.
    // The text is reversed to account for upside-down mounting.
    void displayStaticText(const char *text, unsigned long duration);
};

#endif // TIMERFINISHSEQUENCE_H



