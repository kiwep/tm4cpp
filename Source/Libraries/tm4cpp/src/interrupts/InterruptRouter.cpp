/*
 * InterruptRouter.cpp
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#include "mcu.h"
#include "../../includes/interrupts/InterruptRouter.h"
#include "../../includes/gpio/GpioPorts.h"

namespace tm4cpp
{
  EventHandlerPointerArray InterruptRouter::__targets[kEventVectorSize];

//  void InterruptRouter::initialize()
//  {
//  }

  void InterruptRouter::handleGPIOInterrupt(const uint8_t index, const uint32_t flags)
  {
    EventHandlerPointerArray handlerArray = InterruptRouter::__targets[index];
    uint8_t s = handlerArray.size();
    if (s > 0) {
      uint8_t i;
      for (i = 0; i < s; i++) {
        handlerArray[i]->_handleGPIOInterrupt(index, flags);
      }
    }
  }

} /* namespace tm4cpp */



#define GPIO_HANDLER_FN(letter) \
    void Gpio##letter##intHandler() \
    { \
      uint32_t st = MAP_GPIOIntStatus(tm4cpp::gpio::letter::basePort, true); \
      MAP_GPIOIntClear(tm4cpp::gpio::letter::basePort, st); \
      tm4cpp::InterruptRouter::handleGPIOInterrupt(tm4cpp::gpio::letter::eventIndex, st); \
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
  GPIO_HANDLER_FN(P)
  GPIO_HANDLER_FN(Q)
  GPIO_HANDLER_FN(R)
  GPIO_HANDLER_FN(S)
}
