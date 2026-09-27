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
#define SCL 8
#define SDA 9

// Only for reference, Li-Po 3.7 V
#define BATTERY_CAPACITY_MAH 800

KT_BQ25601 Charger;

bool isPowerPresent;
kt_bq25601_power_status_enum powerStatus;
kt_bq25601_charge_status_enum chargeStatus;
bool isPowerGood;


void setup(){
    Serial.begin(115200);
    delay(1000);

    Wire.begin(SDA, SCL);

    while (!Charger.begin(Wire)) {
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

    Charger.setWatchdog(KT_BQ25601_WATCHDOG_40S);
}

void loop(){

    // Feed the Watchdog
    Charger.resetWatchdog();

    isPowerPresent = Charger.isPowerPresent();
    powerStatus = Charger.getPowerStatus();
    chargeStatus = Charger.getChargeStatus();
    isPowerGood = Charger.isPowerGood();

    Serial.println("BQ25601 Status:");
    Serial.println();

    if(isPowerPresent){
        Serial.println("VBUS Detected");
    } else {
        Serial.println("VBUS Not Detected");
    }

    switch (powerStatus) {
        case KT_BQ25601_POWER_STATUS_NO_INPUT: Serial.println("No input"); break;
        case KT_BQ25601_POWER_STATUS_USB_HOST: Serial.println("USB"); break;
        case KT_BQ25601_POWER_STATUS_ADAPTER: Serial.println("Adapter"); break;
        case KT_BQ25601_POWER_STATUS_OTG: Serial.println("OTG (Powerbank)"); break;
    }

    switch (chargeStatus) {
        case KT_BQ25601_CHARGE_STATUS_NOT_CHARGING: Serial.println("Not charging"); break;
        case KT_BQ25601_CHARGE_STATUS_PRE_CHARGE: Serial.println("Pre-charging"); break;
        case KT_BQ25601_CHARGE_STATUS_FAST_CHARGING: Serial.println("Fast charging"); break;
        case KT_BQ25601_CHARGE_STATUS_CHARGE_TERMINATION: Serial.println("Charge terminated"); break;
    }

    if(isPowerGood){
        Serial.println("Power is good");
    } else {
        Serial.println("Power is not good");
    }
    
    delay(5000);
}