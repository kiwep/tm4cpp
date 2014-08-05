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
      static uint32_t since(uint32_t time);
      static uint32_t distance(const uint32_t &time1, const uint32_t &time2);
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

  inline uint32_t SystemTimer::since(uint32_t time)
  {
    return (__counter >= time) ? __counter - time : UINT32_MAX - time + __counter;
  }

  inline uint32_t SystemTimer::distance(const uint32_t &time1, const uint32_t &time2)
  {
    return (time2 >= time1) ? time2 - time1 : UINT32_MAX - time1 + time2;
  }

} /* namespace tm4cpp */

