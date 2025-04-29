
#ifndef BALLSENSOR_H      // Prevents recursive includes
#define BALLSENSOR_H

#include <Arduino.h>

class BallSensor {
public:
  BallSensor(uint8_t pin);
  void begin();
  void update();

  int getState() const { return _lastState; }

private:
  uint8_t _pin;
  int _lastState;
};

#endif  // BALLSENSOR_H



