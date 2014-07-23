/*
 * InterruptTarget.h
 *
 *  Created on: 2014.07.22.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include <stdint.h>

namespace tm4cpp
{
  namespace _internal
  {
    class InterruptTarget
    {
      public:
        virtual void _handleGPIOInterrupt(const uint8_t index, const uint32_t flags)
        {
        }

        virtual ~InterruptTarget()
        {
        }
    };

  }
}


