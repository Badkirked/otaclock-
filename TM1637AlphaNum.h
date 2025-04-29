
#ifndef TM1637ALPHANUM_H
#define TM1637ALPHANUM_H

#include <Arduino.h>

/* --- Untransformed digit definitions --- */
static const uint8_t digitToSegment[10] = {
  0x3F, // 0 => a,b,c,d,e,f
  0x06, // 1 => b,c
  0x5B, // 2 => a,b,d,e,g
  0x4F, // 3 => a,b,c,d,g
  0x66, // 4 => b,c,f,g
  0x6D, // 5 => a,c,d,f,g
  0x7D, // 6 => a,c,d,e,f,g
  0x07, // 7 => a,b,c
  0x7F, // 8 => a,b,c,d,e,f,g
  0x6F  // 9 => a,b,c,d,f,g
};

/* --- Untransformed letter definitions --- */
#define LETTER_A 0x77  // a,b,c,e,f,g
#define LETTER_B 0x7C  // c,d,e,f,g (resembles lowercase b)
#define LETTER_C 0x39  // a,d,e,f
#define LETTER_D 0x5E  // b,c,d,e,g
#define LETTER_E 0x79  // a,d,e,f,g
#define LETTER_F 0x71  // a,e,f,g
#define LETTER_G 0x3D  // a,c,d,e,f
#define LETTER_H 0x76  // b,c,e,f,g
#define LETTER_I 0x30  // e,f (approximation)
#define LETTER_J 0x1E  // b,c,d,e
#define LETTER_L 0x38  // d,e,f
#define LETTER_N 0x37  // a,c,e,f,g (approximation)
#define LETTER_O 0x3F  // a,b,c,d,e,f (same as 0)
#define LETTER_P 0x73  // a,b,e,f,g
#define LETTER_Q 0x67  // a,b,c,f,g (approximation)
#define LETTER_R 0x50  // e,g (approximation)
#define LETTER_S 0x6D  // a,c,d,f,g (same as 5)
#define LETTER_T 0x78  // d,e,f,g
#define LETTER_U 0x3E  // b,c,d,e,f
#define LETTER_Y 0x6E  // b,c,d,f,g
#define LETTER_Z 0x5B  // a,b,d,e,g (same as 2)

// Definition for colon ":" (center dots typically at 0x80)
#define COLON 0x80

/*
  Combined transformation function:
  Applies a 180° rotation and a horizontal mirror.

  Mapping:
    a (0x01) -> d (0x08)
    b (0x02) -> e (0x10)
    c (0x04) -> f (0x20)
    d (0x08) -> a (0x01)
    e (0x10) -> b (0x02)
    f (0x20) -> c (0x04)
    g (0x40) -> g (0x40)
    colon (0x80) remains colon (0x80)
*/
inline uint8_t flipAndMirrorSegments(uint8_t seg) {
  uint8_t mapped = 0;
  if (seg & 0x01) mapped |= 0x08;  // a -> d
  if (seg & 0x02) mapped |= 0x10;  // b -> e
  if (seg & 0x04) mapped |= 0x20;  // c -> f
  if (seg & 0x08) mapped |= 0x01;  // d -> a
  if (seg & 0x10) mapped |= 0x02;  // e -> b
  if (seg & 0x20) mapped |= 0x04;  // f -> c
  if (seg & 0x40) mapped |= 0x40;  // g remains g
  if (seg & 0x80) mapped |= 0x80;  // colon remains colon
  return mapped;
}

/*
  getDigitSegment()
  -----------------
  Given a digit character ('0'-'9'), returns the transformed 7-segment code.
*/
inline uint8_t getDigitSegment(char digit) {
  if (digit >= '0' && digit <= '9') {
    uint8_t seg = digitToSegment[digit - '0'];
    return flipAndMirrorSegments(seg);
  }
  return 0;
}

/*
  getLetterSegment()
  ------------------
  Given a capital letter ('A'-'Z'), returns the transformed 7-segment code.
*/
inline uint8_t getLetterSegment(char letter) {
  uint8_t seg = 0;
  switch (letter) {
    case 'A': seg = LETTER_A; break;
    case 'B': seg = LETTER_B; break;
    case 'C': seg = LETTER_C; break;
    case 'D': seg = LETTER_D; break;
    case 'E': seg = LETTER_E; break;
    case 'F': seg = LETTER_F; break;
    case 'G': seg = LETTER_G; break;
    case 'H': seg = LETTER_H; break;
    case 'I': seg = LETTER_I; break;
    case 'J': seg = LETTER_J; break;
    case 'L': seg = LETTER_L; break;
    case 'N': seg = LETTER_N; break;
    case 'O': seg = LETTER_O; break;
    case 'P': seg = LETTER_P; break;
    case 'Q': seg = LETTER_Q; break;
    case 'R': seg = LETTER_R; break;
    case 'S': seg = LETTER_S; break;
    case 'T': seg = LETTER_T; break;
    case 'U': seg = LETTER_U; break;
    case 'Y': seg = LETTER_Y; break;
    case 'Z': seg = LETTER_Z; break;
    default: seg = 0; break;
  }
  return flipAndMirrorSegments(seg);
}

/*
  getAlphaNumSegment()
  ---------------------
  Given an alphanumeric character (digit, letter, or colon), returns the transformed
  7-segment code.
*/
inline uint8_t getAlphaNumSegment(char c) {
  if (c >= '0' && c <= '9')
    return getDigitSegment(c);
  if (c >= 'A' && c <= 'Z')
    return getLetterSegment(c);
  if (c >= 'a' && c <= 'z')
    return getLetterSegment(c - 'a' + 'A');
  if (c == ':')
    return flipAndMirrorSegments(COLON); // Colon character
  return 0;
}

#endif // TM1637ALPHANUM_H



