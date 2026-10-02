//
//    FILE: LTC2941_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: test basic charge reading
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

  Serial.print("CONT \tSTAT \tCHARGE\n");
}


void loop()
{
  uint16_t charge;
  LTC.getCharge(charge);
  uint8_t status;
  LTC.getStatus(status);
  uint8_t control;
  LTC.getControl(control);

  Serial.print(control, HEX);
  Serial.print("\t");
  Serial.print(status, HEX);
  Serial.print("\t");
  Serial.println(charge);
  delay(2000);
}


//  -- END OF FILE --
