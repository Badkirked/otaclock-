
#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

class WiFiScanner {
  public:
    // Scans for available WiFi networks and returns a formatted String of SSIDs.
    static String scanNetworks();
};



