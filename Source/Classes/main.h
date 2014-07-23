/*
 * main.h
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

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
