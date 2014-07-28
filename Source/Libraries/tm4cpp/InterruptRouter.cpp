/*
 * InterruptRouter.cpp
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#include "mcu.h"
#include "InterruptRouter.h"

namespace tm4cpp
{
  void InterruptRouter::initialize()
  {
    for (uint_fast8_t i = 0; i < kInterruptVectorSize; i++) {
      __interruptDelegateTable[i] = 0;
    }
  }

  void InterruptRouter::addDelegate(const uint8_t index, const InterruptDelegate *delegate)
  {
    __interruptDelegateTable[index] = *delegate;
  }

  void InterruptRouter::removeDelegate(const uint8_t index)
  {
    __interruptDelegateTable[index].clear();
  }

  bool InterruptRouter::isDelegateRegistered(const uint8_t index)
  {
    return !!__interruptDelegateTable[index];
  }

} /* namespace tm4cpp */


#define GPIO_HANDLER_FN(letter) \
    void Gpio##letter##intHandler() \
    { \
      uint32_t st = MAP_GPIOIntStatus(tm4cpp::gpio::letter::basePort, true); \
      uint8_t index = tm4cpp::gpio::letter::eventIndex; \
      MAP_GPIOIntClear(tm4cpp::gpio::letter::basePort, st); \
      tm4cpp::InterruptDelegate delegate = tm4cpp::__interruptDelegateTable[index]; \
      if (delegate) delegate(st); \
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
