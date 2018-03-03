
void setup() {
  Serial.begin(115200);

  tempSetup();
  wifiConnectSta();
  serverSetup();
}

void loop() {
  scheduler.execute();
  server.handleClient();
}
