//
//    FILE: LTC2941.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2026-09-18
// VERSION: 0.1.0
// PURPOSE: Arduino library for the LTC2941 battery charge and discharge (Coulomb) meter.
//     URL: https://github.com/RobTillaart/LTC2941


#include "LTC2941.h"


const uint8_t LTC2941_REG_STATUS      = 0x00;
const uint8_t LTC2941_REG_CONTROL     = 0x01;
const uint8_t LTC2941_REG_CHARGE_MSB  = 0x02;
const uint8_t LTC2941_REG_CHARGE_LSB  = 0x03;
const uint8_t LTC2941_REG_TH_HIGH_MSB = 0x04;
const uint8_t LTC2941_REG_TH_HIGH_LSB = 0x05;
const uint8_t LTC2941_REG_TH_LOW_MSB  = 0x06;
const uint8_t LTC2941_REG_TH_LOW_LSB  = 0x07;


LTC2941::LTC2941(TwoWire *wire)
{
  _address = 0x64;
  _wire = wire;
}

bool LTC2941::begin()
{
  //  reset variables
  _error = 0;
  if (! isConnected())
  {
    return false;
  }
  return true;
}

bool LTC2941::isConnected()
{
  _wire->beginTransmission(_address);
  return (_wire->endTransmission() == 0);
}

uint8_t LTC2941::getAddress()
{
  return _address;
}


/////////////////////////////////////////////
//
//  STATUS & CONTROL
//
bool LTC2941::getStatus(uint8_t &status)
{
  uint8_t value;
  _read(LTC2941_REG_STATUS, &value, 1);
  status = value;
  return (_error == LTC2941_OK);
}

bool LTC2941::setControl(uint8_t control)
{
  //  not allowed bit pattern. Page 9.
  if (control & 0x06)
  {
    _error = LTC2941_CONTROL_ERROR;
    return false;
  }
  uint8_t value = control;
  _write(LTC2941_REG_CONTROL, &value, 1);
  return (_error == LTC2941_OK);
}

bool LTC2941::getControl(uint8_t &control)
{
  uint8_t value;
  _read(LTC2941_REG_CONTROL, &value, 1);
  control = value;
  return (_error == LTC2941_OK);
}


bool LTC2941::setControl( uint8_t ALCC,
                          uint8_t prescaler,
                          uint8_t shutDown,
                          uint8_t batAlert)
{
  if ((ALCC > 2) || (shutDown > 1) || (batAlert > 3))
  {
    _error = LTC2941_CONTROL_ERROR;
    return false;
  }
  uint8_t control = shutDown | (ALCC << 1) | (prescaler << 3) | (batAlert << 6);
  return setControl(control);
}


bool LTC2941::getControl( uint8_t &ALCC,
                          uint8_t &prescaler,
                          uint8_t &shutDown,
                          uint8_t &batAlert)
{
  uint8_t control;
  if (getControl(control) != LTC2941_OK) return false;
  shutDown  = control & 0x01;
  ALCC      = (control >> 1) & 0x03;
  prescaler = (control >> 3) & 0x07;
  batAlert  = (control >> 6) & 0x03;
  return true;
}


/////////////////////////////////////////////
//
//  CHARGE
//
bool LTC2941::setCharge(int16_t charge)
{
  uint8_t value[2];
  value[0] = charge >> 8;
  value[1] = charge & 0xFF;
  _write(LTC2941_REG_CHARGE_MSB, value, 2);
  return (_error == LTC2941_OK);
}

bool LTC2941::getCharge(int16_t &charge)
{
  uint8_t value[2] = {0, 0};
  _read(LTC2941_REG_CHARGE_MSB, value, 2);
  charge = (value[0] << 8) + value[1];
  return (_error == LTC2941_OK);
}


/////////////////////////////////////////////
//
//  THRESHOLD
//
bool LTC2941::setThresholdHigh(int16_t th)
{
  uint8_t value[2];
  value[0] = th >> 8;
  value[1] = th & 0xFF;
  _write(LTC2941_REG_TH_HIGH_MSB, value, 2);
  return (_error == LTC2941_OK);
}

bool LTC2941::getThresholdHigh(int16_t &th)
{
  uint8_t value[2];
  _read(LTC2941_REG_CHARGE_MSB, value, 2);
  th = (value[0] << 8) + value[1];
  return (_error == LTC2941_OK);
}

bool LTC2941::setThresholdLow(int16_t th)
{
  uint8_t value[2];
  value[0] = th >> 8;
  value[1] = th & 0xFF;
  _write(LTC2941_REG_TH_LOW_MSB, value, 2);
  return (_error == LTC2941_OK);
}

bool LTC2941::getThresholdLow(int16_t &th)
{
  uint8_t value[2];
  _read(LTC2941_REG_TH_LOW_MSB, value, 2);
  th = (value[0] << 8) + value[1];
  return (_error == LTC2941_OK);
}


/////////////////////////////////////////////
//
//  DEBUG
//
int LTC2941::getLastError()
{
  int e = _error;
  _error = LTC2941_OK;
  return e;
}


///////////////////////////////////////////////
//
//  PRIVATE
//
int LTC2941::_write(uint8_t reg, uint8_t * arr, uint8_t size)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  for (uint8_t i = 0; i < size; i++)
  {
    _wire->write(arr[i]);
  }
  _error = _wire->endTransmission();
  return _error;
}

int LTC2941::_read(uint8_t reg, uint8_t * arr, uint8_t size)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _error = _wire->endTransmission();
  if (_error != LTC2941_OK)
  {
    return _error;
  }
  uint8_t bytes = _wire->requestFrom(_address, size);
  if (bytes == 0)
  {
    _error = LTC2941_REQUEST_ERROR1;
    return _error;
  }
  if (bytes < size)
  {
    _error = LTC2941_REQUEST_ERROR2;
    return _error;
  }

  for (uint8_t i = 0; i < size; i++)
  {
    arr[i] = _wire->read();
  }
  _error = LTC2941_OK;
  return LTC2941_OK;
}


//  -- END OF FILE --

