/*
 * InterruptRouter.h
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

//#include "../application/EventHandler.h"
//#include "../helpers/FixedPointerArray.h"
//#include "../../includes/gpio/Port.h"
#include "../../includes/gpio/GpioPorts.h"
#include "InterruptTarget.h"

namespace tm4cpp
{
  static const uint8_t kInterruptVectorSize = 17;

  class InterruptRouter
  {
    public:
      static _internal::InterruptTarget *__interruptTargets[kInterruptVectorSize];

      static void initialize();
      static void addHandler(_internal::InterruptTarget *handler, const uint8_t index);
      static void removeHandler(const uint8_t index);

  };

} /* namespace tm4cpp */
