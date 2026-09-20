//
//    FILE: LTC2941_status.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: split status bits.
//     URL: https://github.com/RobTillaart/LTC2941


#include "LTC2941.h"

LTC2941 LTC;


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("LTC2941_LIB_VERSION: ");
  Serial.println(LTC2941_LIB_VERSION);
  Serial.println();

  Wire.begin();
  bool b = LTC.begin();
  Serial.print("BEGIN:\t");
  Serial.println(b);
  Serial.print("ADDR:\t");
  Serial.println(LTC.getAddress());

  Serial.print("CHARGE \tCONT \tSTAT\n");
}


void loop()
{
  uint16_t charge;
  LTC.getCharge(charge);
  uint8_t status;
  LTC.getStatus(status);
  uint8_t control;
  LTC.getControl(control);

  Serial.print(charge);
  Serial.print("\t");
  Serial.print(control, HEX);
  Serial.print("\t");
  Serial.print(status, HEX);
  Serial.print("\t");
  //  check all status fields, 
  //  multiple alerts are possible
  if (status & LTC2941_ALERT_UNDERVOLTAGE) Serial.print("undervoltage\t");
  if (status & LTC2941_ALERT_BATTERY) Serial.print("battery\t");
  if (status & LTC2941_ALERT_CHARGE_LOW) Serial.print("chargeLow\t");
  if (status & LTC2941_ALERT_CHARGE_HIGH) Serial.print("chargeHigh\t");
  if (status & LTC2941_ALERT_UNDEROVERFLOW) Serial.print("overflow\t");
  Serial.println();

  delay(1000);
}


//  -- END OF FILE --
