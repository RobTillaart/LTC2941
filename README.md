
[![Arduino CI](https://github.com/RobTillaart/LTC2941/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/LTC2941/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/LTC2941/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/LTC2941/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/LTC2941/actions/workflows/jsoncheck.yml)
[![GitHub issues](https://img.shields.io/github/issues/RobTillaart/LTC2941.svg)](https://github.com/RobTillaart/LTC2941/issues)

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/LTC2941/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/LTC2941.svg?maxAge=3600)](https://github.com/RobTillaart/LTC2941/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/robtillaart/library/LTC2941.svg)](https://registry.platformio.org/libraries/robtillaart/LTC2941)


# LTC2941

Arduino library for the LTC2941 battery charge and discharge (Coulomb) meter.


## Description

**Experimental**  **Work in progress**

This library is to use the LTC2941 Coulomb meter.
The LTC2941 indicates the accumulated charge and discharge of a battery
so you can see how "full" they are.

The device is in essence a current sensor, and integrates the measured
current with high resolution over time.

The device allows to set a low/high charge threshold.
These can trigger an alarm when the battery is nearly full / empty.
The device has a status register that shows the different alarms supported.

The LTC2941 can be configured. 
Read the datasheet to understand how to make a correct workable configuration
for your project setup.

Accuracy: ~1%

**Library is not tested with hardware yet.**

Feedback as always is welcome.


### Datasheet warning

_Do not apply this product to safety protection devices or emergency stop equipment,
and any other applications that may cause personal injury due to the product's failure._


### Hardware

Check datasheet!

```
         TOPVIEW LTC2941 (DCB)                 TOPVIEW LTC2941 (MSE)
           +---------------+                     +---------------+
   SENSE + | 1           6 | SENSE -     SENSE + | 1           8 | SENSE -
       GND | 2           5 | AL/CC           GND | 2           7 | AL/CC
       SCL | 3           4 | SDA             SCL | 3           6 | SDA
           | -           - |                  NC | 4           5 | NC
           +---------------+                     +---------------+
```


### Compatibles

The LTC2941 comes at least in two versions. 
Difference is the working temperature range.

The LTC2942 is partial compatible.  
The LTC2943 and LTC2944 are a superset of the LTC2941 and are expected 
to work with this library. Use at own risk.


### Related

Current sensors
- https://github.com/RobTillaart/ACS712 - ACS series current sensor
- https://github.com/RobTillaart/INA226 - family of current and power sensors
- https://github.com/RobTillaart/LTC2941 - this library

Other
- https://github.com/Seeed-Studio/Seeed_LTC2941


### Tested

Library is not tested with hardware yet.

TODO: Test on Arduino UNO and ESP32


## I2C

### I2C Address

The device has a fixed I2C address of 0x64 (100) so only one sensor per I2C bus can be used.
Do not forget appropriate pull up resistors on the I2C SDA and SCL lines.

If one needs more sensors there are some options.
- One could use an I2C multiplexer (see below)
- One could use an MCU with multiple I2C buses.
- One could use a (Two-Wire compatible) SW I2C (outside scope of this library).

Using the VCC as a Chip Select is not advised as the LTC2941
has a preheat time of 2 minutes.
Every time the power is shut off the pre-heat would run again internally.
It is unclear what effect this has on the lifetime and quality of the sensor.


### I2C multiplexing

Sometimes you need to control more devices than possible with the default
address range the device provides.
This is possible with an I2C multiplexer e.g. TCA9548 which creates up
to eight channels (think of it as I2C subnets) which can use the complete
address range of the device.

Drawback of using a multiplexer is that it takes more administration in
your code e.g. which device is on which channel.
This will slow down the access, which must be taken into account when
deciding which devices are on which channel.
Also note that switching between channels will slow down other devices
too if they are behind the multiplexer.

- https://github.com/RobTillaart/TCA9548


### I2C Performance

Only test **getCharge()** as that is the main function.


|  Clock     |  time (us)  |  Notes  |
|:----------:|:-----------:|:--------|
|   100 KHz  |             |  default
|   200 KHz  |             |
|   300 KHz  |             |
|   400 KHz  |             |
|   600 KHz  |             |
|   800 KHz  |             |
|   900 KHz  |             |  max datasheet


TODO: implement and run performance sketch on hardware.


## Interface

```cpp
#include "LTC2941.h"
```

### Constructor

- **LTC2941(TwoWire \*wire = &Wire)** optional select I2C bus.
- **bool begin()** checks if device is visible on the I2C bus.
- **bool isConnected()** Checks if device address can be found on I2C bus.
- **uint8_t getAddress()** Returns the fixed address 0x2A (42).


### Status and control

See datasheet for details.

- **bool getStatus(uint8_t &status)**

Status byte, see datasheet.

|  bit  |  description        |
|:-----:|:--------------------|
|   0   |  undervoltage alert |
|   1   |  Vbattery alert     |
|   2   |  charge alert low   |
|   3   |  charge alert high  |
|   4   |  not used, always 0 |
|   5   |  under/overflow     |
|   6   |  not used, always 0 |
|   7   |  chip ID, always 1  |


- **bool setControl(uint8_t control)**
- **bool getControl(uint8_t &control)**
- **bool setControl(uint8_t ALCC,  uint8_t prescaler,  uint8_t shutDown, uint8_t batAlert)**
- **bool getControl(uint8_t &ALCC, uint8_t &prescaler, uint8_t &shutDown, uint8_t &batAlert)**

Control fields, see datasheet.

|  bit  |  description        |  range  |  notes  |
|:-----:|:--------------------|:--------|:--------|
|   0   |  Shutdown           |   0..1  |
|  1-2  |  AL/CC configure    |   0..2  |  value 3 not allowed.
|  3-5  |  Coulomb pre-scaler |   0..7  |  maps to 1..128
|  6-7  |  battery alert      |   0..3  | 


### Charge

See datasheet page 10-11 for details.

- **bool setCharge(int16_t charge)**
- **bool getCharge(int16_t &charge)**
- **bool setChargeThresholdHigh(int16_t th)**
- **bool getChargeThresholdHigh(int16_t &th)**
- **bool setChargeThresholdLow(int16_t th)**
- **bool getChargeThresholdLow(int16_t &th)**


### Debug

- **int getLastError()** returns last error of low level communication.

|  value  |  error code  |
|:-------:|:-------------|
|   0x00  |  LTC2941_OK
|   0x10  |  LTC2941_NOT_READY
|   0x11  |  LTC2941_REQUEST_ERROR1
|   0x12  |  LTC2941_REQUEST_ERROR2


## Future

#### Must

- improve documentation
- get hardware to test

#### Should

- investigate datasheet in detail
  - missing core functions
  - config support, based upon page 10-11

#### Could

- improve control API
  - add constants per field. (enum?)
- investigate ARA call
  - alert response ADDRESS 0x0C 
  - page 13 bottom.
- add examples
- add unit tests (if possible)

#### Wont

- improve status API, function per field?

## Support

If you appreciate my libraries, you can support the development and maintenance.
Improve the quality of the libraries by providing issues and Pull Requests, or
donate through PayPal or GitHub sponsors.

Thank you,


