
#include "TimerFinishSequence.h"

TimerFinishSequence::TimerFinishSequence(TM1637Display *display, ToneLanguage *tones)
: _display(display), _tones(tones)
{
}

void TimerFinishSequence::displayStaticText(const char *text, unsigned long duration) {
    uint8_t segs[4];
    // Assume text length is 4 characters.
    for (int i = 0; i < 4; i++) {
        segs[i] = getAlphaNumSegment(text[i]);
    }
    // Reverse order for the upside-down mounted display.
    uint8_t rev[4];
    for (int i = 0; i < 4; i++) {
        rev[i] = segs[3 - i];
    }
    _display->setSegments(rev, 4);
    delay(duration);
    _display->clear();
}

void TimerFinishSequence::run() {
    // Flash "00:00" twice.
    for (int j = 0; j < 2; j++) {
        uint8_t zeros[4];
        for (int i = 0; i < 4; i++) {
            zeros[i] = getAlphaNumSegment('0');
        }
        uint8_t revZeros[4];
        for (int i = 0; i < 4; i++) {
            revZeros[i] = zeros[3 - i];
        }
        _display->setSegments(revZeros, 4);
        delay(500);
        _display->clear();
        delay(500);
    }
    
    // Play Tone 37 (Siren)
    _tones->playTone37();
    
    // Flash "BANG" three times (displayed statically)
    // "BANG" is a four-letter word; we display it without scrolling.
    for (int j = 0; j < 3; j++) {
        displayStaticText("BANG", 1000);  // Display "BANG" for 1 second
        delay(500);                       // Wait 500ms between flashes
    }
}



