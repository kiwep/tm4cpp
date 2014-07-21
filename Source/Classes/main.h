/*
 * main.h
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

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
