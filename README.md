# TM4C++
### A C++ Embedded Development Framework for Texas Instruments Tiva TM4C12x Family Boards

**This repository contains a work in progress codebase, plase don't use it yet!**

I started this project because I think a basic Hello ~~World~~ Blink should look like at least this elegant:

```cpp
#include "mcu.h"

using namespace tm4cpp;

class Main: Application
{
    DigitalPeripheral<gpio::F> gpioF;

  public:
    Main()
    {
      gpioF.setPinDirection(gpio::Pin1 | gpio::Pin3, gpio::Output);
      gpioF.setPinHigh(gpio::Pin3);
    }

    void runLoop()
    {
      gpioF.togglePin(gpio::Pin1 | gpio::Pin3);
      SystemTimer::delay(100);
    }
};

```

Currently the code above compiles to a 1016 byte binary with my custom Clang/ARM-GCC toolchain.
