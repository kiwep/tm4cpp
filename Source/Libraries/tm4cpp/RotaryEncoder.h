/*
 * RotaryEncoder.h
 *
 *  Created on: 2014.08.03.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "Runnable.h"
#include "Port.h"
#include "mcu.h"

namespace tm4cpp
{
  typedef enum RotaryEncoderType
  {
    RotaryEncoderTypeDetent,
    RotaryEncoderTypeContinous
  } RotaryEncoderType;

  class RotaryEncoder: public Runnable
  {
    public:
      RotaryEncoder();
      ~RotaryEncoder();

      void setup(_GpioPort *port, const Pin &pin_a, const Pin &pin_b, const RotaryEncoderType &encType, const PinType &type = PinTypeWeakPullup);

      template<typename O, typename T> void setEventHandler(O *obj, void (T::*func)(long, long));

      void handleInterrupt(uint8_t pins);
      void runLoop();

    private:
      _GpioPort *gpioPort = NULL;
      uint8_t pinA = 0;
      uint8_t pinB = 0;
      uint8_t enabledPins = 0;

      uint32_t debounceDelay = 500;
      uint8_t debounceMaxTilt = 1;

      volatile uint32_t intTime0 = 0;
      volatile uint32_t intTime1 = 0;
      volatile uint32_t intTilt0 = 0;
      volatile uint32_t intTilt1 = 0;
      volatile uint8_t intSignal0 = 0;
      volatile uint8_t intSignal1 = 0;

      volatile long currentSteps = 0;
      volatile long lastSteps = 0;

      fastdelegate::FastDelegate2<long, long> onChangeDelegate;
  };

  inline RotaryEncoder::RotaryEncoder()
  {
    Runtime::addToRunLoop(this);
  }

  inline RotaryEncoder::~RotaryEncoder()
  {
    Runtime::removeFromRunLoop(this);
  }

  inline void RotaryEncoder::setup(_GpioPort *port, const Pin &pin_a, const Pin &pin_b, const RotaryEncoderType &encType, const PinType &type)
  {
    gpioPort = port;
    enabledPins = pin_a | pin_b;
    pinA = pin_a;
    pinB = pin_b;

    if (encType == RotaryEncoderTypeContinous) {
      debounceMaxTilt = 3;
    }

    gpioPort->setup(enabledPins, PinDirectionInput, PinStrength2ma, type);
    if (!gpioPort->isInterruptHandlerSet()) {
      gpioPort->setInterruptHandler(this, &RotaryEncoder::handleInterrupt);
    }
    gpioPort->enableInterrupts(enabledPins, PinInterruptBothEdges);
  }

  inline void RotaryEncoder::handleInterrupt(uint8_t changedPins)
  {
    // A
    uint8_t pinVal = gpioPort->get(enabledPins);
    if (changedPins & pinA) {
      uint8_t p0 = pinVal & pinA;
      if (intSignal0 != p0) {
        intSignal0 = p0;
        if ((p0 > 0) && (intSignal1 == 0)) {
          if ((SystemTimer::since(intTime1) > debounceDelay) || intTilt0 > debounceMaxTilt) {
            currentSteps++;
            intTime0 = SystemTimer::milliseconds();
            intTilt1 = 0;
          }
          else {
            intTilt0++;
          }
        }
      }
    }

    // B
    if (changedPins & pinB) {
      uint8_t p1 = pinVal & pinB;
      if (intSignal1 != p1) {
        intSignal1 = p1;
        if ((p1 > 0) && (intSignal0 == 0)) {
          if ((SystemTimer::since(intTime0) > debounceDelay) || intTilt1 > debounceMaxTilt) {
            currentSteps--;
            intTime1 = SystemTimer::milliseconds();
            intTilt0 = 0;
          }
          else {
            intTilt1++;
          }
        }
      }
    }
  }

  inline void RotaryEncoder::runLoop()
  {
    if (currentSteps != lastSteps) {
      if (onChangeDelegate) {
        onChangeDelegate(currentSteps, lastSteps);
      }
      lastSteps = currentSteps;
    }
  }

  template<typename O, typename T>
  inline void RotaryEncoder::setEventHandler(O *obj, void (T::*func)(long, long))
  {
    onChangeDelegate = fastdelegate::MakeDelegate(obj, func);
  }

}  // namespace tm4cpp

