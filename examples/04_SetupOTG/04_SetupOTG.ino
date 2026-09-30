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
#include <KT_BQ25601.h>

KT_BQ25601 Charger;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin();

    while (!Charger.begin(&Wire)) {
        Serial.println("Couldnt find BQ25601.");
        delay(2000);
    }

    Serial.println("BQ25601 initialized successfully.");

    while (Charger.isPowerPresent()) {
        Serial.println(F("VBUS detected. Please disconnect power."));
        delay(1000);
    }

    Charger.setRegulationVoltageOTG(KT_BQ25601_REGULATION_VOLTAGE_OTG_5V15);
    Charger.setCurrentLimitOTG(KT_BQ25601_CURRENT_LIMIT_OTG_1A2);
    Charger.minVoltageOTG(KT_BQ25601_MIN_VOLTAGE_OTG_2V8);

    Charger.enableOTG(true);

    delay(50);

    if (Charger.isOTGEnabled()) {
        Serial.println(F("OTG is enabled."));
    } else {
        Serial.println(F("Failed to enable OTG."));
    }
}

void loop() {
    kt_bq25601_power_status_enum powerStatus = Charger.getPowerStatus();
    kt_bq25601_faults f = Charger.getFaults();

    Serial.print(F("Power Status: "));
    if (powerStatus == KT_BQ25601_POWER_STATUS_OTG) {
        Serial.println(F("OTG Active"));
    } else {
        Serial.println(F("OTG Inactive"));
    }

    if (f.otgFault) {
        Serial.println(F("OTG Fault detected."));
    }
    if (f.ntcFault != KT_BQ25601_NTC_FAULT_NORMAL) {
        Serial.println(F("NTC Fault."));
    }

    Serial.println();
    delay(3000);
}