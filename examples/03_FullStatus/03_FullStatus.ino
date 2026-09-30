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

void printFullStatus();

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin();

    while (!Charger.begin(&Wire)) {
        Serial.println("Couldnt find BQ25601.");
        delay(2000);
    }

    Serial.println("BQ25601 initialized successfully.");
}

void loop() {
    printFullStatus();
    delay(4000);
}

void printFullStatus() {
    Serial.println(F("BQ25601 STATUS"));

    Serial.print("Part Number: 0x");
    Serial.println(Charger.getPartNumber(), HEX);
    Serial.print("Device Revision: 0x");
    Serial.println(Charger.getDeviceRevision(), HEX);

    Serial.print(F("Input VBUS: "));
    Serial.print(Charger.isPowerPresent() ? F("PRESENT") : F("NOT PRESENT"));
    Serial.print(F(" | Power Good: "));
    Serial.print(Charger.isPowerGood() ? F("GOOD") : F("NOT GOOD"));
    Serial.print(F(" | Type: "));
    switch (Charger.getPowerStatus()) {
        case KT_BQ25601_POWER_STATUS_NO_INPUT: Serial.println("No input"); break;
        case KT_BQ25601_POWER_STATUS_USB_HOST: Serial.println("USB"); break;
        case KT_BQ25601_POWER_STATUS_ADAPTER: Serial.println("Adapter"); break;
        case KT_BQ25601_POWER_STATUS_OTG: Serial.println("OTG (Powerbank)"); break;
    }

    Serial.print(F("Charging: "));
    Serial.print(Charger.isChargingEnabled() ? F("Enabled") : F("Disabled"));
    Serial.print(F(" | Active: "));
    Serial.print(Charger.isCharging() ? F("YES") : F("NO"));
    Serial.print(F(" | Completed: "));
    Serial.println(Charger.isCharged() ? F("YES") : F("NO"));

    Serial.print(F("Charge Stage: "));
    switch (Charger.getChargeStatus()) {
        case KT_BQ25601_CHARGE_STATUS_NOT_CHARGING: Serial.println("Not charging"); break;
        case KT_BQ25601_CHARGE_STATUS_PRE_CHARGE: Serial.println("Pre-charging"); break;
        case KT_BQ25601_CHARGE_STATUS_FAST_CHARGING: Serial.println("Fast charging"); break;
        case KT_BQ25601_CHARGE_STATUS_CHARGE_TERMINATION: Serial.println("Charge terminated"); break;
    }

    Serial.println(F("Configured Parameters"));
    Serial.print(F("Charge Voltage (VREG): "));
    Serial.print(Charger.getChargeVoltage());
    Serial.println(F(" mV"));

    Serial.print(F("Charge Current (ICHG): "));
    Serial.print(Charger.getChargeCurrent());
    Serial.println(F(" mA"));

    Serial.print(F("Pre-charge Current: "));
    Serial.print(Charger.getPrechargeCurrent());
    Serial.print(F(" mA | Termination Current: "));
    Serial.print(Charger.getTerminationCurrent());
    Serial.println(F(" mA"));

    Serial.print(F("Input Current Limit (IINDPM): "));
    Serial.print(Charger.getInputCurrentLimit());
    Serial.print(F(" mA | Input Voltage Threshold (VINDPM): "));
    Serial.print(Charger.getInputVoltageThreshold());
    Serial.println(F(" mV"));

    Serial.println(F("Dynamic Regulation Status"));
    Serial.print(F("In VINDPM (Voltage Limit): "));
    Serial.println(Charger.isInputVoltageLimit() ? F("ACTIVE") : F("INACTIVE"));

    Serial.print(F("In IINDPM (Current Limit): "));
    Serial.println(Charger.isInputCurrentLimit() ? F("ACTIVE") : F("INACTIVE"));

    Serial.print(F("Thermal Regulation: "));
    Serial.println(Charger.getThermalRegulationStatus() == KT_BQ25601_THERMAL_REGULATION_ON ? F("REGULATION ON") : F("REGULATION OFF"));

    Serial.print(F("SYS Regulation: "));
    Serial.println(Charger.getSysRegulationStatus() == KT_BQ25601_SYS_REGULATION_ON ? F("REGULATION ON") : F("REGULATION OFF"));

    kt_bq25601_faults f = Charger.getFaults();
    Serial.println(F("Fault Register"));
    if (!f.watchdogFault && !f.otgFault && !f.batFault &&
        f.chargeFault == KT_BQ25601_CHARGE_FAULT_NORMAL &&
        f.ntcFault == KT_BQ25601_NTC_FAULT_NORMAL) {
        Serial.println(F("No faults detected"));
    } else {
        if (f.watchdogFault) Serial.println(F("FAULT: Watchdog Timer Expired!"));
        if (f.otgFault)      Serial.println(F("FAULT: OTG Boost Overload / OVP / Bat Low!"));
        if (f.batFault)      Serial.println(F("FAULT: Battery Overvoltage (BATOVP)!"));

        if (f.chargeFault != KT_BQ25601_CHARGE_FAULT_NORMAL) {
            Serial.print(F("CHARGE FAULT: "));
            switch (f.chargeFault) {
                case KT_BQ25601_CHARGE_FAULT_INPUT_FAULT: Serial.println(F("Input Fault")); break;
                case KT_BQ25601_CHARGE_FAULT_THERMAL_SHUTDOWN: Serial.println(F("Thermal Shutdown")); break;
                case KT_BQ25601_CHARGE_FAULT_CHARGE_SAFETY_TIMER_EXPIRATION: Serial.println(F("Safety Timer Expired")); break;
                default: break;
            }
        }

        if (f.ntcFault != KT_BQ25601_NTC_FAULT_NORMAL) {
            Serial.print(F("NTC THERMISTOR FAULT: "));
            switch (f.ntcFault) {
                case KT_BQ25601_NTC_FAULT_WARM: Serial.println(F("WARM")); break;
                case KT_BQ25601_NTC_FAULT_COOL: Serial.println(F("COOL")); break;
                case KT_BQ25601_NTC_FAULT_COLD: Serial.println(F("COLD")); break;
                case KT_BQ25601_NTC_FAULT_HOT:  Serial.println(F("HOT")); break;
                default: break;
            }
        }
    }
}