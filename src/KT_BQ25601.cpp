/*
 * KT_BQ25601 Arduino Library
 * Copyright (C) 2026 Kacper Tora
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 */

#include "KT_BQ25601.h"

KT_BQ25601::KT_BQ25601(TwoWire *wire){
  _wire = wire;
  _debugPort = nullptr;
}

uint8_t KT_BQ25601::_readRegister(uint8_t reg) {
  _wire -> beginTransmission(KT_BQ25601_ADDR);
  _wire -> write(reg);
  _wire -> endTransmission(false);

  _wire -> requestFrom((uint8_t)KT_BQ25601_ADDR, (uint8_t)1);

  if (_wire -> available()) {
    return _wire -> read();
  }
  return 0x00;
}

/*!
 *    @brief  Sets up the hardware, initializes I2C and disables the watchdog
 *    @param  wire
 *            The Wire object (I2C)
 *    @return True if success
 */
bool KT_BQ25601::begin(TwoWire &wire){

  _wire = &wire;

  if(!isConnected()){

    if (_debugPort != nullptr) _debugPort->println("BQ25601 is not connected!");
    return false;
  }

  if (!setWatchdog(KT_BQ25601_WATCHDOG_DISABLE)) {

    if (_debugPort != nullptr) {
      _debugPort->println("Failed to disable Watchdog!");
    }

    return false;
  }

  return true;
}

/*!
 *    @brief  Check if the BQ25601 is connected
 *    @return True if the BQ25601 is connected
 */
bool KT_BQ25601::isConnected(){

  _wire->beginTransmission(KT_BQ25601_ADDR);
  return ( _wire->endTransmission() == 0);
}

void KT_BQ25601::_writeRegister(uint8_t reg, uint8_t value){

  _wire -> beginTransmission(KT_BQ25601_ADDR);
  _wire -> write(reg);

  _wire -> write(value);

  _wire -> endTransmission();
}

bool KT_BQ25601::updateReg(uint8_t value, uint8_t reg, uint8_t mask, int offset){
  uint8_t regVal = _readRegister(reg);
  regVal &= ~mask;
  regVal |= ((value << offset) & mask);
  _writeRegister(reg, regVal);
  return true;
}

uint8_t KT_BQ25601::_readReg(uint8_t reg, uint8_t mask, int offset){
  uint8_t regVal = _readRegister(reg);
  regVal &= mask;
  return regVal >> offset;
}

// REG00

/*!
 *    @brief Enable High Impedance Mode
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableHIZ(bool enable){
  return updateReg(enable, KT_BQ25601_REG00, KT_BQ25601_ENABLE_HIZ_MASK, 7);
}

/*!
 *    @brief Enable Charger Status (STAT)
 *    @param enable Enum value (KT_BQ25601_ENABLE_STAT or KT_BQ25601_DISABLE_STAT)
 *    @return True if success
 */
bool KT_BQ25601::enableSTAT(kt_bq25601_enable_stat_enum enable){
  return updateReg(enable, KT_BQ25601_REG00, KT_BQ25601_ENABLE_STAT_MASK, 5);
}

/*!
 *    @brief Set Input Current Limit. Offset is 100 mA, default is 2400 mA, max value is 3200 mA, min value is 100 mA
 *    @param current_mA Current in milliamperes
 *    @return True if success
 */
bool KT_BQ25601::setInputCurrentLimit(uint16_t current_mA){

  if(current_mA < 100) current_mA = 100;
  else if(current_mA > 3200) current_mA = 3200;

  current_mA = (current_mA - 100) / 100;

  return updateReg(current_mA, KT_BQ25601_REG00, KT_BQ25601_SET_INPUT_CURRENT_LIMIT_MASK, 0);
}

/*!
 *    @brief Get Input Current Limit
 *    @return Current in milliamperes
 */
uint16_t KT_BQ25601::getInputCurrentLimit(){
  int16_t current_mA = _readReg(KT_BQ25601_REG00, KT_BQ25601_SET_INPUT_CURRENT_LIMIT_MASK, 0);

  current_mA = (current_mA * 100) + 100;
  return current_mA;
}

// REG01

/*!
 *    @brief Enable Pulse Frequency Modulation (PFM)
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enablePFM(bool enable){
  return updateReg(!enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_PFM_MASK, 7);
}

/*!
 *    @brief Resets Watchdog. Important if Wathcdog Timer is set.
 *    @return True if success
 */
bool KT_BQ25601::resetWatchdog(){
  return updateReg(1, KT_BQ25601_REG01, KT_BQ25601_RESET_WATCHDOG_MASK, 6);
}

/*!
 *    @brief Enable USB On The Go (OTG, Powerbank Mode)
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableOTG(bool enable){
  return updateReg(enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_OTG_MASK, 5);
}

/*!
 *    @brief Enable Charging
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableCharging(bool enable){
  return updateReg(enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_CHARGING_MASK, 4);
}

/*!
 *    @brief Set System Minimum Voltage. Default is 3.5 V, max is 3.7 V, min is 2.6 V.
 *    @param voltage Enum value to set voltage (e.g. KT_BQ25601_SYS_MIN_VOLTAGE_3V5)
 *    @return True if success
 */
bool KT_BQ25601::setMinVoltage(kt_bq25601_sys_min_voltage_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG01, KT_BQ25601_SYS_MIN_VOLTAGE_MASK, 1);
}

/*!
 *    @brief Set Battery Minimum Voltage for OTG.
 *    @param voltage Enum to set voltage
 *    @return True if success
 */
bool KT_BQ25601::minVoltageOTG(kt_bq25601_min_voltage_otg_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG01, KT_BQ25601_MIN_VOLTAGE_OTG_MASK, 0);
}

/*!
 *    @brief Check if Charging is enabled
 *    @return True if enabled, false if disabled
 */
bool KT_BQ25601::isChargingEnabled(){
  return _readReg(KT_BQ25601_REG01, KT_BQ25601_ENABLE_CHARGING_MASK, 4);
}

/*!
 *    @brief Check if OTG is enabled
 *    @return True if enabled, false if disabled
 */
bool KT_BQ25601::isOTGEnabled(){
  return _readReg(KT_BQ25601_REG01, KT_BQ25601_ENABLE_OTG_MASK, 5);
}

// REG02

/*!
 *    @brief Set Curent Limit for OTG (500 mA or 1200 mA)
 *    @param limit Enum value to set current (KT_BQ25601_CURRENT_LIMIT_OTG_0A5 or KT_BQ25601_CURRENT_LIMIT_OTG_1A2)
 *    @return True if success
 */
bool KT_BQ25601::setCurrentLImitOTG(kt_bq25601_current_limit_otg_enum limit){
  return updateReg(limit, KT_BQ25601_REG02, KT_BQ25601_SET_CURRENT_LIMIT_OTG_MASK, 7);
}

/*!
 *    @brief Force lower Q1 MOSFET RDSON for better efficiency
 *    @param enable True to use lower Q1 RDSON for better efficiency, False to use higher Q1 RDSON for better accuracy
 *    @return True if success
 */
bool KT_BQ25601::enableQ1FullOn(bool enable) {
  return updateReg(enable, KT_BQ25601_REG02, KT_BQ25601_Q1_FULLON_MASK, 6);
}

/*!
 *    @brief Set Fast Charge Current. Offset is 60 mA, default is 2040 mA, max is 3000 mA, min is 0 mA
 *    @param current_mA Current in miliamperes
 *    @return True if success
 */
bool KT_BQ25601::setChargeCurrent(uint16_t current_mA){
  if(current_mA > 3000) current_mA = 3000;

  current_mA /= 60;

  return updateReg(current_mA, KT_BQ25601_REG02, KT_BQ25601_CHARGE_CURRENT_MASK, 0);
}

/*!
 *    @brief Get Fast Charge Current
 *    @return Current in milliamperes
 */
uint16_t KT_BQ25601::getChargeCurrent(){
  int16_t current_mA = _readReg(KT_BQ25601_REG02, KT_BQ25601_CHARGE_CURRENT_MASK, 0);

  current_mA *= 60;
  return current_mA;
}

// REG03

/*!
 *    @brief Set Precharge Current. Offset is 60 mA, default is 180 mA, max is 960 mA, min is 0 mA
 *    @param current_mA Current in miliamperes
 *    @note If Precharge Current > 780 mA it is clamped to 780 mA
 *    @return True if success
 */
bool KT_BQ25601::setPrechargeCurrent(uint16_t current_mA){
  if (current_mA < 60) current_mA = 60;
  else if (current_mA > 780) current_mA = 780;

  current_mA = (current_mA - 60) / 60;
  return updateReg(current_mA, KT_BQ25601_REG03, KT_BQ25601_PRECHARGE_CURRENT_MASK, 4);
}

/*!
 *    @brief Get Precharge Current
 *    @return Current in milliamperes
 */
uint16_t KT_BQ25601::getPrechargeCurrent(){
  int16_t current_mA = _readReg(KT_BQ25601_REG03, KT_BQ25601_PRECHARGE_CURRENT_MASK, 4);

  current_mA = (current_mA * 60) + 60;
  return current_mA;
}

/*!
 *    @brief Set Termination Current. Offset is 60 mA, default is 180 mA, max is 960 mA, min is 0 mA
 *    @param current_mA Current in miliamperes
 *    @return True if success
 */
bool KT_BQ25601::setTerminationCurrent(uint16_t current_mA){
  if (current_mA < 60) current_mA = 60;
  else if (current_mA > 780) current_mA = 780;

  current_mA = (current_mA - 60) / 60;
  return updateReg(current_mA, KT_BQ25601_REG03, KT_BQ25601_TERMINATION_CURRENT_MASK, 0);
}

/*!
 *    @brief Get Termination Current
 *    @return Current in milliamperes
 */
uint16_t KT_BQ25601::getTerminationCurrent(){
  int16_t current_mA = _readReg(KT_BQ25601_REG03, KT_BQ25601_TERMINATION_CURRENT_MASK, 0);

  current_mA = (current_mA * 60) + 60;
  return current_mA;
}

// REG04

/*!
 *    @brief Set Charge Voltage. Offset is 32 mV, default is 4208 mV, max is 4624 mV, min is 3856 mV
 *    @param voltage_mV Voltage in milivolts
 *    @return True if success
 */
bool KT_BQ25601::setChargeVoltage(uint16_t voltage_mV){
  if(voltage_mV < 3856) voltage_mV = 3856;
  else if(voltage_mV > 4624) voltage_mV = 4624;

  voltage_mV = (voltage_mV - 3856) / 32;

  return updateReg(voltage_mV, KT_BQ25601_REG04, KT_BQ25601_CHARGE_VOLTAGE_MASK, 3);
}

/*!
 *    @brief Get Charge Voltage
 *    @return Voltage in milivolts
 */
uint16_t KT_BQ25601::getChargeVoltage(){
  int16_t voltage_mV = _readReg(KT_BQ25601_REG04, KT_BQ25601_CHARGE_VOLTAGE_MASK, 3);

  voltage_mV = (voltage_mV * 32) + 3856;
  return voltage_mV;
}

/*!
 *    @brief Set the extended time following the terminatiom condition is met (disable, 15 minutes, 30 minutes or 45 minutes)
 *    @param time Enum value to set time (KT_BQ25601_TOPOFF_TIMER_DISABLE or KT_BQ25601_TOPOFF_TIMER_15_MINUTES ect.)
 *    @return True if success
 */
bool KT_BQ25601::setTopOffTimer(kt_bq25601_topoff_timer_enum time){
  return updateReg(time, KT_BQ25601_REG04, KT_BQ25601_TOPOFF_TIMER_MASK, 1);
}

/*!
 *    @brief Set Recharge Threshold (100 mV or 200 mV)
 *    @param time Enum value to set threshold (KT_BQ25601_RECHARGE_THRESHOLD_0V1 or KT_BQ25601_RECHARGE_THRESHOLD_0V2)
 *    @return True if success
 */
bool KT_BQ25601::setRechargeThreshold(kt_bq25601_recharge_threshold_enum threshold){
  return updateReg(threshold, KT_BQ25601_REG04, KT_BQ25601_RECHARGE_THRESHOLD_MASK, 0);
}

// REG05

/*!
 *    @brief Enable Termination
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableTermination(bool enable){
  return updateReg(enable, KT_BQ25601_REG05, KT_BQ25601_ENABLE_TERMINATION_MASK, 7);
}

/*!
 *    @brief Set Watchdog (disable, 40 seconds, 80 seconds or 160 seconds)
 *    @param watchdog Enum value to set Watchdog (KT_BQ25601_WATCHDOG_DISABLE or KT_BQ25601_WATCHDOG_40S ect.)
 *    @return True if success
 */
bool KT_BQ25601::setWatchdog(kt_bq25601_watchdog_enum watchdog){
  return updateReg(watchdog, KT_BQ25601_REG05, KT_BQ25601_SET_WATCHDOG_MASK, 4);
}

/*!
 *    @brief Enable both fast charge and precharge timer
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableSafetyTimer(bool enable){
  return updateReg(enable, KT_BQ25601_REG05, KT_BQ25601_ENABLE_SAFETY_TIMER_MASK, 3);
}

/*!
 *    @brief Set Charge Safety Timer (5 hours or 10 hours)
 *    @param hours Enum value to set timer (KT_BQ25601_CHARGE_SAFETY_TIMER_5H or KT_BQ25601_CHARGE_SAFETY_TIMER_10H)
 *    @return True if success
 */
bool KT_BQ25601::setChargeSafetyTimer(kt_bq25601_charge_safety_timer_enum hours){
  return updateReg(hours, KT_BQ25601_REG05, KT_BQ25601_CHARGE_SAFETY_TIMER_MASK, 2);
}

/*!
 *    @brief Set Thermal Regulation Threshold (90 degrees or 110 degrees)
 *    @param threshold Enum value to set threshold (KT_BQ25601_THERMAL_REGULATION_THRESHOLD_90_DEGREES or KT_BQ25601_THERMAL_REGULATION_THRESHOLD_110_DEGREES)
 *    @return True if success
 */
bool KT_BQ25601::setThermalRegulationThreshold(kt_bq25601_thermal_regulation_threshold_enum threshold){
  return updateReg(threshold, KT_BQ25601_REG05, KT_BQ25601_THERMAL_REGULATION_THRESHOLD_MASK, 1);
}

/*!
 *    @brief Set Cool Temperature Current Limit (50% or 20% of Charge Current)
 *    @param limit Enum value to set limit (KT_BQ25601_COOL_TEMPERATURE_CURRENT_LIMIT_50_PERCENT or KT_BQ25601_COOL_TEMPERATURE_CURRENT_LIMIT_20_PERCENT)
 *    @return True if success
 */
bool KT_BQ25601::setCoolTemperatureCurrentLimit(kt_bq25601_cool_temperature_current_limit_enum limit){
  return updateReg(limit, KT_BQ25601_REG05, KT_BQ25601_COOL_TEMPERATURE_CURRENT_LIMIT_MASK, 0);
}

// REG06

/*!
 *    @brief Set OVP Threshold (5.5 V, 6.5 V, 10.5 V or 14V)
 *    @param voltage Enum value to set threshold (KT_BQ25601_OVP_5V5 or KT_BQ25601_OVP_6V5 ect.)
 *    @return True if success
 */
bool KT_BQ25601::setOVPThreshold(kt_bq25601_ovp_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG06, KT_BQ25601_OVP_MASK, 6);
}

/*!
 *    @brief Set OTG Regulation Voltage (4.85 V, 5 V, 5.15 V or 5.3 V)
 *    @param voltage Enum value to set voltage (KT_BQ25601_REGULATION_VOLTAGE_OTG_4V85 or KT_BQ25601_REGULATION_VOLTAGE_OTG_5V ect.)
 *    @return True if success
 */
bool KT_BQ25601::setRegulationVoltageOTG(kt_bq25601_regulation_voltage_otg_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG06, KT_BQ25601_REGULATION_VOLTAGE_OTG_MASK, 4);
}

/*!
 *    @brief Set Input Voltage Threshold. Offset is 100 mV, default is 4500 mV, max is 5400 mV, min is 3900 mV
 *    @param voltage_mV Voltage in milivolts
 *    @return True if success
 */
bool KT_BQ25601::setInputVoltageThreshold(uint16_t voltage_mV){
  if(voltage_mV < 3900) voltage_mV = 3900;
  else if(voltage_mV > 5400) voltage_mV = 5400;

  voltage_mV = (voltage_mV - 3900) / 100;

  return updateReg(voltage_mV, KT_BQ25601_REG06, KT_BQ25601_INPUT_VOLTAGE_THRESHOLD_MASK, 0);
}

// REG07

/*!
 *    @brief Enable input current limit detection when VBUS is present 
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableInputCurrentDetectionLimit(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_INPUT_CURRENT_DETECTION_LIMIT_MASK, 7);
}

/*!
 *    @brief Slow Safety Timer by 2X
 *    @param enable True to slow
 *    @return True if success
 */
bool KT_BQ25601::slowSafetyTimer(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_SLOW_SAFETY_TIMER_MASK, 6);
}

/*!
 *    @brief Enable Shipping Mode (BATFET disable)
 *    @param enable True to enable shipping mode, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableShippingMode(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_SHIPPING_MODE_MASK, 5);
}

/*!
 *    @brief Set Warm Temperature Voltage SETTING (Charge voltage 4.1 V or charge voltage set to VREG)
 *    @param voltage Enum value to set voltage (KT_BQ25601_WARM_TEMPERATURE_VOLTAGE_SETTING_4V1 or KT_BQ25601_WARM_TEMPERATURE_VOLTAGE_SETTING_CHARGE_VOLTAGE)
 *    @return True if success
 */
bool KT_BQ25601::setWarmTemperatureVoltageSetting(kt_bq25601_warm_temperature_voltage_setting_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG07, KT_BQ25601_WARM_TEMPERATURE_VOLTAGE_SETTING_MASK, 4);
}

/*!
 *    @brief Set BATFET Delay (None or 10 seconds)
 *    @param delay Enum value to set delay (KT_BQ25601_BATFET_DELAY_NONE or KT_BQ25601_BATFET_DELAY_10S)
 *    @return True if success
 */
bool KT_BQ25601::setBatfetDelay(kt_bq25601_batfet_delay_enum delay){
  return updateReg(delay, KT_BQ25601_REG07, KT_BQ25601_BATTFET_DELAY_MASK, 3);
}

/*!
 *    @brief Enable BATFET Reset Fucntion
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableBatfetResetFunction(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_BATTFET_RESET_FUNCTION_MASK, 2);
}

/*!
 *    @brief Set BAT Tracking (Disable, VBAT + 200 mV, +250 mV or +300 mV)
 *    @param track Enum value to set track (KT_BQ25601_BAT_TRACK_DISABLE or KT_BQ25601_BAT_TRACK_0V2 ect.)
 *    @return True if success
 */
bool KT_BQ25601::setBatTracking(kt_bq25601_bat_track_enum track){
  return updateReg(track, KT_BQ25601_REG07, KT_BQ25601_BAT_TRACK_MASK, 0);
}

// REG08

/*!
 *    @brief Get VBUS Status Register (No input, USB, Adapter or OTG)
 *    @return Enum value (KT_BQ25601_POWER_STATUS_NO_INPUT, KT_BQ25601_POWER_STATUS_USB_HOST, KT_BQ25601_POWER_STATUS_ADAPTER or KT_BQ25601_POWER_STATUS_OTG)
 */
kt_bq25601_power_status_enum KT_BQ25601::getPowerStatus(){
  return (kt_bq25601_power_status_enum)_readReg(KT_BQ25601_REG08, KT_BQ25601_VBUS_STATUS_MASK, 5);
}

/*!
 *    @brief Get Charging Status (Not Carging, Pre-charge, Fast Charging or Charge Termination)
 *    @return Enum value (KT_BQ25601_CHARGE_STATUS_NOT_CHARGING, KT_BQ25601_CHARGE_STATUS_PRE_CHARGE, KT_BQ25601_CHARGE_STATUS_FAST_CHARGING or KT_BQ25601_CHARGE_STATUS_CHARGE_TERMINATION)
 */
kt_bq25601_charge_status_enum KT_BQ25601::getChargeStatus(){
  return (kt_bq25601_charge_status_enum)_readReg(KT_BQ25601_REG08, KT_BQ25601_CHARGE_STATUS_MASK, 3);
}

/*!
 *    @brief Get Power Good Status
 *    @return True if good, False if not good
 */
bool KT_BQ25601::isPowerGood(){
  return _readReg(KT_BQ25601_REG08, KT_BQ25601_POWER_GOOD_STATUS_MASK, 2);
}

/*!
 *    @brief Get Thermal Regulation Status (In thermal regulation or not in thermal regilation)
 *    @return Enum value (KT_BQ25601_THERMAL_REGULATION_OFF or KT_BQ25601_THERMAL_REGULATION_ON)
 */
kt_bq25601_thermal_regulation_status_enum KT_BQ25601::getThermalRegulationStatus(){
  return (kt_bq25601_thermal_regulation_status_enum)_readReg(KT_BQ25601_REG08, KT_BQ25601_THERMAL_REGULATION_STATUS_MASK, 1);
}

/*!
 *    @brief Get SYS Regulation Status (In SYS regulation or not in SYS regilation)
 *    @return Enum value (KT_BQ25601_SYS_REGULATION_OFF or KT_BQ25601_SYS_REGULATION_ON)
 */
kt_bq25601_sys_regulation_status_enum KT_BQ25601::getSysRegulationStatus(){
  return (kt_bq25601_sys_regulation_status_enum)_readReg(KT_BQ25601_REG08, KT_BQ25601_SYS_REGULATION_STATUS_MASK, 0);
}

// REG09

/*!
 *    @brief Watchdog Timer Expiration
 *    @return True if yes
 */
bool KT_BQ25601::isWatchdogFault(){
  return _readReg(KT_BQ25601_REG09, KT_BQ25601_WATCHDOG_FAULT_MASK, 7);
}

/*!
 *    @brief VBUS overloaded in OTG, or VBUS OVP, or battery is too low
 *    @return True if yes
 */
bool KT_BQ25601::isOTGFault(){
  return _readReg(KT_BQ25601_REG09, KT_BQ25601_OTG_FAULT_MASK, 6);
}

/*!
 *    @brief Get Charge Fault (No fault, input fault, thermal shutdown or charge safety timer expiration)
 *    @return Enum value (KT_BQ25601_CHARGE_FAULT_NORMAL, KT_BQ25601_CHARGE_FAULT_INPUT_FAULT, KT_BQ25601_CHARGE_FAULT_THERMAL_SHUTDOWM or KT_BQ25601_CHARGE_FAULT_CHARGE_SAFETY_TIMER_EXPIRATION)
 */
kt_bq25601_charge_fault_enum KT_BQ25601::getChargeFault(){
  return (kt_bq25601_charge_fault_enum)_readReg(KT_BQ25601_REG09, KT_BQ25601_CHARGE_FAULT_MASK, 4);
}

/*!
 *    @brief Battery Fault
 *    @return True if yes
 */
bool KT_BQ25601::isBatFault(){
  return _readReg(KT_BQ25601_REG09, KT_BQ25601_BAT_FAULT_MASK, 3);
}

/*!
 *    @brief Get NTC Fault (No fault, warm, cool, cold or hot)
 *    @return Enum value (KT_BQ25601_NTC_FAULT_NORMAL, KT_BQ25601_NTC_FAULT_WARM, KT_BQ25601_NTC_FAULT_COOL, KT_BQ25601_NTC_FAULT_COLD or KT_BQ25601_NTC_FAULT_HOT)
 */
kt_bq25601_ntc_fault_enum KT_BQ25601::getNTCFault(){
  return (kt_bq25601_ntc_fault_enum)_readReg(KT_BQ25601_REG09, KT_BQ25601_NTC_FAULT_MASK, 0);
}

// REG0A

/*!
 *    @brief Is VBUS attached
 *    @return True if yes
 */
bool KT_BQ25601::isPowerPresent(){
  return _readReg(KT_BQ25601_REG0A, KT_BQ25601_POWER_PRESENT_MASK, 7);
}

/*!
 *    @brief Is in VINDPM
 *    @return True if yes
 */
bool KT_BQ25601::isInputVoltageLimit(){
  return _readReg(KT_BQ25601_REG0A, KT_BQ25601_INPUT_VOLTAGE_LIMIT_STATUS_MASK, 6);
}

/*!
 *    @brief Is in IINDPM
 *    @return True if yes
 */
bool KT_BQ25601::isInputCurrentLimit(){
  return _readReg(KT_BQ25601_REG0A, KT_BQ25601_INPUT_CURRENT_LIMIT_STATUS_MASK, 5);
}

/*!
 *    @brief Is Top Off Timer counting
 *    @return True if yes
 */
bool KT_BQ25601::isTopOffTimerActive(){
  return _readReg(KT_BQ25601_REG0A, KT_BQ25601_TOPOFF_TIMER_ACTIVE_MASK, 3);
}

/*!
 *    @brief Is Device in ACOV
 *    @return True if yes
 */
bool KT_BQ25601::isOvervoltage(){
  return _readReg(KT_BQ25601_REG0A, KT_BQ25601_OVERVOLTAGE_STATUS_MASK, 2);
}

/*!
 *    @brief Enable Voltage INT Pulse
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableVoltageRegulationInterrupt(bool enable){
  return updateReg(!enable, KT_BQ25601_REG0A, KT_BQ25601_VINDPM_INT_MASK, 1);
}

/*!
 *    @brief Enable Current INT Pulse
 *    @param enable True to enable, False to disable
 *    @return True if success
 */
bool KT_BQ25601::enableCurrentRegulationInterrupt(bool enable){
  return updateReg(!enable, KT_BQ25601_REG0A, KT_BQ25601_IINDPM_INT_MASK, 0);
}

// REG0B

/*!
 *    @brief Resets all settings
 *    @return True if success
 */
bool KT_BQ25601::reset(){
  return updateReg(1, KT_BQ25601_REG0B, KT_BQ25601_RESET_MASK, 7);
}

/*!
 *    @brief Get Device Part Number (BQ2501 is 0010)
 *    @return Number in uint8_t
 */
uint8_t KT_BQ25601::getPartNumber(){
  return _readReg(KT_BQ25601_REG0B, KT_BQ25601_PART_NUMBER_MASK, 3);
}

/*!
 *    @brief Get Device Revision Number
 *    @return Number in uint8_t
 */
uint8_t KT_BQ25601::getDeviceRevision(){
  return _readReg(KT_BQ25601_REG0B, KT_BQ25601_DEV_REV_MASK, 0);
}