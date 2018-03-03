ESP8266WebServer server(80);

void serverHandleRoot() {
  String response = "id=" WIFI_AP_NAME;
  response += chipId;
  response += "\nmac=";
  response += WiFi.macAddress();
  response += "\ntemp=";
  response += tempCurrent;
  response += "\ntempAge=";
  response += tempGetAge();
  response += "\n";

  server.send(200, "text/plain", response);
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

void serverSetup() {
  server.on("/", serverHandleRoot);
  server.on("/temp", serverGetTemp);
  server.on("/tempRaw", serverGetTempRaw);
  server.begin();
}

