
#include "MyButton.h"

MyButton::MyButton(uint8_t pin)
: _pin(pin),
  _btnState(true),    // Using pull-up, so default "released" = HIGH
  _prevBtnState(true),
  _stateChangeTime(0),
  _pressStartTime(0),
  _tapTimer(0),
  _tapCount(0),
  _finalTapCount(0),
  _currentAction(NONE)
{
  pinMode(_pin, INPUT_PULLUP);
}

void MyButton::update() {
  // Read raw input (invert since pull-up is used, pressed = LOW)
  bool rawState = digitalRead(_pin);

  unsigned long now = millis();
  _currentAction   = NONE;

  // Debounce check
  if (rawState != _prevBtnState && (now - _stateChangeTime) > DEBOUNCE_DELAY) {
    // State changed after stable period
    _prevBtnState    = rawState;
    _stateChangeTime = now;

    if (!rawState) {
      // Button was just pressed
      _pressStartTime = now;
    } else {
      // Button was just released
      unsigned long pressDuration = now - _pressStartTime;

      if (pressDuration >= VERY_LONG_PRESS_TIME) {
        // Very long press => MENU
        _currentAction = MENU;
        _finalTapCount = 0; // Reset tap sequence
      }
      else if (pressDuration >= LONG_PRESS_TIME) {
        // "Long" press => SELECT
        _currentAction = SELECT;
        _finalTapCount = 0; // Reset tap sequence
      }
      else {
        // Quick press => increment tap count
        _tapCount++;
        // Restart the tap timer
        _tapTimer = now;
      }
    }
    // Update stable state
    _btnState = !rawState ? false : true;
  }

  // Check if we are waiting to see if user does multiple taps
  if (_tapCount > 0 && (now - _tapTimer) > TAP_GAP) {
    // Tap sequence done
    finalizeTapCount();
  }
}

// Convert the final _tapCount into one of our enumerated actions
void MyButton::finalizeTapCount() {
  _finalTapCount = _tapCount;
  _tapCount      = 0;

  switch (_finalTapCount) {
    case 1: _currentAction = START;     break;
    case 2: _currentAction = NEXT;      break;
    case 3: _currentAction = BTN_CANCEL;break;
    case 4: _currentAction = PAUSE;     break;
    case 5: _currentAction = RESUME;    break;
    case 6: _currentAction = RESET_TIMER; break;
    default:
      // If 7 or more taps, we handle as “Song 7” in Tune Mode or something special:
      if (_finalTapCount >= 7) {
        // We'll just treat it as BTN_CANCEL or your special choice
        _currentAction = BTN_CANCEL; 
      } else {
        _currentAction = NONE;
      }
      break;
  }
}

// Return the recognized action so the caller can react to it.
ButtonAction MyButton::getAction() {
  ButtonAction temp = _currentAction;
  _currentAction = NONE; // Clear after reading
  return temp;
}

unsigned long MyButton::getHoldDuration() const {
  if (!isPressed()) return 0;
  return millis() - _pressStartTime;
}



