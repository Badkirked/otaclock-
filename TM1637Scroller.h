
#ifndef TM1637SCROLLER_H
#define TM1637SCROLLER_H

#include <Arduino.h>
#include <TM1637Display.h>
#include "TM1637AlphaNum.h"  // Our alphanumeric conversion library

class TM1637Scroller {
public:
    // Constructor: pass a pointer to your TM1637Display object.
    TM1637Scroller(TM1637Display *display);

    // Sets the scroll speed in milliseconds per scroll step.
    // Example: setSpeed(300) => shift the text every 300 ms.
    void setSpeed(uint16_t speedMs);

    // ------------------------------------------------------------------------
    // Non-blocking scrolling methods
    // ------------------------------------------------------------------------
    // 1) startScroll(text): begins scrolling of "text" from left to right.
    // 2) updateScroll(): call periodically in loop() to shift the text
    //    by one position if enough time has elapsed.
    // 3) isScrolling(): returns true while the scroller is active.

    void startScroll(const char *text);
    void updateScroll();
    bool isScrolling() const { return _scrolling; }

    // ------------------------------------------------------------------------
    // The older blocking functions (not recommended if you need real-time
    // responsiveness). They remain available if your code still calls them.
    // ------------------------------------------------------------------------
    // scrollText() scrolls the text once, fully blocking until done.
    void scrollText(const char *text);

    // scrollTextLoop() scrolls the text continuously in an infinite loop
    // (blocking), never returning.
    void scrollTextLoop(const char *text);

private:
    TM1637Display *_display;
    uint16_t _speedMs; // ms delay between scroll steps

    // Non-blocking scroll state variables
    bool        _scrolling;     // are we in the middle of a scroll pass?
    const char* _text;          // text to scroll
    uint8_t     _textLen;       // length of text
    int         _currentPos;    // which character index is at the left edge
    unsigned long _lastStepTime;// when we last advanced the scroll

    // Helper to display a 4-char window starting at pos
    void displayWindow(int pos);
};

#endif // TM1637SCROLLER_H



