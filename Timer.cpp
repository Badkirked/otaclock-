
/*******************************************************
 * Timer.cpp
 *******************************************************/
#include "Timer.h"
#include "TM1637AlphaNum.h" // for getAlphaNumSegment()

Timer::Timer(TM1637Display *display)
: _display(display),
  _seconds(0),
  _finished(true),
  _lastCount(0),
  _lastBlink(0),
  _colonOn(true)
{
}

void Timer::begin() {
  _display->setBrightness(0x0F);
}

void Timer::setCountdown(int seconds) {
  if (seconds < 0) seconds = 0;
  _seconds  = seconds;
  _finished = (seconds == 0);
  _colonOn  = true;

  _lastCount = millis();
  _lastBlink = millis();
  showTimeOrDone();
}

void Timer::update() {
  unsigned long now = millis();

  // Blink colon every 500ms
  if ((now - _lastBlink) >= 500) {
    _colonOn = !_colonOn;
    _lastBlink = now;
    showTimeOrDone();
  }

  // Decrement every 1000ms if not finished
  if (!_finished && (now - _lastCount) >= 1000) {
    _lastCount = now;
    _seconds--;
    if (_seconds <= 0) {
      _seconds  = 0;
      _finished = true;
    }
    showTimeOrDone();
  }
}

// Show either MM:SS or "DONE" if finished
void Timer::showTimeOrDone() {
  if (!_finished) {
    int minutes = _seconds / 60;
    int secs    = _seconds % 60;

    char buf[5];
    buf[0] = (minutes / 10) + '0';
    buf[1] = (minutes % 10) + '0';
    buf[2] = (secs / 10) + '0';
    buf[3] = (secs % 10) + '0';
    buf[4] = '\0';

    uint8_t segs[4];
    for (int i = 0; i < 4; i++) {
      segs[i] = getAlphaNumSegment(buf[i]);
    }

    if (_colonOn) {
      segs[2] |= 0x80;  // colon on
    } else {
      segs[2] &= ~0x80; // colon off
    }

    // Reverse order for your orientation
    uint8_t rev[4] = { segs[3], segs[2], segs[1], segs[0] };
    _display->setSegments(rev);
  } else {
    const char doneMsg[5] = "DONE";
    uint8_t segs[4];
    for (int i = 0; i < 4; i++) {
      segs[i] = getAlphaNumSegment(doneMsg[i]);
    }
    uint8_t rev[4] = { segs[3], segs[2], segs[1], segs[0] };
    _display->setSegments(rev);
  }
}



