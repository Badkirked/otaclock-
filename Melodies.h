
#pragma once
// Melodies.h
// ----------
#include <Arduino.h>
#include "pitches.h"

class Melodies {
public:
    explicit Melodies(uint8_t pin);
    void begin(long baud = 115200);

    void playNokia();
    void playMario();
    void playSimpsons();
    void playIce();
    void playRick();
    void playStarWars();
    void playHarry();

private:
    uint8_t _pin;
};



