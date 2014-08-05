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
#include "InterruptRouter.h"

namespace tm4cpp
{
  class _GpioPort
  {
    public:

      _GpioPort();
      ~_GpioPort();

      void enable();
      void disable();
      void setup(const uint8_t &pins, const PinDirection &direction = PinDirectionOutput,
                 const PinStrength &strength = PinStrength2ma, const PinType &type = PinTypeStd);

      // writing pins
      void set(const uint8_t &pins, const uint8_t &value) const;
      void set(const uint8_t &pin, const bool &value) const;
      void operator()(const uint8_t &pin, bool value);
      void toggle(const uint8_t &pin) const;

      uint8_t get(const uint8_t &pins = 0xff) const;

      // interrupt handling and delegating
      void enableInterrupts(const uint8_t &pin, const uint32_t &eventType);
      void clearInterrupts();
      template <typename O, typename T> void setInterruptHandler(O *obj, void (T::*func)(uint8_t)) const;
      bool isInterruptHandlerSet();
      void clearInterruptHandler();

    protected:
      uint8_t  gpioEventIndex = 0;
      uint32_t gpioPeripheral = 0;
      uint32_t gpioBasePort = 0;
      uint32_t gpioIntPort = 0;
      bool isEnabled = false;

      InterruptDelegate portDelegates[8];
  };

  /**
   * Port implementation
   */

  inline _GpioPort::_GpioPort()
  {
  }

  inline void _GpioPort::enable()
  {
    MAP_SysCtlPeripheralEnable(gpioPeripheral);
    isEnabled = true;
  }

  inline void _GpioPort::disable()
  {
    MAP_SysCtlPeripheralDisable(gpioPeripheral);
    isEnabled = false;
  }

  inline _GpioPort::~_GpioPort()
  {
    clearInterrupts();
    disable();
    clearInterruptHandler();
  }

  inline void _GpioPort::setup(const uint8_t &pins, const PinDirection &direction, const PinStrength &strength, const PinType &type)
  {
    if (!isEnabled) {
      enable();
    }

    if (gpioIntPort == INT_GPIOF && (pins & 1)) {
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
      HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    }

    MAP_GPIODirModeSet(gpioBasePort, pins, direction);
    MAP_GPIOPadConfigSet(gpioBasePort, pins, strength, type);
  }

  inline void _GpioPort::set(const uint8_t &pins, const uint8_t &value) const
  {
    MAP_GPIOPinWrite(gpioBasePort, pins, value);
  }

  inline void _GpioPort::set(const uint8_t &pin, const bool &value) const
  {
    MAP_GPIOPinWrite(gpioBasePort, pin, value ? pin : 0x00);
  }

  inline void _GpioPort::operator()(const uint8_t &pin, bool value)
  {
    MAP_GPIOPinWrite(gpioBasePort, pin, value ? pin : 0x00);
  }

  inline void _GpioPort::toggle(const uint8_t &pin) const
  {
    int8_t pinValue = MAP_GPIOPinRead(gpioBasePort, pin) ^ pin;
    MAP_GPIOPinWrite(gpioBasePort, pin, pinValue);
  }

  inline uint8_t _GpioPort::get(const uint8_t &pins) const
  {
    return MAP_GPIOPinRead(gpioBasePort, pins);
  }

  inline void _GpioPort::enableInterrupts(const uint8_t &pin, const uint32_t &eventType)
  {
    MAP_IntMasterDisable();
    MAP_GPIOIntClear(gpioBasePort, pin);
    MAP_GPIOIntTypeSet(gpioBasePort, pin, eventType);
    MAP_GPIOIntEnable(gpioBasePort, pin);
    MAP_IntEnable(gpioIntPort);
    MAP_IntMasterEnable();
  }

  template <typename O, typename T>
  inline void _GpioPort::setInterruptHandler(O *obj, void (T::*func)(uint8_t)) const
  {
    InterruptDelegate id = fastdelegate::MakeDelegate(obj, func);
    InterruptRouter::addDelegate(gpioEventIndex, &id);
  }

  inline bool _GpioPort::isInterruptHandlerSet()
  {
    return InterruptRouter::isDelegateRegistered(gpioEventIndex);
  }

  inline void _GpioPort::clearInterruptHandler()
  {
    InterruptRouter::removeDelegate(gpioEventIndex);
  }

  inline void _GpioPort::clearInterrupts()
  {
    if (MAP_IntIsEnabled(gpioIntPort)) {
      MAP_IntDisable(gpioIntPort);
      MAP_GPIOIntClear(gpioBasePort, 0xff);
      MAP_GPIOIntDisable(gpioBasePort, 0xff);
    }
  }

#define GEN_GPIO_CLASS(name, letter) \
  class name: public _GpioPort \
  { \
    public: \
      name() \
      { \
        gpioEventIndex = intr::gpio::letter; \
        gpioPeripheral = gpio::letter::peripheral; \
        gpioBasePort = gpio::letter::basePort; \
        gpioIntPort = gpio::letter::intPort; \
      } \
  }; \

  GEN_GPIO_CLASS(GpioA, A);
  GEN_GPIO_CLASS(GpioB, B);
  GEN_GPIO_CLASS(GpioC, C);
  GEN_GPIO_CLASS(GpioD, D);
  GEN_GPIO_CLASS(GpioE, E);
  GEN_GPIO_CLASS(GpioF, F);
  GEN_GPIO_CLASS(GpioG, G);
  GEN_GPIO_CLASS(GpioH, H);
  GEN_GPIO_CLASS(GpioJ, J);
  GEN_GPIO_CLASS(GpioK, K);
  GEN_GPIO_CLASS(GpioL, L);
  GEN_GPIO_CLASS(GpioM, M);
  GEN_GPIO_CLASS(GpioN, N);

//  GEN_GPIO_CLASS(GpioP, P);
//  GEN_GPIO_CLASS(GpioQ, Q);
//  GEN_GPIO_CLASS(GpioR, R);
//  GEN_GPIO_CLASS(GpioS, S);

}
/* namespace tm4cpp */
