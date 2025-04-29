
#include "TM1637Scroller.h"

TM1637Scroller::TM1637Scroller(TM1637Display *display)
: _display(display),
  _speedMs(500),
  _scrolling(false),
  _text(nullptr),
  _textLen(0),
  _currentPos(0),
  _lastStepTime(0)
{
}

void TM1637Scroller::setSpeed(uint16_t speedMs) {
  _speedMs = speedMs;
}

// ------------------------------------------------------------------------
// Non-blocking scrolling
// ------------------------------------------------------------------------
void TM1637Scroller::startScroll(const char *text) {
  if (!text) return;

  // Initialize our state
  _text = text;
  _textLen = strlen(_text);
  _currentPos = 0;
  _scrolling = true;
  _lastStepTime = millis();

  // Immediately show the first window
  displayWindow(_currentPos);
}

void TM1637Scroller::updateScroll() {
  if (!_scrolling) return; // nothing to do if not scrolling

  unsigned long now = millis();
  if ((now - _lastStepTime) >= _speedMs) {
    _lastStepTime = now;
    _currentPos++;

    // If we move beyond the last character, end the scroll
    if (_currentPos >= _textLen) {
      _scrolling = false;
    } else {
      displayWindow(_currentPos);
    }
  }
}

// Displays the 4-char window starting at 'pos'
void TM1637Scroller::displayWindow(int pos) {
  // We'll show [pos, pos+1, pos+2, pos+3].
  // If index is out of range, use ' ' (space).
  uint8_t data[4];
  for (int i = 0; i < 4; i++) {
    int idx = pos + i;
    char c = (idx < _textLen) ? _text[idx] : ' '; // blank if beyond end
    data[i] = getAlphaNumSegment(c);
  }

  // Reverse order if needed for your wiring orientation
  uint8_t reversed[4];
  reversed[0] = data[3];
  reversed[1] = data[2];
  reversed[2] = data[1];
  reversed[3] = data[0];

  _display->setSegments(reversed);
}

// ------------------------------------------------------------------------
// Blocking functions (old style)
// ------------------------------------------------------------------------
void TM1637Scroller::scrollText(const char *text) {
  int len = strlen(text);

  for (int pos = 0; pos < len; pos++) {
    uint8_t data[4];
    // Create a window of 4 characters (wrapping around using modulo).
    for (int j = 0; j < 4; j++) {
      char c = text[(pos + j) % len];
      data[j] = getAlphaNumSegment(c);
    }

    // Reverse for hardware orientation
    uint8_t reversed[4];
    reversed[0] = data[3];
    reversed[1] = data[2];
    reversed[2] = data[1];
    reversed[3] = data[0];

    _display->setSegments(reversed);
    delay(_speedMs);
  }
}

void TM1637Scroller::scrollTextLoop(const char *text) {
  // Repeatedly call scrollText forever
  while (true) {
    scrollText(text);
  }
}



