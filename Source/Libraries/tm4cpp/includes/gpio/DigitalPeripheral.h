/*
 * DigitalPeripheral.h
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include <stdint.h>
#include "driverlib/gpio.h"

namespace tm4cpp
{
  template<typename TGpio>
  class DigitalPeripheral
  {
      static const TGpio &_gpio;

    public:
      DigitalPeripheral()
      {
        MAP_SysCtlPeripheralEnable(_gpio.peripheral);
      }

      DigitalPeripheral(uint32_t direction, uint8_t pins)
      {
        DigitalPeripheral();
        setPinDirections(direction, pins);
      }

      void setPinDirections(uint32_t direction, uint8_t pins) const
      {
        MAP_GPIODirModeSet(_gpio.basePort, pins, direction);
        MAP_GPIOPadConfigSet(_gpio.basePort, pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
      }

      void writePins(uint8_t pins, uint8_t value) const
      {
        GPIOPinWrite(_gpio.basePort, pins, value);
      }

      void setPin(uint8_t pin, bool value) const
      {
        GPIOPinWrite(_gpio.basePort, pin, value ? 0xFF : 0x00);
      }

      void togglePin(uint8_t pin) const
      {
        int8_t pinValue = MAP_GPIOPinRead(_gpio.basePort, pin) ^ pin;
        GPIOPinWrite(_gpio.basePort, pin, pinValue);
      }
  };

} /* namespace tm4cpp */
