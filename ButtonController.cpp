
#include "ButtonController.h"

ButtonController::ButtonController(uint8_t buttonPin,
                                   ToneLanguage *tones,
                                   Melodies *melodies,
                                   Timer *timer,
                                   bool *clockModeRef,
                                   bool *timerModeRef)
: _button(buttonPin),
  _tones(tones),
  _melodies(melodies),
  _timer(timer),
  _clockMode(clockModeRef),
  _timerMode(timerModeRef),
  _tuneMode(false),
  _tuneStart(0),
  _lastAction(NONE) {}

void ButtonController::init(){ /* nothing */ }

void ButtonController::update()
{
  _button.update();

  /* if in tune mode, long‑hold exits */
  if(_tuneMode && _button.isPressed() &&
     _button.getHoldDuration() >= 5000){
        _tuneMode=false; if(_tones) _tones->playTone4();
  }

  ButtonAction action = _button.getAction();
  if(action==NONE) return;

  /* remember for outside world */
  _lastAction = action;

  if(_tuneMode) handleTuneModeAction(action);
  else          handleToneModeAction(action);
}

ButtonAction ButtonController::lastAction()
{
  ButtonAction a = _lastAction;
  _lastAction = NONE;
  return a;
}

/* ------------- Tone‑mode actions (unchanged) ------------------ */
void ButtonController::handleToneModeAction(ButtonAction action)
{
  switch(action){
    case START:  if(_tones) _tones->playTone8();                 break;
    case NEXT:   if(_tones) _tones->playTone7();
                 *_clockMode=false; *_timerMode=true; if(_timer) _timer->setCountdown(10); break;
    case BTN_CANCEL: if(_tones) _tones->playTone10();            break;
    case PAUSE:  _tuneMode=true; _tuneStart=millis(); if(_tones) _tones->playTone16(); break;
    case SELECT: if(_tones) _tones->playTone2();                 break;
    case MENU:   if(_tones) _tones->playTone37();                break;
    default: break;
  }
}

/* ------------- Tune‑mode actions (unchanged) ------------------ */
void ButtonController::handleTuneModeAction(ButtonAction action)
{
  uint8_t taps = _button.getFinalTapCount();
  if(taps>0){ playSelectedMelody(taps); _button.clearFinalTapCount(); }
  if(action==SELECT || action==MENU){
      _tuneMode=false; if(_tones) _tones->playTone4();
  }
}

void ButtonController::playSelectedMelody(uint8_t taps)
{
  if(!_melodies) return;
  switch(taps){
    case 1: _melodies->playNokia();    break;
    case 2: _melodies->playMario();    break;
    case 3: _melodies->playSimpsons(); break;
    case 4: _melodies->playIce();      break;
    case 5: _melodies->playRick();     break;
    case 6: _melodies->playStarWars(); break;
    default:_melodies->playHarry();    break;
  }
}



