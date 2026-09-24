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

bool KT_BQ25601::enableHIZ(bool enable){
  return updateReg(enable, KT_BQ25601_REG00, KT_BQ25601_ENABLE_HIZ_MASK, 7);
}

bool KT_BQ25601::enableSTAT(kt_bq25601_min_voltage_otg_enum enable){
  return updateReg(enable, KT_BQ25601_REG00, KT_BQ25601_ENABLE_STAT_MASK, 5);
}

bool KT_BQ25601::setInputCurrentLimit(uint16_t current_mA){

  if(current_mA < 100) current_mA = 100;
  else if(current_mA > 3200) current_mA = 3200;

  current_mA = (current_mA - 100) / 100;

  return updateReg(current_mA, KT_BQ25601_REG00, KT_BQ25601_SET_INPUT_CURRENT_LIMIT_MASK, 0);
}

bool KT_BQ25601::enableCharging(bool enable){
  return updateReg(enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_CHARGING_MASK, 4);
}

bool KT_BQ25601::enableOTG(bool enable){
  return updateReg(enable, KT_BQ25601_REG01, KT_BQ25601_ENABLE_OTG_MASK, 5);
}

bool KT_BQ25601::setWatchdog(kt_bq25601_watchdog_enum watchdog){
  return updateReg(watchdog, KT_BQ25601_REG05, KT_BQ25601_SET_WATCHDOG_MASK, 4);
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

bool KT_BQ25601::setCurrentLImitOTG(kt_bq25601_current_limit_otg_enum limit){
  return updateReg(limit, KT_BQ25601_REG02, KT_BQ25601_SET_CURRENT_LIMIT_OTG_MASK, 7);
}

bool KT_BQ25601::enableQ1FullOn(bool enable) {
  return updateReg(enable, KT_BQ25601_REG02, KT_BQ25601_Q1_FULLON_MASK, 6);
}

bool KT_BQ25601::setChargeCurrent(uint16_t current_mA){
  if(current_mA < 0) current_mA = 100;
  else if(current_mA > 3000) current_mA = 3000;

  current_mA /= 60;

  return updateReg(current_mA, KT_BQ25601_REG02, KT_BQ25601_CHARGE_CURRENT_MASK, 0);
}

