#pragma once
//    FILE: LTC2941.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
//    DATE: 2026-09-18
// PURPOSE: Arduino library for the LTC2941 Coulomb meter
//     URL: https://github.com/RobTillaart/LTC2941
//
//  Read the datasheet for the details


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

  bool     begin();
  bool     isConnected();
  uint8_t  getAddress();


  //       STATUS & CONTROL
  //       check getLastError
  //       datasheet page 8
  bool     getStatus(uint8_t &status);
  //       datasheet page 9
  bool     setControl(uint8_t control);
  bool     getControl(uint8_t &control);
  //       control register per field     //  VALUES
  bool     setControl(uint8_t ALCC,       //  0..2
                      uint8_t prescaler,  //  0..7
                      uint8_t shutDown,   //  0..1
                      uint8_t batAlert);  //  0..3
  bool     getControl(uint8_t &ALCC,
                      uint8_t &prescaler,
                      uint8_t &shutDown,
                      uint8_t &batAlert);


  //       CHARGE
  bool     setCharge(int16_t charge);
  bool     getCharge(int16_t &charge);

  //       THRESHOLD
  bool     setThresholdHigh(int16_t th);
  bool     getThresholdHigh(int16_t &th);
  bool     setThresholdLow(int16_t th);
  bool     getThresholdLow(int16_t &th);


  //       DEBUG
  int      getLastError();


private:
  uint8_t  _address = 0x64;
  TwoWire* _wire;

  int      _write(uint8_t reg, uint8_t * arr, uint8_t size);
  int      _read(uint8_t reg, uint8_t * arr, uint8_t size);

  int      _error;
};


//  -- END OF FILE --
