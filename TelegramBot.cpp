
#include "TelegramBot.h"

/* ------------------------------------------------------------------
   Constructor
   ------------------------------------------------------------------
   • disables TLS certificate checking (setInsecure)
   • sets a 2 s network timeout (helps boot speed)
   • configures small RX/TX buffers (saves RAM)
------------------------------------------------------------------*/
TelegramBot::TelegramBot(const String& token, const String& chatID)
: _token(token), _chatID(chatID)
{
    _client.setInsecure();          // skip CA validation
    _client.setTimeout(2000);       // abort if no reply in 2 s
    _client.setBufferSizes(512, 512);
    /* setReuse(true) is available only in newer cores;
       left out for ESP8266 core 3.1.2 compatibility */
}

/* ================================================================
   send()  –  POST /sendMessage
   ================================================================*/
bool TelegramBot::send(const String& text)
{
    HTTPClient http;
    if (!beginRequest(http, F("/sendMessage"))) return false;

    String payload = "chat_id=" + _chatID +
                     "&text="   + urlencode(text);

    http.addHeader("Content-Type",
                   "application/x-www-form-urlencoded");
    bool ok = (http.POST(payload) == 200);
    http.end();
    return ok;
}

/* ================================================================
   poll()  –  long‑poll /getUpdates for next text message
   ================================================================*/
bool TelegramBot::poll(String& msg)
{
    HTTPClient http;
    String path = "/getUpdates?timeout=0"
                  "&allowed_updates=message"
                  "&offset=" + String(_lastUpdate + 1);

    if (!beginRequest(http, path)) return false;
    if (http.GET() != 200)         { http.end(); return false; }

    String body = http.getString();
    http.end();

    int upPos  = body.indexOf("\"update_id\":");
    int txtPos = body.indexOf("\"text\":\"");
    if (upPos < 0 || txtPos < 0) return false;

    _lastUpdate = body.substring(upPos + 12).toInt();

    int start = txtPos + 8;
    int end   = body.indexOf('"', start);
    if (end < 0) return false;

    msg = body.substring(start, end);
    msg.replace("\\/", "/");
    msg.replace("\\n", "\n");
    msg.replace("\\\"", "\"");
    return true;
}

/* ================================================================
   helpers
   ================================================================*/
bool TelegramBot::beginRequest(HTTPClient& http,
                               const String& path)
{
    String url = "https://api.telegram.org/bot" + _token + path;
    return http.begin(_client, url);
}

/* percent‑encode for POST bodies */
String TelegramBot::urlencode(const String& s)
{
    String out;
    for (char c : s) {
        if (isalnum(c) || c=='-'||c=='_'||c=='.'||c=='~') out += c;
        else {
            char buf[4]; sprintf(buf,"%%%02X",(unsigned char)c);
            out += buf;
        }
    }
    return out;
}



