
#include "WiFiScanner.h"

String WiFiScanner::scanNetworks() {
  int n = WiFi.scanNetworks();
  String result = "";
  if (n == 0) {
    result = "No networks found";
  } else {
    result = "Found: ";
    for (int i = 0; i < n; i++) {
      result += WiFi.SSID(i);
      if (i < n - 1) {
        result += ", ";
      }
    }
  }
  return result;
}



