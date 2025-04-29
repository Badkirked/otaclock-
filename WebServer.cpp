// === FILE: WebServer.cpp ===

#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include "WebServer.h"
#include "MemoryManager.h"
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

MyWebServer::MyWebServer(uint16_t port)
  : _server(port),
    _scrollMessage(nullptr),
    _newMessageFlag(false),
    _newMessage("") {}

void MyWebServer::setScrollMessage(String *msg) { _scrollMessage = msg; }

void MyWebServer::begin() {
  _server.on("/",             HTTP_GET, std::bind(&MyWebServer::handleRoot,          this));
  _server.on("/set",           HTTP_GET, std::bind(&MyWebServer::handleSetMessage,    this));
  _server.on("/scan",          HTTP_GET, std::bind(&MyWebServer::handleScan,          this));
  _server.on("/connect",       HTTP_GET, std::bind(&MyWebServer::handleConnect,       this));
  _server.on("/playMelody",    HTTP_GET, std::bind(&MyWebServer::handlePlayMelody,    this));
  _server.on("/timer",         HTTP_GET, std::bind(&MyWebServer::handleTimer,         this));
  _server.on("/cancelTimer",   HTTP_GET, std::bind(&MyWebServer::handleCancelTimer,   this));
  _server.on("/setTime",       HTTP_GET, std::bind(&MyWebServer::handleSetTime,       this));
  _server.on("/saveBot",       HTTP_GET, std::bind(&MyWebServer::handleSetTelegram,   this));
  _server.on("/updateFirmware",HTTP_GET, std::bind(&MyWebServer::handleUpdateFirmware,this));
  _server.begin();
}

void MyWebServer::handleClient() { _server.handleClient(); }

void MyWebServer::handleRoot() {
  String html = F(
    "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>ACME Super Device</title>"
    "<link href='https://fonts.googleapis.com/css2?family=Luckiest+Guy&display=swap' rel='stylesheet'>"
    "<style>"
    "html,body{margin:0;padding:0;height:100%;font-family:Arial,Helvetica,sans-serif;background:#6b0000;color:#fff;text-align:center;display:flex;flex-direction:column;}"
    "body::before{content:'';position:fixed;inset:0;background:radial-gradient(circle,rgba(255,205,0,1)0%,rgba(255,51,0,1)40%,rgba(155,0,0,1)70%,rgba(107,0,0,1)100%);z-index:-1;}"
    "h1{font-family:'Luckiest Guy',cursive;font-size:2.4rem;margin:1rem 0;text-shadow:2px 2px #000;}"
    "section{width:90%;max-width:460px;margin:0 auto 1.4rem;background:rgba(0,0,0,.35);border-radius:12px;padding:1rem;}"
    "label{display:block;margin:.5rem 0 .2rem;text-align:left;}"
    "input,select,button{width:100%;padding:.6rem;font-size:1.05rem;margin:.25rem 0;border:none;border-radius:8px;box-sizing:border-box;}"
    "button,input[type=submit]{background:#ffcd00;color:#6b0000;font-weight:bold;}"
    "button:active,input[type=submit]:active{transform:scale(.97);}"
    "@media(min-width:500px){h1{font-size:3rem;}}"
    "</style></head><body>"
    "<h1>ACME Super Device</h1>"

    "<section><h2>Display</h2><form action='/set'><label for='msg'>Scrolling Text</label><input id='msg' name='msg' placeholder='Enter text…' required><input type='submit' value='Send to Display'></form></section>"

    "<section><h2>Wi-Fi</h2><button onclick=\"location.href='/scan'\">Scan Networks</button><form action='/connect'><label for='ssid'>SSID</label><input id='ssid' name='ssid' placeholder='SSID' required><label for='pass'>Password</label><input id='pass' name='pass' type='password' placeholder='Password' required><input type='submit' value='Join Wi-Fi'></form></section>"

    "<section><h2>Tunes</h2><form action='/playMelody'><label for='mel'>Pick a Tune</label><select id='mel' name='melody'>"
    "<option value='nokia'>Tune 01</option><option value='mario'>Tune 02</option><option value='simpsons'>Tune 03</option><option value='ice'>Tune 04</option><option value='rick'>Tune 05</option><option value='starwars'>Tune 06</option><option value='harry'>Tune 07</option>"
    "</select><input type='submit' value='Play Tune'></form></section>"

    "<section><h2>Timer</h2><form action='/timer'><label for='sec'>Seconds</label><input id='sec' type='number' name='sec' placeholder='e.g. 30' min='1' required><input type='submit' value='Start Timer'></form><button onclick=\"location.href='/cancelTimer'\">Stop Timer</button></section>"

    "<section><h2>Clock</h2><form action='/setTime'><label for='tz'>UTC offset (min)</label><input id='tz' type='number' name='tz' placeholder='600 = +10:00' required><label><input type='checkbox' name='dst'> DST</label><input type='submit' value='Save Time'></form></section>"

    "<section><h2>Telegram Bot</h2><form action='/saveBot'><label for='token'>Bot Token</label><input id='token' name='token' placeholder='token' required><label for='chat'>Chat ID</label><input id='chat' name='chat' placeholder='chat ID' required><input type='submit' value='Save & Reload Bot'></form></section>"

    "<section><h2>OTA Update</h2><form action='/updateFirmware'><input type='submit' value='Update Firmware'></form></section>"

    "<footer style='margin:1rem auto;font-size:.8rem;opacity:.6'>© ACME Labs 2025 – That’s all folks!</footer>"

    "</body></html>"
  );

  _server.send(200, "text/html", html);
}

void MyWebServer::handleSetMessage() {
  if (!_server.hasArg("msg") || !_scrollMessage) {
    _server.send(400, "text/plain", "error");
    return;
  }
  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");

  *_scrollMessage = _server.arg("msg");
  _newMessage = _server.arg("msg");
  _newMessageFlag = true;
}

bool MyWebServer::hasNewMessage() { bool f = _newMessageFlag; _newMessageFlag = false; return f; }
String MyWebServer::getNewMessage() { return _newMessage; }

void MyWebServer::handleScan() {
  int n = WiFi.scanNetworks();
  String html = F("<html><body><h2>Wi-Fi Networks</h2><form action='/connect'><select name='ssid'>");
  for (int i = 0; i < n; ++i) html += "<option>" + WiFi.SSID(i) + "</option>";
  html += F("</select><br><input name='pass' type='password' placeholder='pass'><input type='submit' value='Join'></form><p><a href='/'>← Back</a></p></body></html>");
  _server.send(200, "text/html", html);
}

void MyWebServer::handleConnect() {
  if (!_server.hasArg("ssid") || !_server.hasArg("pass")) {
    _server.send(400, "text/plain", "error");
    return;
  }
  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");

  WiFi.mode(WIFI_STA);
  WiFi.begin(_server.arg("ssid").c_str(), _server.arg("pass").c_str());
  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) delay(250);

  ACMEConfig cfg; MemoryManager::loadConfig(cfg);
  if (WiFi.status() == WL_CONNECTED) {
    if (_scrollMessage) *_scrollMessage = String("IP ") + WiFi.localIP().toString();
    tones.playTone7();
    strncpy(cfg.ssid, _server.arg("ssid").c_str(), sizeof(cfg.ssid));
    strncpy(cfg.password, _server.arg("pass").c_str(), sizeof(cfg.password));
  } else {
    tones.playTone10();
  }
  MemoryManager::saveConfig(cfg);
}

void MyWebServer::handlePlayMelody() {
  if (!_server.hasArg("melody")) { _server.send(400, "text/plain", "error"); return; }

  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");

  String m = _server.arg("melody");
  String label = "TUNE";

  tones.playTone8();
  if      (m=="nokia")    { melodies.playNokia();    label="TUNE01"; }
  else if (m=="mario")    { melodies.playMario();    label="TUNE02"; }
  else if (m=="simpsons") { melodies.playSimpsons(); label="TUNE03"; }
  else if (m=="ice")      { melodies.playIce();      label="TUNE04"; }
  else if (m=="rick")     { melodies.playRick();     label="TUNE05"; }
  else if (m=="starwars") { melodies.playStarWars(); label="TUNE06"; }
  else                    { melodies.playHarry();    label="TUNE07"; }

  if (_scrollMessage) *_scrollMessage = label;
  _newMessage = label;
  _newMessageFlag = true;
}

void MyWebServer::handleTimer() {
  if (!_server.hasArg("sec")) { _server.send(400, "text/plain", "error"); return; }
  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");

  int s = _server.arg("sec").toInt();
  if (s > 0) { myTimer.setCountdown(s); timerMode = true; clockMode = false; }
}

void MyWebServer::handleCancelTimer() {
  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");

  myTimer.setCountdown(0);
  timerMode = false;
  clockMode = true;
}

void MyWebServer::handleSetTime() {
  if (!_server.hasArg("tz")) { _server.send(400, "text/plain", "error"); return; }
  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");

  ACMEConfig cfg; MemoryManager::loadConfig(cfg);
  cfg.gmtOffsetMins = _server.arg("tz").toInt();
  cfg.dstEnabled = _server.hasArg("dst");
  MemoryManager::saveConfig(cfg);

  myClock.begin("pool.ntp.org", cfg.gmtOffsetMins * 60, cfg.dstEnabled ? 3600 : 0);
  myClock.update();
}

void MyWebServer::handleSetTelegram() {
  if (!_server.hasArg("token") || !_server.hasArg("chat")) { _server.send(400, "text/plain", "error"); return; }
  _server.sendHeader("Location", "/");
  _server.send(302, "text/plain", "");

  ACMEConfig cfg; MemoryManager::loadConfig(cfg);
  strncpy(cfg.botToken, _server.arg("token").c_str(), sizeof(cfg.botToken));
  strncpy(cfg.chatID, _server.arg("chat").c_str(), sizeof(cfg.chatID));
  MemoryManager::saveConfig(cfg);

  reinitTelegram = true;
  if (_scrollMessage) *_scrollMessage = "BOT SAVED";
  tones.playTone7();
}

void MyWebServer::handleUpdateFirmware() {
  String page = "<!DOCTYPE html><html><head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' content='width=device-width,initial-scale=1'>"
                "<title>Firmware Update</title>"
                "<style>body{background:black;color:yellow;font-family:monospace;text-align:center;padding-top:60px;font-size:22px;}</style>"
                "</head><body>"
                "<h1>Updating Firmware...</h1>"
                "<p>Device will reboot in <span id='countdown'>60</span> seconds.</p>"
                "<p><a href='/'>Return Home</a></p>"
                "<script>"
                "let seconds = 60;"
                "let countdown = document.getElementById('countdown');"
                "setInterval(function() {"
                "  seconds--;"
                "  if (seconds >= 0) countdown.textContent = seconds;"
                "}, 1000);"
                "setTimeout(function(){ location.href='/' }, 60000);"
                "</script>"
                "</body></html>";

  _server.send(200, "text/html", page);

  delay(600);

  WiFiClientSecure client;
  client.setInsecure();
  ESPhttpUpdate.rebootOnUpdate(true);
  ESPhttpUpdate.update(client, "https://raw.githubusercontent.com/Badkirked/boom/main/firmware.bin");
}
