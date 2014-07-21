# TM4C++
### A C++ Embedded Development Framework for Texas Instruments Tiva TM4C12x Family Boards

**This repository contains a work in progress codebase, plase don't use it yet!**

```cpp
#include "mcu.h"

using namespace tm4cpp;

class Main: Application
{
    DigitalPeripheral<gpio::F> gpioF;

  public:
    Main()
    {
      gpioF.setPinDirections(gpio::Output, gpio::Pin1 | gpio::Pin3);
      gpioF.setPin(gpio::Pin3, true);
    }

    void runLoop()
    {
      gpioF.togglePin(gpio::Pin1 | gpio::Pin3);
      SystemTimer::delay(100);
    }

};

```