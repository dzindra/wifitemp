
void wifiConnectSta() {
  WiFiManager wifiManager;
  String ap = WIFI_AP_NAME;
  ap += chipId;
  if (!wifiManager.autoConnect(ap.c_str())) {
    logInfo("Failed to connect or setup, rebooting...");
    delay(1000);
    ESP.reset();
    while (1) delay(1);
  }
}

