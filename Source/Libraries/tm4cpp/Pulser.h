/*
 * Pulser.h
 *
 *  Created on: 2014.07.30.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "Runnable.h"
#include "Port.h"
#include "mcu.h"

namespace tm4cpp
{

  class Pulser: public Runnable
  {
    public:
      Pulser();
      ~Pulser();

      void setup(GpioPortBase *port, const uint8_t &pins);

      void emit(const uint8_t &pin);
      void emitWhen(const bool &condition, const uint8_t &pin);
      void operator()(const uint8_t &pin);

      void setPulseLength(const uint32_t &length);
      uint32_t getPulseLength() const;

      void runLoop();

    private:
      GpioPortBase *gpioPort;
      uint8_t enabledPins;
      uint8_t currentButtonMask;
      uint32_t times[8];
      uint8_t activePinCount;
      uint32_t pulseLength;
  };

  inline Pulser::Pulser() : gpioPort(NULL), enabledPins(0), currentButtonMask(0), activePinCount(0), pulseLength(25)
  {
    for (uint8_t i = 0; i < 8; i++) {
      times[i] = 0;
    }

    Runtime::addToRunLoop(this);
  }

  inline Pulser::~Pulser()
  {
    Runtime::removeFromRunLoop(this);
  }

  inline void Pulser::setup(GpioPortBase *port, const uint8_t &pins)
  {
    gpioPort = port;
    enabledPins = pins;

    gpioPort->setup(enabledPins);
  }

  inline void Pulser::emit(const uint8_t &pin)
  {
    uint32_t currentTime = SystemTimer::milliseconds();

    for (uint8_t i = 0; i < 8; i++) {
      if (pin & (1 << i)) {
        times[i] = currentTime;
        activePinCount++;
      }
    }

    currentButtonMask = currentButtonMask | pin;
    gpioPort->set(enabledPins, currentButtonMask);
  }

  inline void Pulser::operator()(const uint8_t &pin)
  {
    emit(pin);
  }

  inline void Pulser::emitWhen(const bool &condition, const uint8_t &pin)
  {
    if (!condition) {
      return;
    }

    emit(pin);
  }

  inline void Pulser::runLoop()
  {
    if (activePinCount > 0) {
      bool shouldUpdatePins = false;
      for (uint8_t i = 0; i < 8; i++) {
        uint32_t ptime = times[i];
        if (ptime > 0 && SystemTimer::since(ptime) > pulseLength) {
          times[i] = 0;
          currentButtonMask = currentButtonMask ^ (1 << i);
          shouldUpdatePins = true;
          activePinCount--;
        }
      }

      if (shouldUpdatePins) {
        gpioPort->set(enabledPins, currentButtonMask);
      }
    }
  }

  inline void Pulser::setPulseLength(const uint32_t &length)
  {
    pulseLength = length;
  }

  inline uint32_t Pulser::getPulseLength() const
  {
    return pulseLength;
  }


}  // namespace tm4cpp

