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
    Button lrButton;

  public:
    Main()
    {
      // Output: LEDs
      ioPortF.setup(gpio::Pin1 | gpio::Pin3);
      ioPortF(gpio::Pin1, true);

      // Button setup
      lrButton.setup(&ioPortF, gpio::Pin0 | gpio::Pin4);
      lrButton.setEventHandler(this, &Main::onButtonPress, button::Press);
    }

    void onButtonPress(uint8_t pins)
    {
    }

    void runLoop()
    {
    }

};


/*

//      lrButton.setEventHandler(this, &Main::onButtonDown, button::Down);
//      lrButton.setEventHandler(this, &Main::onButtonUp, button::Up);
//      addToRunLoop(&lrButton);

      // Input: Buttons
//      pF.setupPins(gpio::Pin0 | gpio::Pin4, gpio::Input, gpio::Strength2ma, gpio::TypeWeakPullup);
//      pF.registerInterruptDelegate(this, &Main::mainPortInterruptReceived);
//      pF.enablePinInterrupts(gpio::Pin0 | gpio::Pin4, gpio::BothEdges);

    void onButtonDown(uint8_t pins)
    {
    }

    void onButtonUp(uint8_t pins)
    {
    }

//    void mainPortInterruptReceived(uint8_t pins)
//    {
//      newBtnMap = pF.readPins() ^ 0xff;
//    }

//      if (newBtnMap != olbBtnMap) {
//        pF.setPin(gpio::Pin1, (newBtnMap & gpio::Pin0));
//        pF.setPin(gpio::Pin3, (newBtnMap & gpio::Pin4));
//        olbBtnMap = newBtnMap;
//      }

*/
