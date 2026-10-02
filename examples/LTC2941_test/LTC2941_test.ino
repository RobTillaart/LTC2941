//
//    FILE: LTC2941_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: simple register test
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

  Serial.print("STAT \tCONTROL\n");
  uint8_t status;
  LTC.getStatus(status);
  uint8_t control;
  LTC.getControl(control);
  Serial.print(status, HEX);
  Serial.print("\t");
  Serial.println(control, HEX);


  Serial.print("getCharge: \t");
  //  save old value to restore
  uint16_t val;
  uint16_t nval;
  LTC.getCharge(val);
  val ^= 0xFFFF;
  LTC.setCharge(val);
  LTC.getCharge(nval);
  if (nval != (val))
  {
    Serial.println("Error");
  }
  else
  {
    Serial.println("OK");
  }
  //  restore
  LTC.setCharge(val ^ 0xFFFF);


  Serial.print("getThresholdHigh: \t");
  //  save old value to restore
  LTC.getThresholdHigh(val);
  val ^= 0xFFFF;
  LTC.setThresholdHigh(val);
  LTC.getThresholdHigh(nval);
  if (nval != val)
  {
    Serial.println("Error");
  }
  else
  {
    Serial.println("OK");
  }
  //  restore
  LTC.setThresholdHigh(val ^ 0xFFFF);


  Serial.print("getThresholdLow: \t");
  //  save old value to restore
  LTC.getThresholdLow(val);
  val ^= 0xFFFF;
  LTC.setThresholdLow(val);
  LTC.getThresholdHigh(nval);
  if (nval != val)
  {
    Serial.println("Error");
  }
  else
  {
    Serial.println("OK");
  }
  //  restore
  LTC.setThresholdLow(val ^ 0xFFFF);


  Serial.println("\ndone");
}


void loop()
{

}


//  -- END OF FILE --
