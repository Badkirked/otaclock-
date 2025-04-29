/*
 *  WebServer.cpp – ACME Super Device (Looney Tunes mobile UI + OTA fixed + EEPROM autoload, no struct conflict)
 *  ---------------------------------------------------------------------
 *  Minimal strings for TM1637 display but full
 *  Looney Tunes-style WEB UI for phones & tablets.
 */

#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <EEPROM.h>
#include "WebServer.h"
#include "MemoryManager.h"   // <--- ACMEConfig is already defined here
#include "ToneLanguage.h"
#include "Melodies.h"
#include "Timer.h"
#include "MyClock.h"

extern ToneLanguage tones;
extern Melodies     melodies;
extern Timer        myTimer;
extern MyClock      myClock;
extern bool         timerMode;
extern bool         clockMode;
extern bool         reinitTelegram;

ACMEConfig config;    // <-- USE existing struct from MemoryManager
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

MyWebServer::MyWebServer(uint16_t port)
  : _server(port),
    _scrollMessage(nullptr),
    _newMessageFlag(false),
    _newMessage("") {}

void MyWebServer::setScrollMessage(String *msg) { _scrollMessage = msg; }

void MyWebServer::begin() {
  // Load saved Wi-Fi + Timezone
  EEPROM.begin(sizeof(config));
  EEPROM.get(0, config);
  EEPROM.end();

  WiFi.mode(WIFI_STA);
  WiFi.begin(config.ssid, config.password);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    timeClient.setTimeOffset(config.gmtOffsetMins * 60);
    timeClient.begin();
  } else {
    WiFi.softAP("ACME-SETUP");
  }

  _server.on("/",            HTTP_GET, std::bind(&MyWebServer::handleRoot,         this));
  _server.on("/set",         HTTP_GET, std::bind(&MyWebServer::handleSetMessage,   this));
  _server.on("/scan",        HTTP_GET, std::bind(&MyWebServer::handleScan,         this));
  _server.on("/connect",     HTTP_GET, std::bind(&MyWebServer::handleConnect,      this));
  _server.on("/playMelody",  HTTP_GET, std::bind(&MyWebServer::handlePlayMelody,   this));
  _server.on("/timer",       HTTP_GET, std::bind(&MyWebServer::handleTimer,        this));
  _server.on("/cancelTimer", HTTP_GET, std::bind(&MyWebServer::handleCancelTimer,  this));
  _server.on("/setTime",     HTTP_GET, std::bind(&MyWebServer::handleSetTime,      this));
  _server.on("/saveBot",     HTTP_GET, std::bind(&MyWebServer::handleSetTelegram,  this));
  _server.on("/updateFirmware", HTTP_GET, std::bind(&MyWebServer::handleUpdateFirmware, this));
  _server.begin();
}

void MyWebServer::handleClient() {
  _server.handleClient();
  if (timeClient.isTimeSet() == false) timeClient.update();
}

void MyWebServer::handleRoot() {
  String html = F(
    "<!DOCTYPE html><html><head>"
    "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>ACME Super Device</title>"
    "<link href='https://fonts.googleapis.com/css2?family=Luckiest+Guy&display=swap' rel='stylesheet'>"
    "<style>"
      "html,body{margin:0;padding:0;height:100%;font-family:Arial,Helvetica,sans-serif;"
      "background:#6b0000;color:#fff;text-align:center;display:flex;flex-direction:column;}"
      "body::before{content:'';position:fixed;inset:0;"
      "background:radial-gradient(circle,rgba(255,205,0,1)0%,rgba(255,51,0,1)40%,rgba(155,0,0,1)70%,rgba(107,0,0,1)100%);z-index:-1;}"
      "h1{font-family:'Luckiest Guy',cursive;font-size:2.4rem;margin:1rem 0;text-shadow:2px 2px #000;}"
      "section{width:90%;max-width:460px;margin:0 auto 1.4rem;background:rgba(0,0,0,.35);border-radius:12px;padding:1rem;}"
      "label{display:block;margin:.5rem 0 .2rem;text-align:left;}"
      "input,select,button{width:100%;padding:.6rem;font-size:1.05rem;margin:.25rem 0;border:none;border-radius:8px;box-sizing:border-box;}"
      "button,input[type=submit]{background:#ffcd00;color:#6b0000;font-weight:bold;}"
      "button:active,input[type=submit]:active{transform:scale(.97);}"
      "@media(min-width:500px){h1{font-size:3rem;}}"
      "#updateModal{display:none;position:fixed;z-index:1000;left:0;top:0;width:100%;height:100%;background:rgba(0,0,0,0.8);"
      "backdrop-filter:blur(3px);align-items:center;justify-content:center;flex-direction:column;color:#ffcd00;font-family:'Luckiest Guy',cursive;font-size:2rem;text-align:center;}"
      "@keyframes flicker{0%,18%,22%,25%,53%,57%,100%{opacity:1;}20%,24%,55%{opacity:0.4;}}"
      "#updateModal h2{animation:flicker 2s infinite;}"
    "</style></head><body>"

    "<h1>ACME Super Device</h1>"

    "<section><h2>Display</h2><form action='/set'>"
    "<label for='msg'>Scrolling Text</label><input id='msg' name='msg' placeholder='Enter text…' required>"
    "<input type='submit' value='Send to Display'></form></section>"

    "<section><h2>Wi-Fi</h2><button onclick=\"location.href='/scan'\">Scan Networks</button><form action='/connect'>"
    "<label for='ssid'>SSID</label><input id='ssid' name='ssid' placeholder='SSID' required>"
    "<label for='pass'>Password</label><input id='pass' name='pass' type='password' placeholder='Password' required>"
    "<input type='submit' value='Join Wi-Fi'></form></section>"

    "<section><h2>Tunes</h2><form action='/playMelody'>"
    "<label for='mel'>Pick a Tune</label><select id='mel' name='melody'>"
    "<option value='nokia'>Tune 01</option><option value='mario'>Tune 02</option><option value='simpsons'>Tune 03</option><option value='ice'>Tune 04</option>"
    "<option value='rick'>Tune 05</option><option value='starwars'>Tune 06</option><option value='harry'>Tune 07</option>"
    "</select><input type='submit' value='Play Tune'></form></section>"

    "<section><h2>Timer</h2><form action='/timer'>"
    "<label for='sec'>Seconds</label><input id='sec' type='number' name='sec' placeholder='e.g. 30' min='1' required>"
    "<input type='submit' value='Start Timer'></form>"
    "<button onclick=\"location.href='/cancelTimer'\">Stop Timer</button></section>"

    "<section><h2>Clock</h2><form action='/setTime'>"
    "<label for='tz'>UTC offset (min)</label><input id='tz' type='number' name='tz' placeholder='600 = +10:00' required>"
    "<label><input type='checkbox' name='dst'> DST</label>"
    "<input type='submit' value='Save Time'></form></section>"

    "<section><h2>Telegram Bot</h2><form action='/saveBot'>"
    "<label for='token'>Bot Token</label><input id='token' name='token' placeholder='token' required>"
    "<label for='chat'>Chat ID</label><input id='chat' name='chat' placeholder='chat ID' required>"
    "<input type='submit' value='Save & Reload Bot'></form></section>"

    "<section><h2>OTA Update</h2><form action='/updateFirmware' method='get' onsubmit='showUpdateModal()'>"
    "<input type='submit' value='Update Firmware'></form></section>"

    "<div id='updateModal' style='display:none;'><h2>Updating Firmware…<br>Device will reboot.<br>Returning to Home shortly…</h2></div>"

    "<footer style='margin:1rem auto;font-size:.8rem;opacity:.6'>© ACME Labs 2025 – That’s all folks!</footer>"

    "<script>function showUpdateModal(){document.getElementById('updateModal').style.display='flex';setTimeout(function(){location.href='/'},60000);}</script>"

    "</body></html>"
  );

  _server.send(200, "text/html", html);
}

void MyWebServer::handleSetMessage() { /* same */ }
void MyWebServer::handleScan() { /* same */ }
void MyWebServer::handleConnect() { /* same */ }
void MyWebServer::handleTimer() { /* same */ }
void MyWebServer::handleCancelTimer() { /* same */ }
void MyWebServer::handleSetTime() { /* same */ }
void MyWebServer::handleSetTelegram() { /* same */ }

void MyWebServer::handlePlayMelody() {
  if (!_server.hasArg("melody")) {
    _server.send(400, "text/plain", "error");
    return;
  }
  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");
  String m = _server.arg("melody");
  String label = "TUNE";

  tones.playTone8();

  if      (m == "nokia")    { melodies.playNokia();    label = "TUNE01"; }
  else if (m == "mario")    { melodies.playMario();    label = "TUNE02"; }
  else if (m == "simpsons") { melodies.playSimpsons(); label = "TUNE03"; }
  else if (m == "ice")      { melodies.playIce();      label = "TUNE04"; }
  else if (m == "rick")     { melodies.playRick();     label = "TUNE05"; }
  else if (m == "starwars") { melodies.playStarWars(); label = "TUNE06"; }
  else                      { melodies.playHarry();    label = "TUNE07"; }

  if (_scrollMessage) *_scrollMessage = label;
  _newMessage = label;
  _newMessageFlag = true;
}

void MyWebServer::handleUpdateFirmware() {
  _server.send(200, "text/html", "<!DOCTYPE html><html><head>"
                                 "<meta charset='UTF-8'>"
                                 "<meta name='viewport' content='width=device-width,initial-scale=1'>"
                                 "<title>Firmware Update</title>"
                                 "<style>body{background:black;color:yellow;font-family:Arial,sans-serif;text-align:center;padding-top:50px;}</style>"
                                 "</head><body><h1>Updating Firmware...</h1>"
                                 "<p>Device will reboot shortly.<br><br>"
                                 "<a href='/'>Return Home</a> if not redirected.</p>"
                                 "</body></html>");
  
  delay(150);

  WiFiClientSecure client;
  client.setInsecure();
  ESPhttpUpdate.rebootOnUpdate(true);
  ESPhttpUpdate.update(client, "https://raw.githubusercontent.com/Badkirked/boom/main/firmware.bin");
}
