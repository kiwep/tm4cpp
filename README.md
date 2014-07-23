# TM4C++
### A C++ Embedded Development Framework for Texas Instruments Tiva TM4C12x Family Boards

**This repository contains a work in progress codebase, plase don't use it yet!**

I started this project because I think a basic Hello ~~World~~ Blink should look like at least this elegant:

```cpp
#include "mcu.h"

using namespace tm4cpp;

class Main
{
    Port<gpio::F, Main> pF;

  public:
    Main()
    {
      // Output: LEDs
      pF.setupPins(gpio::Pin1 | gpio::Pin3);
      pF.setPinHigh(gpio::Pin3);
    }

    void runLoop()
    {
      // This will alternate the two LEDs every 100ms
      pF.togglePin(gpio::Pin1 | gpio::Pin3);
      SystemTimer::delay(100);
    }
};
```

This one is a slightly more advanced example, interrupt handling for two buttons:

```cpp
#include "mcu.h"

using namespace tm4cpp;

class Main
{
    Port<gpio::F, Main> pF;
    volatile uint8_t olbBtnMap, newBtnMap;

  public:
    Main() :
        olbBtnMap(0), newBtnMap(0)
    {
      // Output: LEDs
      pF.setupPins(gpio::Pin1 | gpio::Pin3);

      // Input: Buttons
      pF.setupPins(gpio::Pin0 | gpio::Pin4, gpio::Input, gpio::Strength2ma, gpio::TypeWeakPullup);
      pF.setInterruptCallback(this, &Main::mainPortInterruptReceived);
      pF.enablePinInterrupts(gpio::Pin0 | gpio::Pin4, gpio::BothEdges);
    }

    void mainPortInterruptReceived(uint8_t pins)
    {
      newBtnMap = pF.readPins() ^ 0xff;
    }

    void runLoop()
    {
      if (newBtnMap != olbBtnMap) {
        pF.setPin(gpio::Pin1, (newBtnMap & gpio::Pin0));
        pF.setPin(gpio::Pin3, (newBtnMap & gpio::Pin4));
        olbBtnMap = newBtnMap;
      }
    }

};
```

The code above compiles to 2904 bytes with my custom Clang/ARM-GCC toolchain.
