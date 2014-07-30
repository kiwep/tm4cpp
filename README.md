# TM4C++
### A C++ Embedded Development Framework for Texas Instruments Tiva TM4C12x Family Boards

**This repository contains a work in progress codebase, plase don't use it yet!**

I started this project because I think a basic Hello ~~World~~ Blink should look like at least this elegant:

```cpp
#include "mcu.h"

using namespace tm4cpp;

class Main: public Runnable
{
    GpioF ioPortF; // the leds are connected to port F on our test board

  public:

	// this is the startup code
    Main()
    {
      ioPortF.setup(gpio::Pin1 | gpio::Pin3); // setup the two led pins as output
      ioPortF(gpio::Pin3, true); // turn on pin3
    }

	// you can have a run loop if you so desire
    void runLoop()
    {
      ioPortF.toggle(gpio::Pin1 | gpio::Pin3); // inverts the state of the two leds
      SystemTimer::delay(100); // waits 0.1 seconds
    }
};
```

The problem with the blinking led example is that it's too simple. You want to make a lot more complex things then that, don't you? Ok then lets see a little more andvanced stuff, handling two buttons:

```cpp
#include "mcu.h"

using namespace tm4cpp;

class Main: public Runnable
{
    GpioF ioPortF; // the F port is where the leds and buttons connected to
    Button buttons; // button handler
    Pulser pulser; // pulser for turning on the leds for a short amount of time

  public:

	// this is the startup code
    Main()
    {
      // output leds (Pin1=red, Pin3=green)
      pulser.setup(&ioPortF, gpio::Pin1 | gpio::Pin3);

      // Button handler setup (Pin0=right, Pin4=left)
      buttons.setup(&ioPortF, gpio::Pin0 | gpio::Pin4);
      buttons.setRepeatParameters(500, 200);
      buttons.setEventHandler(this, &Main::onButtonPress, button::Press);
    }

	// this gets called when a button is pressed
    void onButtonPress(uint8_t pin, uint8_t flags)
    {
      pulser.emitWhen((pin & gpio::Pin0), gpio::Pin1);
      pulser.emitWhen((pin & gpio::Pin4), gpio::Pin3);
    }
};

```

The code above compiles to ~5.6k with the Clang/GCC toolchain. Doesn't you miss anything? Yep, there is no runloop! It's interrupt based, the Button class handles debouncing and has repeat and longpress event support.
