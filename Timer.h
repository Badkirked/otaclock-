
/*******************************************************
 * Timer.h
 *******************************************************/
#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include <TM1637Display.h>

/*
 * Countdown Timer class for TM1637:
 *
 * - Displays MM:SS on a TM1637.
 * - Blinks the colon every 500ms.
 * - After reaching zero, it displays "DONE".
 * - You call update() regularly; if it finishes,
 *   isFinished() returns true. Then your main
 *   code can switch back to clock mode if desired.
 */

class Timer {
public:
  Timer(TM1637Display *display);

  void begin();
  void setCountdown(int seconds);
  void update();
  bool isFinished() const { return _finished; }

private:
  TM1637Display *_display;
  int  _seconds;
  bool _finished;

  unsigned long _lastCount;
  unsigned long _lastBlink;
  bool _colonOn;

  void showTimeOrDone();
};

#endif



