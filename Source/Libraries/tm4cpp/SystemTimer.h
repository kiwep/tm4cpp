/*
 * SystemTimer.h
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include <stdint.h>

namespace tm4cpp
{

  class SystemTimer
  {
    public:
      volatile static uint32_t __counter;

      static void initialize();
      static void delay(uint32_t milliseconds);
      static uint32_t milliseconds();
      static void reset();
  };

  inline void SystemTimer::delay(uint32_t milliseconds)
  {
    uint32_t target = __counter + milliseconds;
    while (__counter < target)
      ;
  }

  inline uint32_t SystemTimer::milliseconds()
  {
    return __counter;
  }

  inline void SystemTimer::reset()
  {
    __counter = 0;
  }

} /* namespace tm4cpp */

