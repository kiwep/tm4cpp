/*
 * Port.h
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "mcu.h"
#include <stdint.h>
#include "driverlib/gpio.h"
#include "GpioPorts.h"
#include "../interrupts/InterruptRouter.h"
#include "../helpers/Empty.h"

namespace tm4cpp
{

  template<typename TGpio, typename IClass = _internal::Empty>
  class Port: public _internal::InterruptTarget
  {
      typedef void (IClass::*IHObjMemberFunc)(uint8_t);

      IClass *interruptHandlerObj;
      IHObjMemberFunc interruptHandlerMethod;

    public:

      Port();
      ~Port();

      void setupPins(const uint8_t &pins = 0xff, const uint32_t &direction = gpio::Output,
                     const uint32_t &strength = gpio::Strength2ma, const uint32_t &type = gpio::TypeStd) const;

      // writing pins
      void writePins(const uint8_t &pins, const uint8_t &value) const;
      void setPin(const uint8_t &pin, const bool &value) const;
      void setPinHigh(const uint8_t &pin) const;
      void setPinLow(const uint8_t &pin) const;
      void togglePin(const uint8_t &pin) const;

      uint8_t readPins(const uint8_t &pins = 0xff) const;
      bool getPin(const uint8_t &pin) const;

      // interrupt handling and delegating
      void enablePinInterrupts(const uint8_t &pin, const uint32_t &eventType);
      void clearPinInterrupts();
      void setInterruptCallback(IClass *obj, IHObjMemberFunc func);
      void _handleGPIOInterrupt(const uint8_t index, const uint32_t flags);

  };

  /**
   * Port implementation
   */

#define PortBaseTmpl(type) template<typename TGpio, typename IClass> inline type Port<TGpio, IClass>

  PortBaseTmpl()::Port()
  {
    interruptHandlerObj = NULL;
    interruptHandlerMethod = NULL;

    InterruptRouter::addHandler(this, TGpio::eventIndex);
    MAP_SysCtlPeripheralEnable(TGpio::peripheral);
  }

  PortBaseTmpl()::~Port()
  {
    clearPinInterrupts();
    MAP_SysCtlPeripheralDisable(TGpio::peripheral);
    InterruptRouter::removeHandler(TGpio::eventIndex);
  }

  PortBaseTmpl(void)::setupPins(const uint8_t &pins, const uint32_t &direction, const uint32_t &strength, const uint32_t &type) const
  {
    if (TGpio::intPort == INT_GPIOF && (pins & 1)) {
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
      HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    }

    MAP_GPIODirModeSet(TGpio::basePort, pins, direction);
    MAP_GPIOPadConfigSet(TGpio::basePort, pins, strength, type);
  }

  PortBaseTmpl(void)::writePins(const uint8_t &pins, const uint8_t &value) const
  {
    MAP_GPIOPinWrite(TGpio::basePort, pins, value);
  }

  PortBaseTmpl(void)::setPin(const uint8_t &pin, const bool &value) const
  {
    MAP_GPIOPinWrite(TGpio::basePort, pin, value ? pin : 0x00);
  }

  PortBaseTmpl(void)::setPinHigh(const uint8_t &pin) const
  {
    MAP_GPIOPinWrite(TGpio::basePort, pin, pin);
  }

  PortBaseTmpl(void)::setPinLow(const uint8_t &pin) const
  {
    MAP_GPIOPinWrite(TGpio::basePort, pin, 0x00);
  }

  PortBaseTmpl(void)::togglePin(const uint8_t &pin) const
  {
    int8_t pinValue = MAP_GPIOPinRead(TGpio::basePort, pin) ^ pin;
    MAP_GPIOPinWrite(TGpio::basePort, pin, pinValue);
  }

  PortBaseTmpl(uint8_t)::readPins(const uint8_t &pins) const
  {
    return MAP_GPIOPinRead(TGpio::basePort, pins);
  }

  PortBaseTmpl(bool)::getPin(const uint8_t &pin) const
  {
    return !!MAP_GPIOPinRead(TGpio::basePort, pin);
  }

  PortBaseTmpl(void)::enablePinInterrupts(const uint8_t &pin, const uint32_t &eventType)
  {
    MAP_IntMasterDisable();
    MAP_GPIOIntClear(TGpio::basePort, pin);
    MAP_GPIOIntTypeSet(TGpio::basePort, pin, eventType);
    MAP_GPIOIntEnable(TGpio::basePort, pin);
    MAP_IntEnable(TGpio::intPort);
    MAP_IntMasterEnable();

  }

  PortBaseTmpl(void)::setInterruptCallback(IClass *obj, IHObjMemberFunc func)
  {
    interruptHandlerObj = obj;
    interruptHandlerMethod = func;
  }

  PortBaseTmpl(void)::clearPinInterrupts()
  {
    if (MAP_IntIsEnabled(TGpio::intPort)) {
      MAP_IntDisable(TGpio::intPort);
      MAP_GPIOIntClear(TGpio::basePort, 0xff);
      MAP_GPIOIntDisable(TGpio::basePort, 0xff);
    }
  }

  PortBaseTmpl(void)::_handleGPIOInterrupt(const uint8_t index, const uint32_t flags)
  {
    if (interruptHandlerObj != NULL) {
      ((*interruptHandlerObj).*(interruptHandlerMethod))(flags);
    }
  }

}
/* namespace tm4cpp */
