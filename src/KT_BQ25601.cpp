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

bool KT_BQ25601::begin(TwoWire &wire){

  _wire = &wire;

  if(!isConnected()){

    if (_debugPort != nullptr) _debugPort->println("BQ25601 is not connected!");
    return false;
  }

  if (!setWatchdog(KT_BQ25601_WATCHDOG_NONE)) {

    if (_debugPort != nullptr) {
      _debugPort->println("Failed to disable Watchdog!");
    }

    return false;
  }

  return true;
}

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

// REG00

bool KT_BQ25601::enableHIZ(bool enable){
  return updateReg(enable, KT_BQ25601_REG00, KT_BQ25601_ENABLE_HIZ_MASK, 7);
}

bool KT_BQ25601::enableSTAT(kt_bq25601_enable_stat_enum enable){
  return updateReg(enable, KT_BQ25601_REG00, KT_BQ25601_ENABLE_STAT_MASK, 5);
}

bool KT_BQ25601::setInputCurrentLimit(uint16_t current_mA){

  if(current_mA < 100) current_mA = 100;
  else if(current_mA > 3200) current_mA = 3200;

  current_mA = (current_mA - 100) / 100;

  return updateReg(current_mA, KT_BQ25601_REG00, KT_BQ25601_SET_INPUT_CURRENT_LIMIT_MASK, 0);
}

// REG01

bool KT_BQ25601::enableCharging(bool enable){
  return updateReg(enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_CHARGING_MASK, 4);
}

bool KT_BQ25601::enableOTG(bool enable){
  return updateReg(enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_OTG_MASK, 5);
}

bool KT_BQ25601::resetWatchdog(){
  return updateReg(1, KT_BQ25601_REG01, KT_BQ25601_RESET_WATCHDOG_MASK, 6);
}

bool KT_BQ25601::setMinVoltage(kt_bq25601_sys_min_voltage_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG01, KT_BQ25601_SYS_MIN_VOLTAGE_MASK, 1);
}

bool KT_BQ25601::minVoltageOTG(kt_bq25601_min_voltage_otg_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG01, KT_BQ25601_MIN_VOLTAGE_OTG_MASK, 0);
}

bool KT_BQ25601::enablePFM(bool enable){
  return updateReg(!enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_PFM_MASK, 7);
}

// REG02

bool KT_BQ25601::setCurrentLImitOTG(kt_bq25601_current_limit_otg_enum limit){
  return updateReg(limit, KT_BQ25601_REG02, KT_BQ25601_SET_CURRENT_LIMIT_OTG_MASK, 7);
}

bool KT_BQ25601::enableQ1FullOn(bool enable) {
  return updateReg(enable, KT_BQ25601_REG02, KT_BQ25601_Q1_FULLON_MASK, 6);
}

bool KT_BQ25601::setChargeCurrent(uint16_t current_mA){
    if(current_mA > 3000) current_mA = 3000;

    current_mA /= 60;

    return updateReg(current_mA, KT_BQ25601_REG02, KT_BQ25601_CHARGE_CURRENT_MASK, 0);
}

// REG03

bool KT_BQ25601::setPrechargeCurrent(uint16_t current_mA){
  if(current_mA < 60) current_mA = 60;
  else if(current_mA > 960) current_mA = 960;

  current_mA = (current_mA - 60) / 60;

  return updateReg(current_mA, KT_BQ25601_REG03, KT_BQ25601_PRECHARGE_CURRENT_MASK, 4);
}

bool KT_BQ25601::setTerminationCurrent(uint16_t current_mA){
  if(current_mA < 60) current_mA = 60;
  else if(current_mA > 960) current_mA = 960;

  current_mA = (current_mA - 60) / 60;

  return updateReg(current_mA, KT_BQ25601_REG03, KT_BQ25601_TERMINATION_CURRENT_MASK, 0);
}

// REG04

bool KT_BQ25601::setChargeVoltage(uint16_t voltage_mV){
  if(voltage_mV < 3856) voltage_mV = 3856;
  else if(voltage_mV > 4624) voltage_mV = 4624;

  voltage_mV = (voltage_mV - 3856) / 32;

  return updateReg(voltage_mV, KT_BQ25601_REG04, KT_BQ25601_CHARGE_VOLTAGE_MASK, 3);
}

bool KT_BQ25601::setTopOffTimer(kt_bq25601_topoff_timer_enum time){
  return updateReg(time, KT_BQ25601_REG04, KT_BQ25601_TOPOFF_TIMER_MASK, 1);
}

bool KT_BQ25601::setRechargeThreshold(kt_bq25601_recharge_threshold_enum threshold){
  return updateReg(threshold, KT_BQ25601_REG04, KT_BQ25601_RECHARGE_THRESHOLD_MASK, 0);
}

// REG05

bool KT_BQ25601::setWatchdog(kt_bq25601_watchdog_enum watchdog){
  return updateReg(watchdog, KT_BQ25601_REG05, KT_BQ25601_SET_WATCHDOG_MASK, 4);
}

bool KT_BQ25601::enableTermination(bool enable){
  return updateReg(enable, KT_BQ25601_REG05, KT_BQ25601_ENABLE_TERMINATION_MASK, 7);
}

bool KT_BQ25601::enableSafetyTimer(bool enable){
  return updateReg(enable, KT_BQ25601_REG05, KT_BQ25601_ENABLE_SAFETY_TIMER_MASK, 3);
}

bool KT_BQ25601::enableChargeSafetyTimer(bool enable){
  return updateReg(enable, KT_BQ25601_REG05, KT_BQ25601_ENABLE_CHARGE_SAFETY_TIMER_MASK, 2);
}

bool KT_BQ25601::setThermalRegulationThreshold(kt_bq25601_thermal_regulation_threshold_enum threshold){
  return updateReg(threshold, KT_BQ25601_REG05, KT_BQ25601_THERMAL_REGULATION_THRESHOLD_MASK, 1);
}

bool KT_BQ25601::setCoolTemperatureCurrentLimit(kt_bq25601_cool_temperature_current_limit_enum limit){
  return updateReg(limit, KT_BQ25601_REG05, KT_BQ25601_COOL_TEMPERATURE_CURRENT_LIMIT_MASK, 0);
}

// REG06

bool KT_BQ25601::setOVP(kt_bq25601_ovp_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG06, KT_BQ25601_OVP_MASK, 6);
}

bool KT_BQ25601::setRegulationVoltageOTG(kt_bq25601_regulation_voltage_otg_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG06, KT_BQ25601_REGULATION_VOLTAGE_OTG_MASK, 4);
}

bool KT_BQ25601::setInputVoltageThreshold(uint16_t voltage_mV){
  if(voltage_mV < 3900) voltage_mV = 3900;
  else if(voltage_mV > 5400) voltage_mV = 5400;

  voltage_mV = (voltage_mV - 3900) / 100;

  return updateReg(voltage_mV, KT_BQ25601_REG06, KT_BQ25601_INPUT_VOLTAGE_THRESHOLD_MASK, 0);
}

// REG07

bool KT_BQ25601::enableInputCurrentDetectionLimit(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_INPUT_CURRENT_DETECTION_LIMIT_MASK, 7);
}

bool KT_BQ25601::slowSafetyTimer(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_SLOW_SAFETY_TIMER_MASK, 6);
}

bool KT_BQ25601::enableShippingMode(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_SHIPPING_MODE_MASK, 5);
}

bool KT_BQ25601::setWarmTemperatureVoltageSetting(kt_bq25601_warm_temperature_voltage_setting_enum voltage){
  return updateReg(voltage, KT_BQ25601_REG07, KT_BQ25601_WARM_TEMPERATURE_VOLTAGE_SETTING_MASK, 4);
}

bool KT_BQ25601::setBatfetDelay(kt_bq25601_batfet_delay_enum delay){
  return updateReg(delay, KT_BQ25601_REG07, KT_BQ25601_BATTFET_DELAY_MASK, 3);
}

bool KT_BQ25601::enableBatfetResetFunction(bool enable){
  return updateReg(enable, KT_BQ25601_REG07, KT_BQ25601_BATTFET_RESET_FUNCTION_MASK, 2);
}

bool KT_BQ25601::setBatTracking(kt_bq25601_bat_track_enum track){
  return updateReg(track, KT_BQ25601_REG07, KT_BQ25601_BAT_TRACK_MASK, 0);
}

