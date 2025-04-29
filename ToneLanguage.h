
#ifndef TONELANGUAGE_H
#define TONELANGUAGE_H

#include "Arduino.h"
#include "pitches.h"  // Ensure pitches.h is in the same folder

class ToneLanguage {
  public:
    // Constructor: specify the output pin for the piezo buzzer.
    ToneLanguage(uint8_t pin);
    
    // Initialization method.
    void begin(long baud = 115200);

    // 40 Tone functions (the original ones)
    void playTone1();
    void playTone2();
    void playTone3();
    void playTone4();
    void playTone5();
    void playTone6();
    void playTone7();
    void playTone8();
    void playTone9();
    void playTone10();
    void playTone11();
    void playTone12();
    void playTone13();
    void playTone14();
    void playTone15();
    void playTone16();
    void playTone17();
    void playTone18();
    void playTone19();
    void playTone20();
    void playTone21();
    void playTone22();
    void playTone23();
    void playTone24();
    void playTone25();
    void playTone26();
    void playTone27();
    void playTone28();
    void playTone29();
    void playTone30();
    void playTone31();
    void playTone32();
    void playTone33();
    void playTone34();
    void playTone35();
    void playTone36();
    void playTone37();
    void playTone38();
    void playTone39();
    void playTone40();
    
    // Additional click sound (optional)
    void playClickSound();
    
  private:
    uint8_t _pin;  // Output pin for the piezo buzzer.
};

#endif



