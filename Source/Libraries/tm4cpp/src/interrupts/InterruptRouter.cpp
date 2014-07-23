/*
 * InterruptRouter.cpp
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#include "mcu.h"
#include "../../includes/interrupts/InterruptRouter.h"

namespace tm4cpp
{
  _internal::InterruptTarget *InterruptRouter::__interruptTargets[kInterruptVectorSize];

  void InterruptRouter::initialize()
  {
    for (uint_fast8_t i = 0; i < kInterruptVectorSize; i++) {
      InterruptRouter::__interruptTargets[i] = 0;
    }
  }

  void InterruptRouter::addHandler(_internal::InterruptTarget *handler, const uint8_t index)
  {
    InterruptRouter::__interruptTargets[index] = handler;
  }

  void InterruptRouter::removeHandler(const uint8_t index)
  {
    InterruptRouter::__interruptTargets[index] = 0;
  }

} /* namespace tm4cpp */


#define GPIO_HANDLER_FN(letter) \
    void Gpio##letter##intHandler() \
    { \
      uint32_t st = MAP_GPIOIntStatus(tm4cpp::gpio::letter::basePort, true); \
      uint8_t index = tm4cpp::gpio::letter::eventIndex; \
      MAP_GPIOIntClear(tm4cpp::gpio::letter::basePort, st); \
      tm4cpp::_internal::InterruptTarget *handler = tm4cpp::InterruptRouter::__interruptTargets[index]; \
      if (handler != 0) handler->_handleGPIOInterrupt(index, st); \
    }

extern "C"
{
  GPIO_HANDLER_FN(A)
  GPIO_HANDLER_FN(B)
  GPIO_HANDLER_FN(C)
  GPIO_HANDLER_FN(D)
  GPIO_HANDLER_FN(E)
  GPIO_HANDLER_FN(F)
  GPIO_HANDLER_FN(G)
  GPIO_HANDLER_FN(H)
  GPIO_HANDLER_FN(J)
  GPIO_HANDLER_FN(K)
  GPIO_HANDLER_FN(L)
  GPIO_HANDLER_FN(M)
  GPIO_HANDLER_FN(N)
//  GPIO_HANDLER_FN(P)
//  GPIO_HANDLER_FN(Q)
//  GPIO_HANDLER_FN(R)
//  GPIO_HANDLER_FN(S)
}
