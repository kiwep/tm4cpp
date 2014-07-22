/*
 * Port.h
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include <stdint.h>
#include "driverlib/gpio.h"

namespace tm4cpp
{
  #define ClassTemplate template<typename TGpio>

  ClassTemplate
  class Port: TGpio
  {
    public:
      // constructors
      Port();
      Port(uint32_t direction, uint8_t pins);

      // setting up the port
      void setPinDirection(uint8_t pins, uint32_t direction) const;

      // writing pins
      void writePin(uint8_t pins, uint8_t value) const;
      void setPinHigh(uint8_t pin) const;
      void setPinLow(uint8_t pin) const;
      void togglePin(uint8_t pin) const;
  };

  /**
   * Port implementation
   */

  ClassTemplate
  inline Port<TGpio>::Port()
  {
    MAP_SysCtlPeripheralEnable(TGpio::peripheral);
  }

  ClassTemplate
  inline Port<TGpio>::Port(uint32_t direction, uint8_t pins)
  {
    Port();
    setPinDirection(pins, direction);
  }

  ClassTemplate
  inline void Port<TGpio>::setPinDirection(uint8_t pins, uint32_t direction) const
  {
    MAP_GPIODirModeSet(TGpio::basePort, pins, direction);
    MAP_GPIOPadConfigSet(TGpio::basePort, pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  }

  ClassTemplate
  inline void Port<TGpio>::writePin(uint8_t pins, uint8_t value) const
  {
    MAP_GPIOPinWrite(TGpio::basePort, pins, value);
  }

  ClassTemplate
  inline void Port<TGpio>::setPinHigh(uint8_t pin) const
  {
    MAP_GPIOPinWrite(TGpio::basePort, pin, pin);
  }

  ClassTemplate
  inline void Port<TGpio>::setPinLow(uint8_t pin) const
  {
    MAP_GPIOPinWrite(TGpio::basePort, pin, 0x00);
  }

  ClassTemplate
  inline void Port<TGpio>::togglePin(uint8_t pin) const
  {
    int8_t pinValue = MAP_GPIOPinRead(TGpio::basePort, pin) ^ pin;
    MAP_GPIOPinWrite(TGpio::basePort, pin, pinValue);
  }

} /* namespace tm4cpp */
