
#ifndef TELEGRAMBOT_H
#define TELEGRAMBOT_H
/*
   Tiny Telegram helper for ESP8266
   --------------------------------
   • send(text) ................. post a plain‑text message
   • poll(msg)  ................. long‑poll for next text (true if new)
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

class TelegramBot {
public:
    TelegramBot(const String& token, const String& chatID);

    bool send(const String& text);          // outgoing
    bool poll(String& msg);                 // incoming

private:
    String _token, _chatID;
    WiFiClientSecure _client;
    uint32_t _lastUpdate = 0;

    static String urlencode(const String& s);
    bool beginRequest(HTTPClient& http, const String& path);
};

#endif /* TELEGRAMBOT_H */



