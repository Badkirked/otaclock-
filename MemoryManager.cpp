
#include "MemoryManager.h"

void MemoryManager::begin() { EEPROM.begin(EEPROM_SIZE); }

bool MemoryManager::loadConfig(ACMEConfig &cfg){
  EEPROM.get(CONFIG_START,cfg);
  return cfg.ssid[0] && !isspace(uint8_t(cfg.ssid[0]));
}

bool MemoryManager::saveConfig(const ACMEConfig &cfg){
  EEPROM.put(CONFIG_START,cfg);
  return EEPROM.commit();
}

void MemoryManager::resetDefaults(ACMEConfig &cfg){
  memset(&cfg,0,sizeof(cfg));
  strncpy(cfg.scrollMessage,"ACME READY",sizeof(cfg.scrollMessage));
  cfg.brightness   =4;
  cfg.displayMode  =0;
  cfg.gmtOffsetMins=600;   // +10 h
  cfg.dstEnabled   =true;
  saveConfig(cfg);
}



