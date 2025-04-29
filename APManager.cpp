
#include "APManager.h"

void APManager::begin(const char* ssid)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);  // no password
}

String APManager::getAPIP()
{
    return WiFi.softAPIP().toString();
}
