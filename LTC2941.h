#pragma once
//    FILE: LTC2941.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
//    DATE: 2026-09-18
// PURPOSE: Arduino library for the LTC2941 battery charge and discharge (Coulomb) meter.
//     URL: https://github.com/RobTillaart/LTC2941
//
//
//  READ THE DATASHEET FOR THE DETAILS


#include "Arduino.h"
#include "Wire.h"


#define LTC2941_LIB_VERSION         (F("0.1.0"))

//  ERROR CODES
//  values <> 0 are errors.
const uint8_t LTC2941_OK             = 0x00;
const uint8_t LTC2941_NOT_READY      = 0x10;
const uint8_t LTC2941_REQUEST_ERROR1 = 0x11;
const uint8_t LTC2941_REQUEST_ERROR2 = 0x12;
const uint8_t LTC2941_CONTROL_ERROR  = 0x13;


//  STATUS MASKS
const uint8_t LTC2941_ALERT_UNDERVOLTAGE  = 0x01;
const uint8_t LTC2941_ALERT_BATTERY       = 0x02;
const uint8_t LTC2941_ALERT_CHARGE_LOW    = 0x04;
const uint8_t LTC2941_ALERT_CHARGE_HIGH   = 0x08;
const uint8_t LTC2941_ALERT_UNDEROVERFLOW = 0x20;
const uint8_t LTC2941_CHIP_ID             = 0x80;


class LTC2941
{
public:
  LTC2941(TwoWire *wire = &Wire);

  //  RSense > 0.005 Ohm.
  //  RSense < 50 mVolt / Imax  TODO - readme
  //  mAh is the capacity of the battery.
  bool     begin(float RSense, float mAh);
  bool     isConnected();
  uint8_t  getAddress();


  //       below = register level access, use with care.

  //       CHARGE
  //       raw values have to be multiplied/divided by QLSB.
  bool     setRawCharge(uint16_t raw);
  bool     getRawCharge(uint16_t &raw);
  bool     setCharge(float charge);
  bool     getCharge(float &charge);
  bool     getPercentageFull(float &percentage);
  float    getQLSB();


  //       STATUS & CONTROL
  //       check getLastError
  //       datasheet page 8
  bool     getStatus(uint8_t &status);
  //       datasheet page 9
  bool     setControl(uint8_t control);
  bool     getControl(uint8_t &control);
  //       control register per field     //  VALUES
  bool     setControl(uint8_t ALCC,       //  0..2
                      uint8_t prescaler,  //  0..7 maps to 1,2,4,8,...128
                      uint8_t shutDown,   //  0..1
                      uint8_t batAlert);  //  0..3
  bool     getControl(uint8_t &ALCC,
                      uint8_t &prescaler,
                      uint8_t &shutDown,
                      uint8_t &batAlert);


  //       THRESHOLD
  //       raw values have to be multiplied/divided by QLSB.
  bool     setRawThresholdHigh(uint16_t th);
  bool     getRawThresholdHigh(uint16_t &th);
  bool     setRawThresholdLow(uint16_t th);
  bool     getRawThresholdLow(uint16_t &th);
  bool     setThresholdHigh(float th);
  bool     getThresholdHigh(float &th);
  bool     setThresholdLow(float th);
  bool     getThresholdLow(float &th);


  //       DEBUG
  int      getLastError();
  //  TODO keep?
  uint8_t  getPrescaler() { return _prescaler; };
  float    getMaxCurrent() { return 0.050f / _RSense; };

private:
  uint8_t  _address = 0x64;
  TwoWire* _wire;

  //  datasheet page 4
  float    _RSense    = 0.050f;  //  50 mOhm (page 4)
  float    _mAh       = 0;       //  not initialized, aka Qbat
  uint8_t  _prescaler = 128;     //  default POR (page 7)
  //  _QLSB = 0.085f * 0.050f / _RSense * _prescaler / 128;
  float    _QLSB      = 0.085f;

  void     _calculateQLSB();

  int      _write(uint8_t reg, uint8_t * arr, uint8_t size);
  int      _read(uint8_t reg, uint8_t * arr, uint8_t size);

  int      _error;
};


//  -- END OF FILE --
