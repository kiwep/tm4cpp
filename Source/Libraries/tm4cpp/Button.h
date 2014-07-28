/*
 * Button.h
 *
 *  Created on: 2014.07.27.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "Runnable.h"
#include "Port.h"
#include "mcu.h"

namespace tm4cpp
{
  namespace button
  {
    enum button {
      Down,
      Up,
      Press
    };
  }  // namespace button

  class Button: public Runnable
  {
    public:
      Button();
      ~Button();

      void setup(GpioPortBase *port, const uint8_t &pins, const uint32_t &type = gpio::TypeWeakPullup);

      void setRepeatDelay(const uint32_t &delay);
      void setRepeatInterval(const uint32_t &interval);
      void setLongPressDuration(const uint32_t &duration);

      template<typename O, typename T> void setEventHandler(O *obj, void (T::*func)(uint8_t), const uint8_t &type);

      void handleInterrupt(uint8_t pins);
      void runLoop();

    private:
      GpioPortBase *_port;
      uint8_t _pins;
  };

  inline Button::Button()
  {
    _port = NULL;
    _pins = 0;
    Runtime::addToRunLoop(this);
  }

  inline Button::~Button()
  {
    Runtime::removeFromRunLoop(this);
  }

  inline void Button::runLoop()
  {
    _log("button is running\n");
  }

  inline void Button::setup(GpioPortBase *port, const uint8_t &pins, const uint32_t &type)
  {
    _port = port;
    _pins = pins;

    _port->setup(gpio::Pin0 | gpio::Pin4, gpio::Input, gpio::Strength2ma, type);
    if (!_port->isInterruptHandlerSet()) {
      _port->setInterruptHandler(this, &Button::handleInterrupt);
    }
  }

  template <typename O, typename T>
  inline void Button::setEventHandler(O *obj, void (T::*func)(uint8_t), const uint8_t &type)
  {
  }

  inline void Button::handleInterrupt(uint8_t pins)
  {
  }

}  // namespace tm4cpp

