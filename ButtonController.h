
#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include <Arduino.h>
#include "MyButton.h"
#include "ToneLanguage.h"
#include "Melodies.h"
#include "Timer.h"

/* --------------------------------------------------------------
 * ButtonController: handles tone‑mode & tune‑mode,
 * now also exposes the last completed action to main sketch.
 * --------------------------------------------------------------*/
class ButtonController {
public:
  ButtonController(uint8_t buttonPin,
                   ToneLanguage *tones,
                   Melodies *melodies,
                   Timer *timer,
                   bool *clockModeRef,
                   bool *timerModeRef);

  void init();
  void update();

  /* return last recognized action, then clear it */
  ButtonAction lastAction();

private:
  MyButton      _button;
  ToneLanguage *_tones;
  Melodies     *_melodies;
  Timer        *_timer;
  bool         *_clockMode;
  bool         *_timerMode;

  bool _tuneMode;
  unsigned long _tuneStart;

  ButtonAction _lastAction = NONE;   // NEW

  void handleToneModeAction(ButtonAction action);
  void handleTuneModeAction(ButtonAction action);
  void playSelectedMelody(uint8_t taps);
};

#endif



