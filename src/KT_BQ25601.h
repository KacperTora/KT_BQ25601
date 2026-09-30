/*
 * KT_BQ25601 Arduino Library
 * Copyright (C) 2026 Kacper Tora
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 */

#pragma once

#include "Arduino.h"
#include "Wire.h"

#define KT_BQ25601_ADDR 0x6B

#define KT_BQ25601_REG00 0x00
#define KT_BQ25601_REG01 0x01
#define KT_BQ25601_REG02 0x02
#define KT_BQ25601_REG03 0x03
#define KT_BQ25601_REG04 0x04
#define KT_BQ25601_REG05 0x05
#define KT_BQ25601_REG06 0x06
#define KT_BQ25601_REG07 0x07
#define KT_BQ25601_REG08 0x08
#define KT_BQ25601_REG09 0x09
#define KT_BQ25601_REG0A 0x0A
#define KT_BQ25601_REG0B 0x0B

#define KT_BQ25601_ENABLE_CHARGING_MASK 0x10
#define KT_BQ25601_ENABLE_OTG_MASK 0x20
#define KT_BQ25601_SET_WATCHDOG_MASK 0x30
#define KT_BQ25601_RESET_WATCHDOG_MASK 0x40
#define KT_BQ25601_SYS_MIN_VOLTAGE_MASK 0x0E
#define KT_BQ25601_MIN_VOLTAGE_OTG_MASK 0x01
#define KT_BQ25601_ENABLE_PFM_MASK 0x80
#define KT_BQ25601_ENABLE_HIZ_MASK 0x80
#define KT_BQ25601_ENABLE_STAT_MASK 0x60
#define KT_BQ25601_SET_INPUT_CURRENT_LIMIT_MASK 0x1F
#define KT_BQ25601_SET_CURRENT_LIMIT_OTG_MASK 0x80
#define KT_BQ25601_Q1_FULLON_MASK 0x40
#define KT_BQ25601_CHARGE_CURRENT_MASK 0x3F
#define KT_BQ25601_PRECHARGE_CURRENT_MASK 0xF0
#define KT_BQ25601_TERMINATION_CURRENT_MASK 0x0F
#define KT_BQ25601_CHARGE_VOLTAGE_MASK 0xF8
#define KT_BQ25601_TOPOFF_TIMER_MASK 0x06
#define KT_BQ25601_RECHARGE_THRESHOLD_MASK 0x01
#define KT_BQ25601_ENABLE_TERMINATION_MASK 0x80
#define KT_BQ25601_ENABLE_SAFETY_TIMER_MASK 0x08
#define KT_BQ25601_CHARGE_SAFETY_TIMER_MASK 0x04
#define KT_BQ25601_THERMAL_REGULATION_THRESHOLD_MASK 0x02
#define KT_BQ25601_COOL_TEMPERATURE_CURRENT_LIMIT_MASK 0x01
#define KT_BQ25601_OVP_MASK 0xC0
#define KT_BQ25601_REGULATION_VOLTAGE_OTG_MASK 0x30
#define KT_BQ25601_INPUT_VOLTAGE_THRESHOLD_MASK 0x0F
#define KT_BQ25601_INPUT_CURRENT_DETECTION_LIMIT_MASK 0x80
#define KT_BQ25601_SLOW_SAFETY_TIMER_MASK 0x40
#define KT_BQ25601_SHIPPING_MODE_MASK 0x20
#define KT_BQ25601_WARM_TEMPERATURE_VOLTAGE_SETTING_MASK 0x10
#define KT_BQ25601_BATTFET_DELAY_MASK 0x08
#define KT_BQ25601_BATTFET_RESET_FUNCTION_MASK 0x04
#define KT_BQ25601_BAT_TRACK_MASK 0x03
#define KT_BQ25601_CHARGE_STATUS_MASK 0x18
#define KT_BQ25601_VBUS_STATUS_MASK 0xE0
#define KT_BQ25601_POWER_GOOD_STATUS_MASK 0x04
#define KT_BQ25601_THERMAL_REGULATION_STATUS_MASK 0x02
#define KT_BQ25601_SYS_REGULATION_STATUS_MASK 0x01
#define KT_BQ25601_WATCHDOG_FAULT_MASK 0x80
#define KT_BQ25601_OTG_FAULT_MASK 0x40
#define KT_BQ25601_CHARGE_FAULT_MASK 0x30
#define KT_BQ25601_BAT_FAULT_MASK 0x08
#define KT_BQ25601_NTC_FAULT_MASK 0x07
#define KT_BQ25601_POWER_PRESENT_MASK 0x80
#define KT_BQ25601_INPUT_VOLTAGE_LIMIT_STATUS_MASK 0x40
#define KT_BQ25601_INPUT_CURRENT_LIMIT_STATUS_MASK 0x20
#define KT_BQ25601_TOPOFF_TIMER_ACTIVE_MASK 0x08
#define KT_BQ25601_OVERVOLTAGE_STATUS_MASK 0x04
#define KT_BQ25601_VINDPM_INT_MASK 0x02
#define KT_BQ25601_IINDPM_INT_MASK 0x01
#define KT_BQ25601_RESET_MASK 0x80
#define KT_BQ25601_PART_NUMBER_MASK 0x78
#define KT_BQ25601_DEV_REV_MASK 0x03

enum kt_bq25601_watchdog_enum {
    KT_BQ25601_WATCHDOG_DISABLE = 0,
    KT_BQ25601_WATCHDOG_40S = 1,
    KT_BQ25601_WATCHDOG_80S = 2,
    KT_BQ25601_WATCHDOG_160S = 3
};

enum kt_bq25601_sys_min_voltage_enum {
    KT_BQ25601_SYS_MIN_VOLTAGE_2V6 = 0,
    KT_BQ25601_SYS_MIN_VOLTAGE_2V8 = 1,
    KT_BQ25601_SYS_MIN_VOLTAGE_3V = 2,
    KT_BQ25601_SYS_MIN_VOLTAGE_3V2 = 3,
    KT_BQ25601_SYS_MIN_VOLTAGE_3V4 = 4,
    KT_BQ25601_SYS_MIN_VOLTAGE_3V5 = 5,
    KT_BQ25601_SYS_MIN_VOLTAGE_3V6 = 6,
    KT_BQ25601_SYS_MIN_VOLTAGE_3V7 = 7
};

enum kt_bq25601_enable_stat_enum {
    KT_BQ25601_ENABLE_STAT = 0,
    KT_BQ25601_DISABLE_STAT = 3
};

enum kt_bq25601_min_voltage_otg_enum {
    KT_BQ25601_MIN_VOLTAGE_OTG_2V8 = 0,
    KT_BQ25601_MIN_VOLTAGE_OTG_2V5 = 1
};

enum kt_bq25601_current_limit_otg_enum {
    KT_BQ25601_CURRENT_LIMIT_OTG_0A5 = 0,
    KT_BQ25601_CURRENT_LIMIT_OTG_1A2 = 1
};

enum kt_bq25601_topoff_timer_enum {
    KT_BQ25601_TOPOFF_TIMER_DISABLE = 0,
    KT_BQ25601_TOPOFF_TIMER_15_MINUTES = 1,
    KT_BQ25601_TOPOFF_TIMER_30_MINUTES = 2,
    KT_BQ25601_TOPOFF_TIMER_45_MINUTES = 3
};

enum kt_bq25601_recharge_threshold_enum {
    KT_BQ25601_RECHARGE_THRESHOLD_0V1 = 0,
    KT_BQ25601_RECHARGE_THRESHOLD_0V2 = 1
};

enum kt_bq25601_thermal_regulation_threshold_enum {
    KT_BQ25601_THERMAL_REGULATION_THRESHOLD_90_DEGREES = 0,
    KT_BQ25601_THERMAL_REGULATION_THRESHOLD_110_DEGREES = 1
};

enum kt_bq25601_cool_temperature_current_limit_enum {
    KT_BQ25601_COOL_TEMPERATURE_CURRENT_LIMIT_50_PERCENT = 0,
    KT_BQ25601_COOL_TEMPERATURE_CURRENT_LIMIT_20_PERCENT = 1
};

enum kt_bq25601_ovp_enum {
    KT_BQ25601_OVP_5V5 = 0,
    KT_BQ25601_OVP_6V5 = 1,
    KT_BQ25601_OVP_10V5 = 2,
    KT_BQ25601_OVP_14V = 3
};

enum kt_bq25601_regulation_voltage_otg_enum {
    KT_BQ25601_REGULATION_VOLTAGE_OTG_4V85 = 0,
    KT_BQ25601_REGULATION_VOLTAGE_OTG_5V = 1,
    KT_BQ25601_REGULATION_VOLTAGE_OTG_5V15 = 2,
    KT_BQ25601_REGULATION_VOLTAGE_OTG_5V3 = 3
};

enum kt_bq25601_warm_temperature_voltage_setting_enum {
    KT_BQ25601_WARM_TEMPERATURE_VOLTAGE_SETTING_4V1 = 0,
    KT_BQ25601_WARM_TEMPERATURE_VOLTAGE_SETTING_CHARGE_VOLTAGE = 1
};

enum kt_bq25601_batfet_delay_enum {
    KT_BQ25601_BATFET_DELAY_NONE = 0,
    KT_BQ25601_BATFET_DELAY_10S = 1
};

enum kt_bq25601_bat_track_enum {
    KT_BQ25601_BAT_TRACK_DISABLE = 0,
    KT_BQ25601_BAT_TRACK_0V2 = 1,
    KT_BQ25601_BAT_TRACK_0V25 = 2,
    KT_BQ25601_BAT_TRACK_0V3 = 3
};

enum kt_bq25601_power_status_enum {
    KT_BQ25601_POWER_STATUS_NO_INPUT = 0,
    KT_BQ25601_POWER_STATUS_USB_HOST = 1,
    KT_BQ25601_POWER_STATUS_ADAPTER = 3,
    KT_BQ25601_POWER_STATUS_OTG = 7
};


enum kt_bq25601_charge_status_enum {
    KT_BQ25601_CHARGE_STATUS_NOT_CHARGING = 0,
    KT_BQ25601_CHARGE_STATUS_PRE_CHARGE = 1,
    KT_BQ25601_CHARGE_STATUS_FAST_CHARGING = 2,
    KT_BQ25601_CHARGE_STATUS_CHARGE_TERMINATION = 3
};

enum kt_bq25601_thermal_regulation_status_enum {
    KT_BQ25601_THERMAL_REGULATION_OFF = 0,
    KT_BQ25601_THERMAL_REGULATION_ON = 1
};

enum kt_bq25601_sys_regulation_status_enum {
    KT_BQ25601_SYS_REGULATION_OFF = 0,
    KT_BQ25601_SYS_REGULATION_ON = 1
};

enum kt_bq25601_charge_fault_enum {
    KT_BQ25601_CHARGE_FAULT_NORMAL = 0,
    KT_BQ25601_CHARGE_FAULT_INPUT_FAULT = 1,
    KT_BQ25601_CHARGE_FAULT_THERMAL_SHUTDOWN = 2,
    KT_BQ25601_CHARGE_FAULT_CHARGE_SAFETY_TIMER_EXPIRATION = 3
};

enum kt_bq25601_ntc_fault_enum {
    KT_BQ25601_NTC_FAULT_NORMAL = 0,
    KT_BQ25601_NTC_FAULT_WARM = 2,
    KT_BQ25601_NTC_FAULT_COOL = 3,
    KT_BQ25601_NTC_FAULT_COLD = 5,
    KT_BQ25601_NTC_FAULT_HOT = 6
};

enum kt_bq25601_charge_safety_timer_enum {
    KT_BQ25601_CHARGE_SAFETY_TIMER_5H = 0,
    KT_BQ25601_CHARGE_SAFETY_TIMER_10H = 1
};

struct kt_bq25601_faults {
    bool watchdogFault;
    bool otgFault;
    kt_bq25601_charge_fault_enum chargeFault;
    bool batFault;
    kt_bq25601_ntc_fault_enum ntcFault;
};

class KT_BQ25601
{
    public:
    explicit KT_BQ25601(TwoWire *wire = &Wire);

    bool begin(TwoWire *wire = nullptr);
    bool isConnected();
    bool enableCharging(bool enable);
    bool enableOTG(bool enable);
    bool setWatchdog(kt_bq25601_watchdog_enum watchdog);
    bool resetWatchdog();
    bool setMinVoltage(kt_bq25601_sys_min_voltage_enum voltage);
    bool minVoltageOTG(kt_bq25601_min_voltage_otg_enum voltage);
    bool enablePFM(bool enable);
    bool enableHIZ(bool enable);
    bool enableSTAT(kt_bq25601_enable_stat_enum enable);
    bool setInputCurrentLimit(uint16_t current_mA);
    bool setCurrentLimitOTG(kt_bq25601_current_limit_otg_enum limit);
    bool enableQ1FullOn(bool enable);
    bool setChargeCurrent(uint16_t current_mA);
    bool setPrechargeCurrent(uint16_t current_mA);
    bool setTerminationCurrent(uint16_t current_mA);
    bool setChargeVoltage(uint16_t voltage_mV);
    bool setTopOffTimer(kt_bq25601_topoff_timer_enum time);
    bool setRechargeThreshold(kt_bq25601_recharge_threshold_enum threshold);
    bool enableTermination(bool enable);
    bool enableSafetyTimer(bool enable);
    bool setChargeSafetyTimer(kt_bq25601_charge_safety_timer_enum hours);
    bool setThermalRegulationThreshold(kt_bq25601_thermal_regulation_threshold_enum threshold);
    bool setCoolTemperatureCurrentLimit(kt_bq25601_cool_temperature_current_limit_enum limit);
    bool setOVPThreshold(kt_bq25601_ovp_enum voltage);
    bool setRegulationVoltageOTG(kt_bq25601_regulation_voltage_otg_enum voltage);
    bool setInputVoltageThreshold(uint16_t voltage_mV);
    bool enableInputCurrentDetectionLimit(bool enable);
    bool slowSafetyTimer(bool enable);
    bool enableShippingMode(bool enable);
    bool setWarmTemperatureVoltageSetting(kt_bq25601_warm_temperature_voltage_setting_enum voltage);
    bool setBatfetDelay(kt_bq25601_batfet_delay_enum delay);
    bool enableBatfetResetFunction(bool enable);
    bool setBatTracking(kt_bq25601_bat_track_enum track);
    bool enableVoltageRegulationInterrupt(bool enable);
    bool enableCurrentRegulationInterrupt(bool enable);
    bool reset();

    kt_bq25601_power_status_enum getPowerStatus();
    kt_bq25601_charge_status_enum getChargeStatus();
    bool isPowerGood();
    kt_bq25601_thermal_regulation_status_enum getThermalRegulationStatus();
    kt_bq25601_sys_regulation_status_enum getSysRegulationStatus();
    bool isWatchdogFault();
    bool isOTGFault();
    kt_bq25601_charge_fault_enum getChargeFault();
    bool isBatFault();
    kt_bq25601_ntc_fault_enum getNTCFault();
    kt_bq25601_faults getFaults();
    bool isPowerPresent();
    bool isInputVoltageLimit();
    bool isInputCurrentLimit();
    bool isTopOffTimerActive();
    bool isOvervoltage();
    uint8_t getPartNumber();
    uint8_t getDeviceRevision();
    uint16_t getInputCurrentLimit();
    uint16_t getChargeCurrent();
    uint16_t getPrechargeCurrent();
    uint16_t getChargeVoltage();
    uint16_t getTerminationCurrent();
    uint16_t getInputVoltageThreshold();
    bool isChargingEnabled();
    bool isOTGEnabled();

    void setDebugPort(Stream &debugPort) { _debugPort = &debugPort; }
    void disableDebug() { _debugPort = nullptr; }

    private:

    uint8_t _readRegister(uint8_t reg);
    bool _writeRegister(uint8_t reg, uint8_t value);
    bool _updateReg(uint8_t value, uint8_t reg, uint8_t mask, int offset);
    uint8_t _readReg(uint8_t reg, uint8_t mask, int offset);

    TwoWire* _wire;
    Stream *_debugPort; 
};