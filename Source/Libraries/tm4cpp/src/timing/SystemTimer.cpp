/*
 * SystemTimer.cpp
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

#include "mcu.h"
#include "../../includes/timing/SystemTimer.h"

namespace tm4cpp
{
  volatile uint32_t SystemTimer::__counter;

  void SystemTimer::initialize()
  {
    uint32_t clockRate;

    __counter = 0;

#ifdef CLASS_IS_TM4C123
    clockRate = MAP_SysCtlClockGet();
#else
    clockRate = MAP_SysCtlClockFreqGet();
#endif

    if (clockRate == 66666666 && MCU_CLOCK == CLK80) { // some boards reports the clock frequency incorrectly
      clockRate = 80000000;
    }

    MAP_SysTickPeriodSet(clockRate / 1000);

    MAP_SysTickIntEnable();
    MAP_SysTickEnable();
  }

} /* namespace tm4cpp */

extern "C" void SysTickHandler(void)
{
  tm4cpp::SystemTimer::__counter++;
}
