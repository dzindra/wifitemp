
void setup() {
  Serial.begin(115200);
  logInfo("\n\nStarting WiFiTemp");
  logValue("chipId: ", chipId);

  EEPROM.begin(512);

  tempSetup();
  wifiConnectSta();
  serverSetup();
}

void loop() {
  scheduler.execute();
  server.handleClient();
}
