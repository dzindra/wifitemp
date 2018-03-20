ESP8266WebServer server(80);

#define RESPONSE_START() String __response = ""
#define RESPONSE_ADD(_key,_value) __response += _key "="; __response += _value; __response += "\n"
#define RESPONSE_END(_code,_content) server.send(_code, _content, __response)

void serverHandleRoot() {
  RESPONSE_START();
  RESPONSE_ADD("id", chipId);
  RESPONSE_ADD("mac", WiFi.macAddress());
  RESPONSE_ADD("temp", tempCurrent);
  RESPONSE_ADD("tempOrig", tempUnadjusted);
  RESPONSE_ADD("tempAge", tempGetAge());
  RESPONSE_ADD("tempA", tempConfig.a);
  RESPONSE_ADD("tempB", tempConfig.b);
  RESPONSE_END(200, "text/plain");
}

void serverGetTemp() {
  String response = "{\"temperature\":";
  response += tempCurrent;
  response += ",\"age\":";
  response += tempGetAge();
  response += "}\n";
  server.send(200, "application/json", response);
}

void serverGetTempRaw() {
  String response = "";
  response += tempCurrent;
  server.send(200, "text/plain", response);
}

void serverSetConfig() {
  if (!server.hasArg("a") || !server.hasArg("b")) {
    server.send(400, "text/plain", "Missing a and b params.\n");
    return;
  }

  long aInt = server.arg("a").toInt();
  long bInt = server.arg("b").toInt();

  if (aInt == 0) {
    server.send(400, "text/plain", "Param a shouldn't be zero.\n");
    return;
  }

  float a = (float)aInt / 1000.0;
  float b = (float)bInt / 1000.0;

  tempSetConfig(a, b);

  RESPONSE_START();
  RESPONSE_ADD("tempA", tempConfig.a);
  RESPONSE_ADD("tempB", tempConfig.b);
  RESPONSE_END(200, "text/plain");
}

void serverSetup() {
  server.on("/", serverHandleRoot);
  server.on("/temp", serverGetTemp);
  server.on("/tempRaw", serverGetTempRaw);
  server.on("/setConfig", serverSetConfig);
  server.begin();
}
