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
    GpioB ioPortB;
    RotaryEncoder encoder;
//    Button button;
    Pulser pulser;

  public:

    Main()
    {
      ioPortF.setup(Pin3, PinDirectionOutput);

      // Pulser handled LEDs (Pin1=red, Pin2=blue, Pin3=green)
      pulser.setup(&ioPortF, Pin1 | Pin2);

      // Rotary Encoder setup
      encoder.setup(&ioPortB, Pin0, Pin1, RotaryEncoderTypeDetent);
      encoder.setEventHandler(this, &Main::onEncoderStepChange);

      // Button on the encoder
//      button.setup(&ioPortB, Pin4);
//      button.setEventHandler(this, &Main::onButtonPress, ButtonEventTypePress);
//      button.setEventHandler(this, &Main::onButtonRelease, ButtonEventTypeRelease);
    }

    void onEncoderStepChange(long newValue, long oldValue)
    {
      pulser((newValue > oldValue) ? Pin1 : Pin2);
    }

//    void onButtonPress(uint8_t pin, uint8_t flags)
//    {
//      ioPortF(Pin3, true);
//    }
//
//    void onButtonRelease(uint8_t pin, uint8_t flags)
//    {
//      ioPortF(Pin3, false);
//    }

};
