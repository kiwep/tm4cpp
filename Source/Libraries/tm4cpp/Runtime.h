/*
 * Runtime.h
 *
 *  Created on: 2014.07.28.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once
#include <stdint.h>
#include "helpers/FixedPointerArray.h"
#include "Runnable.h"

namespace tm4cpp
{
  typedef helpers::FixedPointerArray<Runnable, 32> RunnablePointerArray;
  class Runtime
  {
    public:
      static RunnablePointerArray __handlers;

      static void addToRunLoop(Runnable *runnable);
      static void removeFromRunLoop(Runnable *runnable);
      static void runLoop();
  };

}  // namespace tm4cpp

