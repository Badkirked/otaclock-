
/* =================================================================
   ACME™ All-Purpose Countdown Gizmo (dynamic Telegram creds, OTA)
   ---------------------------------------------------------------
   • EEPROM-stored SSID/Pass, timezone & Telegram token/chat  
   • TelegramBot instantiated only if creds exist  
   • “Save & Reload Bot” on Web UI recreates the bot at runtime  
   • OTA support via ArduinoOTA
   ================================================================*/

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <time.h>                     // local getLocalTime shim
#include "APManager.h"
#include "WebServer.h"                // declaration only – impl. in WebServer.cpp
#include <TM1637Display.h>
#include "TM1637Scroller.h"
#include "ToneLanguage.h"
#include "Melodies.h"
#include "MemoryManager.h"
#include "MyClock.h"
#include "ButtonController.h"
#include "Timer.h"
#include "BallSensor.h"
#include "TimerFinishSequence.h"
#include "TelegramBot.h"
#include "ScrollerMessages.h"         // one-line scroll helper

// ─── getLocalTime shim ─────────────────────────────────────────────
static bool getLocalTime(struct tm *info) {
  if (!info) return false;
  time_t now = time(nullptr);
  if (now == 0) return false;
  localtime_r(&now, info);
  return true;
}

// ─── TelegramBot ptr & reload flag ─────────────────────────────────
TelegramBot *tg             = nullptr;
bool         reinitTelegram = false;

// ─── Hardware pins ─────────────────────────────────────────────────
constexpr uint8_t PIEZO_PIN  = D2;
constexpr uint8_t CLK_PIN    = D4;
constexpr uint8_t DIO_PIN    = D5;
constexpr uint8_t BUTTON_PIN = D1;
constexpr uint8_t BALL_PIN   = D6;

// ─── Mode & state flags ────────────────────────────────────────────
bool clockMode     = true;
bool timerMode     = false;
bool sensorArmed   = true;
bool colonVisible  = false;
bool bootScrolling = true;
bool telegramReady = false;

// ─── Scrolling buffers & timing ───────────────────────────────────
String       bannerMsg = "ENTER 192 168 4 1 ON PHONE OR PC TO CONNECT ";// initial banner
String       tgScrollBuf;                      // for Telegram messages
constexpr uint16_t SCROLL_MS = 130;
constexpr uint16_t FAST_MS   = 100;

// ─── Millis-based timers ───────────────────────────────────────────
unsigned long tBlink     = 0;
unsigned long tClockUpd  = 0;
unsigned long tClockLive = 0;
unsigned long tTgPoll    = 0;
unsigned long tNoMove    = 0;
unsigned long tAlarmSend = 0;

constexpr unsigned TG_POLL_MS     = 7000;
constexpr unsigned NO_MOVE_MS     = 2000;
constexpr unsigned TG_DEFER_MS    = 5000;
constexpr unsigned ALARM_DELAY_MS = 25000;

// ─── Deferred alarm state ──────────────────────────────────────────
bool  alarmPending   = false;
char  alarmTimeStr[10] = "--:--:--";

// ─── Wi-Fi helpers ─────────────────────────────────────────────────
bool station   = false;
int  safeState = HIGH;

// ─── Core objects (match WebServer.cpp externs) ────────────────────
TM1637Display      display(CLK_PIN, DIO_PIN);
ToneLanguage       tones(PIEZO_PIN);
Melodies           melodies(PIEZO_PIN);
MyWebServer        web(80);
TM1637Scroller     scroller(&display);
MyClock            myClock(&display);
Timer              myTimer(&display);
TimerFinishSequence finishSequence(&display, &tones);
ButtonController   btn(BUTTON_PIN, &tones, &melodies, &myTimer,
                       &clockMode, &timerMode);
BallSensor         ball(BALL_PIN);

// ─── Show incoming Telegram on the scroll ─────────────────────────
void showTelegramText(const String &txt) {
  display.setBrightness(0x0F);
  tgScrollBuf = "    " + txt + "    ";
  if (tg) tg->send("📟 Display queued");
  tones.playTone7();
  clockMode = false;
  scroller.setSpeed(FAST_MS);
  scroller.startScroll(tgScrollBuf.c_str());
}

// ─── Safe Telegram polling ─────────────────────────────────────────
bool pollTelegram(String &out) {
  if (!tg || !telegramReady || timerMode || scroller.isScrolling()) return false;
  if (millis() - tTgPoll < TG_POLL_MS) return false;
  tTgPoll = millis();
  return tg->poll(out);
}

// ==================================================================
//                                setup
// ==================================================================
void setup() {
  Serial.begin(115200);
  delay(200);

  // Buzzer + display ready jingle
  tones.begin(115200);
  tones.playTone8();
  display.setBrightness(0x0F);

  // Button & timer init
  btn.init();
  myTimer.begin();

  // Scroller speed
  scroller.setSpeed(SCROLL_MS);

  // Ball sensor
  ball.begin();
  ball.update();
  safeState = ball.getState();

  // Load or reset config
  MemoryManager::begin();
  ACMEConfig cfg;
  if (!MemoryManager::loadConfig(cfg)) {
    MemoryManager::resetDefaults(cfg);
  }

  // TelegramBot if creds present
  if (cfg.botToken[0] && cfg.chatID[0]) {
    tg = new TelegramBot(cfg.botToken, cfg.chatID);
  }

  // Wi-Fi / AP
  if (cfg.ssid[0]) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(cfg.ssid, cfg.password);
    unsigned long t = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t < 8000) delay(250);
    if (WiFi.status() == WL_CONNECTED) {
      station   = true;
      bannerMsg = String("CONNECTION SUCCESSFUL IP IS ") + WiFi.localIP().toString();
      tones.playTone7();
    }
  }
  if (!station) {
    APManager::begin("acme");
    bannerMsg = "ACME AP:" + APManager::getAPIP();
  }

  // —— OTA SETUP —— (must be after Wi-Fi connect)
  ArduinoOTA.setHostname("acme-device");
  ArduinoOTA.setPassword("d43f8d6354");

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA End");
  });
  ArduinoOTA.onProgress([](unsigned int prog, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (prog * 100) / total);
  });
  ArduinoOTA.onError([](ota_error_t e) {
    Serial.printf("OTA Error[%u]: ", e);
    if      (e == OTA_AUTH_ERROR)    Serial.println("Auth Failed");
    else if (e == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
    else if (e == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (e == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (e == OTA_END_ERROR)     Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  // Web UI & initial banner scroll
  web.setScrollMessage(&bannerMsg);
  web.begin();
  scroller.startScroll(bannerMsg.c_str());

  // NTP clock init
  myClock.begin("pool.ntp.org",
                cfg.gmtOffsetMins * 60,
                cfg.dstEnabled ? 3600 : 0);
  myClock.update();
  tClockUpd = millis();
}

// ==================================================================
//                                 loop
// ==================================================================
void loop() {
  // Handle OTA on every pass
  ArduinoOTA.handle();

  // 1) Button & Telegram reload
  btn.update();
  if (reinitTelegram) {
    reinitTelegram = false;
    if (tg) { delete tg; tg = nullptr; }
    ACMEConfig cfg2;
    MemoryManager::loadConfig(cfg2);
    if (cfg2.botToken[0] && cfg2.chatID[0]) {
      tg = new TelegramBot(cfg2.botToken, cfg2.chatID);
    }
  }
  ButtonAction act = btn.lastAction();

  // 2) Boot scrolling
  if (bootScrolling) {
    scroller.updateScroll();
    if (!scroller.isScrolling()) {
      bootScrolling = false;
      tClockLive    = millis();
    }
    return;
  }

  // 3) Cancel scroll on START tap
  if (act == START && !clockMode) {
    scroller.startScroll("");
    tgScrollBuf.clear();
    display.clear();
    clockMode = true;
  }

  // 4) Enable Telegram after delay
  if (!telegramReady && millis() - tClockLive >= TG_DEFER_MS) {
    telegramReady = true;
    if (tg && station) tg->send("🟢 ACME Online");
    tTgPoll = millis();
  }

  // 5) Deferred alarm send
  if (alarmPending && telegramReady && millis() >= tAlarmSend &&
      !scroller.isScrolling() && !timerMode && tg) {
    tg->send(String("💥 Alarm triggered at ") + alarmTimeStr);
    alarmPending = false;
  }

  // 6) Poll Telegram
  String tmsg;
  if (pollTelegram(tmsg)) {
    if      (tmsg == "/arm")    { sensorArmed = true;  tg->send("🔒 Armed");   }
    else if (tmsg == "/disarm") { sensorArmed = false; tg->send("🔓 Disarmed"); }
    else if (tmsg == "/status") {
      String s = String("Status: ") + (sensorArmed ? "Armed" : "Disarmed");
      if (station) s += "\nIP:" + WiFi.localIP().toString();
      tg->send(s);
    } else {
      showTelegramText(tmsg);
    }
  }

  // 7) Ball sensor + timer logic
  ball.update();
  int cur = ball.getState();
  if (sensorArmed) {
    if (cur != safeState && !timerMode) {
      myTimer.setCountdown(20);
      timerMode  = true;
      clockMode  = false;
      struct tm info;
      if (getLocalTime(&info))
        strftime(alarmTimeStr, sizeof(alarmTimeStr), "%H:%M:%S", &info);
      alarmPending = true;
      tAlarmSend   = millis() + ALARM_DELAY_MS;
      scrollMsg("T-20");
    }
    if (cur == safeState && timerMode && millis() - tNoMove >= NO_MOVE_MS) {
      myTimer.setCountdown(0);
      timerMode = false;
      clockMode = true;
      scrollMsg(TXT_DONE);
    }
    if (cur != safeState) tNoMove = millis();
  }

  // 8) Timer mode beeps & finish
  if (timerMode) {
    myTimer.update();
    if (millis() - tBlink >= 1000) { tones.playTone1(); tBlink = millis(); }
    if (myTimer.isFinished()) {
      finishSequence.run();
      scrollMsg(TXT_DONE);
      timerMode = false;
      clockMode = true;
    }
    return;
  }

  // 9) Restart Telegram scroll if needed
  if (!clockMode && !scroller.isScrolling() && !tgScrollBuf.isEmpty()) {
    scroller.startScroll(tgScrollBuf.c_str());
  }

  // 10) Web UI
  web.handleClient();

  // 11) Clock display
  if (clockMode && !scroller.isScrolling()) {
    unsigned long now = millis();
    if (now - tBlink >= 1000) { colonVisible = !colonVisible; tBlink = now; }
    myClock.showTime(colonVisible);
    if (now - tClockUpd >= 60000) { myClock.update(); tClockUpd = now; }
  }

  // 12) Scroll updates
  scroller.updateScroll();
}



