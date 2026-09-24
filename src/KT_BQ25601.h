#pragma once

#include "Arduino.h"
#include "Wire.h"

#define KT_BQ25601_ADDR 0x6B

#define KT_BQ25601_REG01 0x01
#define KT_BQ25601_REG00 0x00
#define KT_BQ25601_REG05 0x05

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

enum kt_bq25601_min_voltage_otg_enum {
    KT_BQ25601_ENABLE_STAT = 0,
    KT_BQ25601_DISBALE_STAT = 3
};

enum kt_bq25601_enable_stat_enum {
    KT_BQ25601_MIN_VOLTAGE_OTG_2V8 = 0,
    KT_BQ25601_MIN_VOLTAGE_OTG_2V5 = 1
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
    bool enableSTAT(kt_bq25601_min_voltage_otg_enum enable);
    bool setInputCurrentLimit(uint16_t current_mA);

    void setDebugPort(Stream &debugPort) { _debugPort = &debugPort; }
    void disableDebug() { _debugPort = nullptr; }

    private:

    uint8_t _readRegister(uint8_t reg);
    void _writeRegister(uint8_t reg, uint8_t value);
    bool updateReg(uint8_t value, uint8_t reg, uint8_t mask, int offset);

    TwoWire* _wire;
    Stream *_debugPort; 
};