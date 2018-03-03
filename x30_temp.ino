Task tempTask(1, TASK_FOREVER, &tempConvertCb, &scheduler, true);

OneWire tempWire(PIN_TEMP);
float tempCurrent;
uint32_t tempTime;

void tempSetup() {
  tempCurrent = 0;
  tempTime = 0;
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
      tempCurrent = (float)raw / 16.0;
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
