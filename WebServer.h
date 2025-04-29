#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <Arduino.h>

class MyWebServer {
public:
  // Constructor takes the port (usually 80)
  explicit MyWebServer(uint16_t port = 80);

  // Call once in setup()
  void begin();
  // Call in loop()
  void handleClient();

  // Hook for your scrolling buffer
  void setScrollMessage(String *msg);
  // Check if user submitted new scroll text
  bool hasNewMessage();
  String getNewMessage();

private:
  ESP8266WebServer _server;
  String*          _scrollMessage;
  bool             _newMessageFlag;
  String           _newMessage;

  // Dashboard
  void handleRoot();
  // Set scrolling text
  void handleSetMessage();
  // Wi-Fi scan & join
  void handleScan();
  void handleConnect();
  // Tunes
  void handlePlayMelody();
  // Timer control
  void handleTimer();
  void handleCancelTimer();
  // Timezone
  void handleSetTime();
  // Telegram credentials
  void handleSetTelegram();
  // 🔥 OTA Firmware Update
  void handleUpdateFirmware();  // <-- *** ADD THIS ***
};

#endif // WEBSERVER_H
