/*
 * main.h
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "mcu.h"

using namespace tm4cpp;

class Main: public Runnable
{
    GpioF ioPortF;
    Button buttons;
    Pulser pulser;

  public:

    Main()
    {
      // Output: LEDs (Pin1=red, Pin3=green)
      pulser.setup(&ioPortF, gpio::Pin1 | gpio::Pin3);

      // Button handler setup (Pin0=right, Pin4=left)
      buttons.setup(&ioPortF, gpio::Pin0 | gpio::Pin4);
      buttons.setRepeatParameters(500, 200);
      buttons.setEventHandler(this, &Main::onButtonPress, button::Press);
    }

    void onButtonPress(uint8_t pin, uint8_t flags)
    {
      pulser.emitWhen((pin & gpio::Pin0), gpio::Pin1);
      pulser.emitWhen((pin & gpio::Pin4), gpio::Pin3);
    }

};
