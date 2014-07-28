/*
 * Runtime.cpp
 *
 *  Created on: 2014.07.28.
 *      Author: Peter Kovacs ~ k'wep
 */

#include "Runtime.h"

namespace tm4cpp {

  RunnablePointerArray Runtime::__handlers;

  void Runtime::addToRunLoop(Runnable *runnable)
  {
    __handlers.add(runnable);
  }

  void Runtime::removeFromRunLoop(Runnable *runnable)
  {
    __handlers.remove(runnable);
  }

  void Runtime::runLoop()
  {
    uint8_t s = __handlers.size();
    for (uint8_t i = 0; i < s; i++) {
      __handlers[i]->runLoop();
    }
  }

}  // namespace tm4cpp


