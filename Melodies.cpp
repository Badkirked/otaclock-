
// Melodies.cpp – all tables in‑line, stored in PROGMEM
// ----------------------------------------------------
#include "Melodies.h"
#include <pgmspace.h>

Melodies::Melodies(uint8_t pin) : _pin(pin) {
    pinMode(_pin, OUTPUT);
}

void Melodies::begin(long /*baud*/) {
    if (Serial) Serial.println(F("Melodies Library – Initialised"));
}

/* --------------------------------------------------------------
   Small helper used by every melody player
----------------------------------------------------------------*/
static void playMelody(const uint16_t *notes,
                       const uint8_t  *dur,
                       uint16_t        count,
                       uint8_t         pin)
{
    for (uint16_t i = 0; i < count; ++i) {
        uint16_t note = pgm_read_word(&notes[i]);
        uint8_t  div  = pgm_read_byte(&dur[i]);
        int noteDuration = 1000 / div;

        if (note != REST) tone(pin, note, noteDuration);
        delay(static_cast<int>(noteDuration * 1.30f));
        noTone(pin);
    }
}

/* ==============================================================
   NOKIA
================================================================*/
static const uint16_t nokiaNotes[] PROGMEM = {
    NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, NOTE_CS5, NOTE_B4, NOTE_D4,
    NOTE_E4, NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4, NOTE_A4
};
static const uint8_t nokiaDur[] PROGMEM = {
    8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 2
};

void Melodies::playNokia() {
    playMelody(nokiaNotes, nokiaDur,
               sizeof(nokiaDur) / sizeof(nokiaDur[0]), _pin);
}

/* ==============================================================
   MARIO
================================================================*/
static const uint16_t marioNotes[] PROGMEM = {
    NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
    NOTE_G5, REST, NOTE_G4, REST,
    NOTE_C5, NOTE_G4, REST, NOTE_E4,
    NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
    NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
    REST, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4,
    NOTE_C5, NOTE_G4, REST, NOTE_E4,
    NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
    NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
    REST, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4,

    REST, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5,
    REST, NOTE_GS4, NOTE_A4, NOTE_C4, REST, NOTE_A4, NOTE_C5, NOTE_D5,
    REST, NOTE_DS5, REST, NOTE_D5,
    NOTE_C5, REST,

    REST, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5,
    REST, NOTE_GS4, NOTE_A4, NOTE_C4, REST, NOTE_A4, NOTE_C5, NOTE_D5,
    REST, NOTE_DS5, REST, NOTE_D5,
    NOTE_C5, REST,

    NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5,
    NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,

    NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5, NOTE_E5,
    REST,
    NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5,
    NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,
    NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
    NOTE_G5, REST, NOTE_G4, REST,
    NOTE_C5, NOTE_G4, REST, NOTE_E4
};
static const uint8_t marioDur[] PROGMEM = {
    8, 8, 8, 8, 8, 8, 8,
    4, 4, 8, 4,
    4, 8, 4, 4,
    4, 4, 8, 4,
    8, 8, 8, 4, 8, 8,
    8, 4, 8, 8, 4,
    4, 8, 4, 4,
    4, 4, 8, 4,
    8, 8, 8, 4, 8, 8,
    8, 4, 8, 8, 4,

    4, 8, 8, 8, 4, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    4, 4, 8, 4,
    2, 2,

    4, 8, 8, 8, 4, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    4, 4, 8, 4,
    2, 2,

    8, 4, 8, 8, 8, 4,
    8, 4, 8, 2,

    8, 4, 8, 8, 8, 8, 8,
    1,
    8, 4, 8, 8, 8, 4,
    8, 4, 8, 2,
    8, 8, 8, 8, 8, 8, 4,
    4, 4, 4, 4,
    4, 8, 4, 4
};

void Melodies::playMario() {
    playMelody(marioNotes, marioDur,
               sizeof(marioDur) / sizeof(marioDur[0]), _pin);
}

/* ==============================================================
   SIMPSONS
================================================================*/
static const uint16_t simpsonsNotes[] PROGMEM = {
    NOTE_C4, NOTE_E4, NOTE_FS4, REST, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_C4,
    NOTE_A3, NOTE_FS3, NOTE_FS3, NOTE_FS3, NOTE_G3, REST, NOTE_FS3,
    NOTE_FS3, NOTE_FS3, NOTE_G3, NOTE_AS3, NOTE_B3, REST
};
static const uint8_t simpsonsDur[] PROGMEM = {
    2, 4, 4, 32, 8, 2, 4, 4, 8, 8, 8, 8, 4, 2, 8, 8, 8, 4, 2, 2, 2
};

void Melodies::playSimpsons() {
    playMelody(simpsonsNotes, simpsonsDur,
               sizeof(simpsonsDur) / sizeof(simpsonsDur[0]), _pin);
}

/* ==============================================================
   ICE
================================================================*/
static const uint16_t iceNotes[] PROGMEM = {
    NOTE_D3, REST, NOTE_D3, REST, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_A2, REST,
    NOTE_D3, REST, NOTE_D3, REST, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_A2, REST,
    NOTE_D3, REST, NOTE_D3, REST, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_A2, REST,
    NOTE_D3,
    NOTE_D4, REST, NOTE_D4, NOTE_D4, REST,
    NOTE_E3, NOTE_D3, NOTE_F3, REST, NOTE_F3,
    NOTE_D4, REST, NOTE_D4, NOTE_D4, REST,
    NOTE_D3,
    NOTE_D4, REST, NOTE_D4, NOTE_D4, REST,
    NOTE_E3, NOTE_D3, NOTE_F3, REST, NOTE_F3,
    NOTE_D4,
    REST
};
static const uint8_t iceDur[] PROGMEM = {
    8, 16, 8, 16, 8, 16, 10, 10, 10, 3, 2,
    8, 16, 8, 16, 8, 16, 10, 10, 10, 3, 2,
    8, 16, 8, 16, 8, 16, 10, 10, 10, 3, 2,
    4,
    4, 3, 4, 4, 3,
    6, 6, 6, 33, 6,
    4, 3, 4, 4, 3,
    4,
    4, 3, 4, 4, 3,
    6, 6, 6, 33, 6,
    3,
    1
};

void Melodies::playIce() {
    playMelody(iceNotes, iceDur, sizeof(iceDur) / sizeof(iceDur[0]), _pin);
}

/* ==============================================================
   RICK ROLL
================================================================*/
static const uint16_t rickNotes[] PROGMEM = {
  NOTE_A4, REST, NOTE_B4, REST, NOTE_C5, REST, NOTE_A4, REST,
  NOTE_D5, REST, NOTE_E5, REST, NOTE_D5, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_D5, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G5, NOTE_B4, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,
  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_E5, REST,
  NOTE_G5, NOTE_F5, NOTE_E5, REST,
  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST
};
static const uint8_t rickDur[] PROGMEM = {
  8,8,8,8,8,8,8,4, 8,8,8,8,2,2,
  8,8,8,8,2,8,8, 2,8,
  8,8,8,8,2,8,8, 4,8,8,8,8,
  8,8,8,8,2,8,8, 2,8,4,8,8,8,8,8,1,4,
  8,8,8,8,2,8,8, 2,8,
  8,8,8,8,2,8,8, 2,8,8,8,8,
  8,8,8,8,2,8,8, 4,8,3,8,8,8,8,8,1,4,
  2,6,2,6,4,4,2,6,2,3, 8,8,8,8,
  2,6,2,6,2,1
};

void Melodies::playRick() {
    playMelody(rickNotes, rickDur,
               sizeof(rickDur) / sizeof(rickDur[0]), _pin);
}

/* ==============================================================
   STAR WARS
================================================================*/
static const uint16_t starNotes[] PROGMEM = {
    NOTE_AS4, NOTE_AS4, NOTE_AS4,
    NOTE_F5, NOTE_C6,
    NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
    NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
    NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_F5, NOTE_C6,
    NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,

    NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
    NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5,
    NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
    NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
    NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,

    NOTE_C6, NOTE_G5, NOTE_G5, REST, NOTE_C5,
    NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
    NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C6, NOTE_C6,
    NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,
    NOTE_C6
};
static const uint8_t starDur[] PROGMEM = {
    8, 8, 8,
    2, 2,
    8, 8, 8, 2, 4,
    8, 8, 8, 2, 4,
    8, 8, 8, 2, 8, 8, 8,
    2, 2,
    8, 8, 8, 2, 4,

    8, 8, 8, 2, 4,
    8, 8, 8, 2, 8, 16,
    4, 8, 8, 8, 8, 8,
    8, 8, 8, 4, 8, 4, 8, 16,
    4, 8, 8, 8, 8, 8,

    8, 16, 2, 8, 8,
    4, 8, 8, 8, 8, 8,
    8, 8, 8, 4, 8, 4, 8, 16,
    4, 8, 4, 8, 4, 8, 4, 8,
    1
};

void Melodies::playStarWars() {
    playMelody(starNotes, starDur,
               sizeof(starDur) / sizeof(starDur[0]), _pin);
}

/* ==============================================================
   HARRY POTTER
================================================================*/
static const uint16_t harryNotes[] PROGMEM = {
    REST, NOTE_D4,
    NOTE_G4, NOTE_AS4, NOTE_A4,
    NOTE_G4, NOTE_D5,
    NOTE_C5,
    NOTE_A4,
    NOTE_G4, NOTE_AS4, NOTE_A4,
    NOTE_F4, NOTE_GS4,
    NOTE_D4,
    NOTE_D4,

    NOTE_G4, NOTE_AS4, NOTE_A4,
    NOTE_G4, NOTE_D5,
    NOTE_F5, NOTE_E5,
    NOTE_DS5, NOTE_B4,
    NOTE_DS5, NOTE_D5, NOTE_CS5,
    NOTE_CS4, NOTE_B4,
    NOTE_G4,
    NOTE_AS4,

    NOTE_D5, NOTE_AS4,
    NOTE_D5, NOTE_AS4,
    NOTE_DS5, NOTE_D5,
    NOTE_CS5, NOTE_A4,
    NOTE_AS4, NOTE_D5, NOTE_CS5,
    NOTE_CS4, NOTE_D4,
    NOTE_D5,
    REST, NOTE_AS4,

    NOTE_D5, NOTE_AS4,
    NOTE_D5, NOTE_AS4,
    NOTE_F5, NOTE_E5,
    NOTE_DS5, NOTE_B4,
    NOTE_DS5, NOTE_D5, NOTE_CS5,
    NOTE_CS4, NOTE_AS4,
    NOTE_G4
};
static const uint8_t harryDur[] PROGMEM = {
    2, 4,
    4, 8, 4,
    2, 4,
    2,
    2,
    4, 8, 4,
    2, 4,
    1,
    4,

    4, 8, 4,
    2, 4,
    2, 4,
    2, 4,
    4, 8, 4,
    2, 4,
    1,
    4,

    2, 4,
    2, 4,
    2, 4,
    2, 4,
    4, 8, 4,
    2, 4,
    1,
    4, 4,

    2, 4,
    2, 4,
    2, 4,
    2, 4,
    4, 8, 4,
    2, 4,
    1
};

void Melodies::playHarry() {
    playMelody(harryNotes, harryDur,
               sizeof(harryDur) / sizeof(harryDur[0]), _pin);
}



