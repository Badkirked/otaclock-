
#ifndef APMANAGER_H
#define APMANAGER_H

#include <ESP8266WiFi.h>
#include <Arduino.h>

class APManager {
public:
    static void begin(const char* ssid);  // open AP
    static String getAPIP();
};

#endif
