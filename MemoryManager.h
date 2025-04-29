
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE  512          // increased to store Telegram creds
#define CONFIG_START 0

/* ------------------------------------------------------------------
   Persistent configuration structure
   ------------------------------------------------------------------*/
struct ACMEConfig {
  char ssid[32];
  char password[32];
  char scrollMessage[100];
  uint8_t brightness;        // 0‑15
  uint8_t displayMode;       // 0 = scroll, 1 = static
  int16_t gmtOffsetMins;     // minutes from UTC
  bool    dstEnabled;        // daylight‑saving flag

  /* Telegram credentials (NEW) */
  char botToken[64];
  char chatID[32];
};

class MemoryManager {
public:
  static void begin();                              // call once in setup()
  static bool loadConfig(ACMEConfig &config);       // returns false if empty
  static bool saveConfig(const ACMEConfig &config); // write & commit
  static void resetDefaults(ACMEConfig &config);    // factory defaults
};

#endif



