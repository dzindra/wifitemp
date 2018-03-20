
void setup() {
  Serial.begin(115200);
  logInfo("\n\nStarting WiFiTemp");
  logValue("chipId: ", chipId);

  EEPROM.begin(512);

  tempSetup();
  wifiConnectSta();
  serverSetup();

  String hn = WIFI_AP_NAME;
  hn += chipId;
  ArduinoOTA.setHostname(hn.c_str());
  ArduinoOTA.onStart([]() {
    logInfo("OTA starting");
  });
  ArduinoOTA.onEnd([]() {
    logInfo("OTA finished");
  });
  ArduinoOTA.begin();
}

void loop() {
  scheduler.execute();
  server.handleClient();
  ArduinoOTA.handle();
}
