/*
 * InterruptRouter.h
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "../application/EventHandler.h"
#include "../helpers/FixedPointerArray.h"

namespace tm4cpp
{
  static const uint8_t kMaxHandlersPerEvent = 10;
  static const uint8_t kEventVectorSize = 10;
  typedef helpers::FixedPointerArray<EventHandler, kMaxHandlersPerEvent> EventHandlerPointerArray;

  class InterruptRouter
  {
    public:
      static EventHandlerPointerArray __targets[kEventVectorSize];

//      static void initialize();
      static void handleGPIOInterrupt(const uint8_t index, const uint32_t flags);
  };

} /* namespace tm4cpp */
