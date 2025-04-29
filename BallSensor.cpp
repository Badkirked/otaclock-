
#include "BallSensor.h"

BallSensor::BallSensor(uint8_t pin) : _pin(pin), _lastState(-1) {}

void BallSensor::begin() {
    pinMode(_pin, INPUT_PULLUP);
    if (Serial) Serial.println(F("Ball Sensor Initialised"));
}

void BallSensor::update() {
    int current = digitalRead(_pin);
    if (current != _lastState) {
        _lastState = current;
        if (Serial) {
            Serial.println(current == LOW ?
                F("🟢 Movement Detected (LOW)") :
                F("⚪️ No Movement (HIGH)"));
        }
    }
}



