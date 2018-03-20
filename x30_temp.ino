Task tempTask(1, TASK_FOREVER, &tempConvertCb, &scheduler, true);

typedef struct {
  uint8_t magic;
  float a;
  float b;
} TempConfig;

OneWire tempWire(PIN_TEMP);
float tempCurrent;
float tempUnadjusted;
TempConfig tempConfig;
uint32_t tempTime;

#define TEMP_MAGIC 0x5e

void tempSetup() {
  tempCurrent = 0;
  tempUnadjusted = 0;
  tempTime = 0;

  EEPROM.get(0, tempConfig);
  if (tempConfig.magic == TEMP_MAGIC) {
    logInfo("Got temp config");
    logValue("  a: ", tempConfig.a);
    logValue("  b: ", tempConfig.b);
  } else {
    logInfo("Invalid temp config, resetting");

    tempConfig.magic = TEMP_MAGIC;
    tempConfig.a = 1;
    tempConfig.b = 0;
  }
}

void tempConvertCb() {
  tempWire.reset();
  tempWire.skip();
  tempWire.write(0x44);

  tempTask.setInterval(1000);
  tempTask.setCallback(&tempReadCb);
}

uint32_t tempGetAge() {
  return tempTime == 0 ? 0 : millis() - tempTime;
}

void tempCalcCurrent() {
  tempCurrent = tempUnadjusted * tempConfig.a + tempConfig.b;
}

void tempSetConfig(float a, float b) {
  tempConfig.a = a;
  tempConfig.b = b;
  tempConfig.magic = TEMP_MAGIC;
  EEPROM.put(0, tempConfig);
  EEPROM.commit();
  tempCalcCurrent();
  logInfo("Saved temp config");
  logValue("  a: ", tempConfig.a);
  logValue("  b: ", tempConfig.b);
}

// reading works only when sensor is set to 12bit precision
void tempReadCb() {
  if (tempWire.reset()) {
    tempWire.skip();
    tempWire.write(0xBE);

    uint8_t data[9];
    for (uint8_t i = 0; i < 9; i++) {
      data[i] = tempWire.read();
    }

    if (data[8] == OneWire::crc8(data, 8)) {
      int16_t raw = (data[1] << 8) | data[0];
      tempUnadjusted = (float)raw / 16.0;
      tempCalcCurrent();
      tempTime = millis();
      tempTask.setInterval(30000);

      logValue("Got temp: ", tempCurrent);
    } else {
      logInfo("CRC mismatch");
    }
  } else {
    logInfo("Unable to reset bus");
  }
  tempTask.setCallback(&tempConvertCb);
}
