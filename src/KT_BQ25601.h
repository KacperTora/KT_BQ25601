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
#define KT_BQ25601_ENABLE_CHARGE_SAFETY_TIMER_MASK 0x04
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

enum kt_bq25601_watchdog_enum {
    KT_BQ25601_WATCHDOG_NONE = 0,
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
    KT_BQ25601_DISBALE_STAT = 3
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
    KT_BQ25601_REGULATION_VOLTAGE_OTG_5V3 = 1
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

class KT_BQ25601
{
    public:
    explicit KT_BQ25601(TwoWire *wire = &Wire);

    bool begin(TwoWire &wire = Wire);
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
    bool setCurrentLImitOTG(kt_bq25601_current_limit_otg_enum limit);
    bool enableQ1FullOn(bool enable);
    bool setChargeCurrent(uint16_t current_mA);
    bool setPrechargeCurrent(uint16_t current_mA);
    bool setTerminationCurrent(uint16_t current_mA);
    bool setChargeVoltage(uint16_t voltage_mV);
    bool setTopOffTimer(kt_bq25601_topoff_timer_enum time);
    bool setRechargeThreshold(kt_bq25601_recharge_threshold_enum threshold);
    bool enableTermination(bool enable);
    bool enableSafetyTimer(bool enable);
    bool enableChargeSafetyTimer(bool enable);
    bool setThermalRegulationThreshold(kt_bq25601_thermal_regulation_threshold_enum threshold);
    bool setCoolTemperatureCurrentLimit(kt_bq25601_cool_temperature_current_limit_enum limit);
    bool setOVP(kt_bq25601_ovp_enum voltage);
    bool setRegulationVoltageOTG(kt_bq25601_regulation_voltage_otg_enum voltage);
    bool setInputVoltageThreshold(uint16_t voltage_mV);
    bool enableInputCurrentDetectionLimit(bool enable);
    bool slowSafetyTimer(bool enable);
    bool enableShippingMode(bool enable);
    bool setWarmTemperatureVoltageSetting(kt_bq25601_warm_temperature_voltage_setting_enum voltage);
    bool setBatfetDelay(kt_bq25601_batfet_delay_enum delay);
    bool enableBatfetResetFunction(bool enable);
    bool setBatTracking(kt_bq25601_bat_track_enum track);

    void setDebugPort(Stream &debugPort) { _debugPort = &debugPort; }
    void disableDebug() { _debugPort = nullptr; }

    private:

    uint8_t _readRegister(uint8_t reg);
    void _writeRegister(uint8_t reg, uint8_t value);
    bool updateReg(uint8_t value, uint8_t reg, uint8_t mask, int offset);

    TwoWire* _wire;
    Stream *_debugPort; 
};