/*
 * InterruptRouter.h
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "GpioPorts.h"
#include "helpers/FastDelegate.h"

namespace tm4cpp
{
  static const uint8_t kInterruptVectorSize = 13;
  typedef fastdelegate::FastDelegate1<uint8_t> InterruptDelegate;
  static InterruptDelegate __interruptDelegateTable[kInterruptVectorSize];

  class InterruptRouter
  {
    public:

      static void initialize();
      static void addDelegate(const uint8_t index, const InterruptDelegate *delegate);
      static void removeDelegate(const uint8_t index);
      static bool isDelegateRegistered(const uint8_t index);

  };

} /* namespace tm4cpp */
