/*
 * KT_BQ25601 Arduino Library
 * Copyright (C) 2026 Kacper Tora
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 */

#include <Arduino.h>
#include <Wire.h>
#include "KT_BQ25601.h"

// Your I2C pins
#if defined(ESP32)
  #define I2C_SDA 8
  #define I2C_SCL 9
#endif

// Only for reference, Li-Po 3.7 V
#define BATTERY_CAPACITY_MAH 800

KT_BQ25601 Charger;

void setup(){
    Serial.begin(115200);
    delay(1000);

    #if defined(ESP32)
        Wire.begin(I2C_SDA, I2C_SCL);
    #else
        Wire.begin();
    #endif

    while (!Charger.begin(&Wire)) {
        Serial.println("Couldnt find BQ25601.");
        delay(2000);
    }
    Serial.println("BQ25601 initialized successfully.");
    Serial.print("Part Number: 0x");
    Serial.println(Charger.getPartNumber(), HEX);
    Serial.print("Device Revision: 0x");
    Serial.println(Charger.getDeviceRevision(), HEX);

    // 0.5C = 400 mA
    Charger.setChargeCurrent(400);
    Charger.setPrechargeCurrent(60);
    Charger.setTerminationCurrent(60);
    Charger.setInputCurrentLimit(500);
    Charger.setChargeVoltage(4208);

    uint16_t inputCurrentLimit = Charger.getInputCurrentLimit();
    uint16_t chargeVoltage = Charger.getChargeVoltage();
    uint16_t chargeCurrent = Charger.getChargeCurrent();
    uint16_t preChargeCurrent = Charger.getPrechargeCurrent();
    uint16_t terminationCurrent = Charger.getTerminationCurrent();

    Serial.print(F("Input Current Limit: "));
    Serial.print(inputCurrentLimit);
    Serial.println(F(" mA"));

    Serial.print(F("Charge Voltage: "));
    Serial.print(chargeVoltage);
    Serial.println(F(" mV"));

    Serial.print(F("Charge Current: "));
    Serial.print(chargeCurrent);
    Serial.println(F(" mA"));

    Serial.print(F("Precharge Current: "));
    Serial.print(preChargeCurrent);
    Serial.println(F(" mA"));

    Serial.print(F("Termination Current: "));
    Serial.print(terminationCurrent);
    Serial.println(F(" mA"));
}

void loop(){}