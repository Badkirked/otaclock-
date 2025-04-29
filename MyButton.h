
#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <Arduino.h>

// Times in milliseconds for press/hold thresholds
#define DEBOUNCE_DELAY           50    // Debounce interval
#define TAP_GAP                  300   // Time allowed between taps
#define LONG_PRESS_TIME          700   // 0.7s for a "long" press
#define VERY_LONG_PRESS_TIME     1500  // 1.5s for a "very long" press

// The recognized actions from a button press/tap/hold
enum ButtonAction {
  NONE = 0,
  START,       // Single tap  (Tone Mode) / Song 1 (Tune Mode)
  NEXT,        // Double tap  (Toggle arm state)
  SELECT,      // Long press  (Tone Mode) / exit in Tune Mode
  BTN_CANCEL,  // Triple tap  (Tone Mode) / Song 3 (Tune Mode)
  PAUSE,       // Quad tap    => Switch to Tune Mode
  RESUME,      // Quint tap   => (Optional extra action if desired)
  RESET_TIMER, // Sext tap    => (Optional extra action if desired)
  MENU         // Very long press
};

class MyButton {
public:
  // Constructor: pass in the pin connected to the button (with internal pull-up).
  MyButton(uint8_t pin);

  // Call this frequently (e.g. once per loop) to update internal state.
  void update();

  // After update() indicates a "finalized" gesture, getAction() returns that action once.
  ButtonAction getAction();

  // The total number of taps in the most recently finalized tap-sequence.
  uint8_t getFinalTapCount() const { return _finalTapCount; }

  // Clears the stored tap count once it’s used (for tune-mode).
  void clearFinalTapCount() { _finalTapCount = 0; }

  // Returns true if the button is physically pressed right now.
  bool isPressed() const { return !_btnState; }

  // How long (ms) the button has been continuously pressed (0 if not pressed).
  unsigned long getHoldDuration() const;

private:
  uint8_t _pin;
  bool    _btnState;      // True = button reads HIGH, false = LOW
  bool    _prevBtnState;  // Last stable state

  unsigned long _stateChangeTime;  // Time of last state change
  unsigned long _pressStartTime;   // When the user first pressed the button
  unsigned long _tapTimer;         // When waiting for multiple taps
  uint8_t       _tapCount;         // Current # of taps in an ongoing sequence
  uint8_t       _finalTapCount;    // Final # of taps after user finishes tapping

  ButtonAction  _currentAction;    // Action recognized from last update

  // Internal helper to finalize a tap sequence & deduce the button action
  void finalizeTapCount();
};

#endif



