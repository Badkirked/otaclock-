
// ToneLanguage.cpp – single‑initialisation version
// -------------------------------------------------
#include "ToneLanguage.h"

ToneLanguage::ToneLanguage(uint8_t pin) : _pin(pin) {
    pinMode(_pin, OUTPUT);
}

void ToneLanguage::begin(long /*baud*/) {
    if (Serial) Serial.println(F("Tone Language Library – Initialised"));
}

/* -------------------------------------------------
   Tone 1 : ON / Power On (1000 Hz for 200 ms)
--------------------------------------------------*/
void ToneLanguage::playTone1() {
    tone(_pin, 1000, 200);
    delay(250);
    noTone(_pin);
}

/* Tone 2 : OFF / Power Off (600 Hz for 200 ms) */
void ToneLanguage::playTone2() {
    tone(_pin, 600, 200);
    delay(250);
    noTone(_pin);
}

/* Tone 3 : CONNECTED (800 → 1000 Hz) */
void ToneLanguage::playTone3() {
    tone(_pin, 800, 100);
    delay(150);
    tone(_pin, 1000, 100);
    delay(150);
    noTone(_pin);
}

/* Tone 4 : ALARM – 1200 Hz beeps (3 s) */
void ToneLanguage::playTone4() {
    unsigned long start = millis();
    while (millis() - start < 3000) {
        tone(_pin, 1200, 100);
        delay(150);
    }
    noTone(_pin);
}

/* Tone 5 : BEEP CONTINUOUS (900 Hz, 2 s) */
void ToneLanguage::playTone5() {
    tone(_pin, 900);
    delay(2000);
    noTone(_pin);
}

/* Tone 6 : TICK‑TOCK (1000 / 800 Hz ×3) */
void ToneLanguage::playTone6() {
    for (int i = 0; i < 3; i++) {
        tone(_pin, 1000, 100);
        delay(150);
        tone(_pin, 800, 100);
        delay(150);
    }
    noTone(_pin);
}

/* Tone 7 : BEEP‑BEEP playful alert */
void ToneLanguage::playTone7() {
    tone(_pin, 400, 100);
    delay(150);
    tone(_pin, 1000, 100);
    delay(150);
    tone(_pin, 400, 100);
    delay(150);
    tone(_pin, 1000, 100);
    delay(150);
    noTone(_pin);
}

/* Tone 8 : START‑UP (ascending 800‑1200 Hz) */
void ToneLanguage::playTone8() {
    tone(_pin, 800, 100);
    delay(150);
    tone(_pin, 1000, 100);
    delay(150);
    tone(_pin, 1200, 150);
    delay(200);
    noTone(_pin);
}

/* Tone 9 : SHUT‑DOWN (descending 1200‑800 Hz) */
void ToneLanguage::playTone9() {
    tone(_pin, 1200, 150);
    delay(200);
    tone(_pin, 1000, 100);
    delay(150);
    tone(_pin, 800, 100);
    delay(150);
    noTone(_pin);
}

/* Tone 10 : ERROR (1000‑800‑600 Hz) */
void ToneLanguage::playTone10() {
    tone(_pin, 1000, 100);
    delay(120);
    tone(_pin, 800, 100);
    delay(120);
    tone(_pin, 600, 100);
    delay(120);
    noTone(_pin);
}

/* Tone 11 : WARNING – three 1100 Hz beeps */
void ToneLanguage::playTone11() {
    for (int i = 0; i < 3; i++) {
        tone(_pin, 1100, 80);
        delay(50);
        noTone(_pin);
        delay(50);
    }
}

/* Tone 12 : NOTIFICATION – short 900 Hz beep */
void ToneLanguage::playTone12() {
    tone(_pin, 900, 80);
    delay(100);
    noTone(_pin);
}

/* Tone 13 : SLOW RISING 600‑1000 Hz / 500 ms */
void ToneLanguage::playTone13() {
    unsigned long start = millis();
    while (millis() - start < 500) {
        int f = map(millis() - start, 0, 500, 600, 1000);
        tone(_pin, f);
        delay(10);
    }
    noTone(_pin);
}

/* Tone 14 : FAST RISING 600‑1200 Hz / 200 ms */
void ToneLanguage::playTone14() {
    unsigned long start = millis();
    while (millis() - start < 200) {
        int f = map(millis() - start, 0, 200, 600, 1200);
        tone(_pin, f);
        delay(10);
    }
    noTone(_pin);
}

/* Tone 15 : SLOW FALLING 1000‑600 Hz / 500 ms */
void ToneLanguage::playTone15() {
    unsigned long start = millis();
    while (millis() - start < 500) {
        int f = map(millis() - start, 0, 500, 1000, 600);
        tone(_pin, f);
        delay(10);
    }
    noTone(_pin);
}

/* Tone 16 : FAST FALLING 1200‑600 Hz / 200 ms */
void ToneLanguage::playTone16() {
    unsigned long start = millis();
    while (millis() - start < 200) {
        int f = map(millis() - start, 0, 200, 1200, 600);
        tone(_pin, f);
        delay(10);
    }
    noTone(_pin);
}

/* Tone 17 : DOUBLE BEAT – two 800 Hz beeps */
void ToneLanguage::playTone17() {
    for (int i = 0; i < 2; i++) {
        tone(_pin, 800, 100);
        delay(150);
    }
    noTone(_pin);
}

/* Tone 18 : TRIPLE BEAT – three 800 Hz beeps */
void ToneLanguage::playTone18() {
    for (int i = 0; i < 3; i++) {
        tone(_pin, 800, 100);
        delay(150);
    }
    noTone(_pin);
}

/* Tone 19 : QUADRUPLE BEAT – four 800 Hz beeps */
void ToneLanguage::playTone19() {
    for (int i = 0; i < 4; i++) {
        tone(_pin, 800, 100);
        delay(150);
    }
    noTone(_pin);
}

/* Tone 20 : RANDOM 1 (700, 1100, 900 Hz) */
void ToneLanguage::playTone20() {
    tone(_pin, 700, 150);
    delay(200);
    tone(_pin, 1100, 150);
    delay(200);
    tone(_pin, 900, 150);
    delay(200);
    noTone(_pin);
}

/* Tone 21 : RANDOM 2 (800 → 1200 Hz) */
void ToneLanguage::playTone21() {
    tone(_pin, 800, 200);
    delay(250);
    tone(_pin, 1200, 200);
    delay(250);
    noTone(_pin);
}

/* Tone 22 : RANDOM 3 (950 → 650 Hz) */
void ToneLanguage::playTone22() {
    tone(_pin, 950, 150);
    delay(200);
    tone(_pin, 650, 150);
    delay(200);
    noTone(_pin);
}

/* Tone 23 : RANDOM 4 (850 / 750 / 950 Hz) */
void ToneLanguage::playTone23() {
    tone(_pin, 850, 100);
    delay(120);
    tone(_pin, 750, 100);
    delay(120);
    tone(_pin, 950, 100);
    delay(120);
    noTone(_pin);
}

/* Tone 24 : RANDOM 5 (1000 → 900 Hz) */
void ToneLanguage::playTone24() {
    tone(_pin, 1000, 150);
    delay(200);
    tone(_pin, 900, 150);
    delay(200);
    noTone(_pin);
}

/* Tone 25 : RANDOM 6 – alternating 800/1000 Hz ×3 */
void ToneLanguage::playTone25() {
    for (int i = 0; i < 3; i++) {
        tone(_pin, 800, 100);
        delay(150);
        tone(_pin, 1000, 100);
        delay(150);
    }
    noTone(_pin);
}

/* Tone 26 : RANDOM 7 (700,800,900,1000 Hz) */
void ToneLanguage::playTone26() {
    tone(_pin, 700, 100);
    delay(120);
    tone(_pin, 800, 100);
    delay(120);
    tone(_pin, 900, 100);
    delay(120);
    tone(_pin, 1000, 100);
    delay(120);
    noTone(_pin);
}

/* Tone 27 : RANDOM 8 (1100 → 1000 Hz) */
void ToneLanguage::playTone27() {
    tone(_pin, 1100, 150);
    delay(200);
    tone(_pin, 1000, 150);
    delay(200);
    noTone(_pin);
}

/* Tone 28 : RANDOM 9 – 850 Hz, 300 ms */
void ToneLanguage::playTone28() {
    tone(_pin, 850, 300);
    delay(350);
    noTone(_pin);
}

/* Tone 29 : RANDOM 10 – 950 Hz, 300 ms */
void ToneLanguage::playTone29() {
    tone(_pin, 950, 300);
    delay(350);
    noTone(_pin);
}

/* Tone 30 : LOW PULSE 600 Hz, 100 ms pulses (2 s) */
void ToneLanguage::playTone30() {
    unsigned long start = millis();
    while (millis() - start < 2000) {
        tone(_pin, 600);
        delay(100);
        noTone(_pin);
        delay(100);
    }
}

/* Tone 31 : HIGH PULSE 1200 Hz, 100 ms pulses (2 s) */
void ToneLanguage::playTone31() {
    unsigned long start = millis();
    while (millis() - start < 2000) {
        tone(_pin, 1200);
        delay(100);
        noTone(_pin);
        delay(100);
    }
}

/* Tone 32 : SHORT BEEP 1000 Hz, 50 ms */
void ToneLanguage::playTone32() {
    tone(_pin, 1000, 50);
    delay(70);
    noTone(_pin);
}

/* Tone 33 : LONG BEEP 1000 Hz, 500 ms */
void ToneLanguage::playTone33() {
    tone(_pin, 1000, 500);
    delay(550);
    noTone(_pin);
}

/* Tone 34 : BEEP‑SILENCE‑BEEP */
void ToneLanguage::playTone34() {
    tone(_pin, 1000, 100);
    delay(100);
    noTone(_pin);
    delay(100);
    tone(_pin, 1000, 100);
    delay(150);
    noTone(_pin);
}

/* Tone 35 : ASCENDING SCALE 600‑1000 Hz */
void ToneLanguage::playTone35() {
    for (int f = 600; f <= 1000; f += 100) {
        tone(_pin, f, 100);
        delay(120);
    }
    noTone(_pin);
}

/* Tone 36 : DESCENDING SCALE 1000‑600 Hz */
void ToneLanguage::playTone36() {
    for (int f = 1000; f >= 600; f -= 100) {
        tone(_pin, f, 100);
        delay(120);
    }
    noTone(_pin);
}

/* Tone 37 : SIREN 1000↔1200 Hz (3 s) */
void ToneLanguage::playTone37() {
    unsigned long start = millis();
    while (millis() - start < 3000) {
        for (int f = 1000; f <= 1200; f += 20) {
            tone(_pin, f);
            delay(20);
        }
        for (int f = 1200; f >= 1000; f -= 20) {
            tone(_pin, f);
            delay(20);
        }
    }
    noTone(_pin);
}

/* Tone 38 : RUMBLE – 300 Hz pulses (3 s) */
void ToneLanguage::playTone38() {
    unsigned long start = millis();
    while (millis() - start < 3000) {
        tone(_pin, 300, 100);
        delay(150);
        noTone(_pin);
        delay(150);
    }
}

/* Tone 39 : CLICK – 200 Hz, 30 ms */
void ToneLanguage::playTone39() {
    tone(_pin, 200, 30);
    delay(50);
    noTone(_pin);
}

/* Tone 40 : BUZZ – 800 Hz continuous (1 s) */
void ToneLanguage::playTone40() {
    tone(_pin, 800);
    delay(1000);
    noTone(_pin);
}

/* -------------------------------------------------
   Extra helper: click sound (re‑uses Tone 39)
--------------------------------------------------*/
void ToneLanguage::playClickSound() {
    playTone39();
}



