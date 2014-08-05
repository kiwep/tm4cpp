/*
 * Button.h
 *
 *  Created on: 2014.07.27.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "Runnable.h"
#include "Port.h"
#include "mcu.h"

namespace tm4cpp
{
  typedef enum ButtonEventType
  {
    ButtonEventTypePress,
    ButtonEventTypeRelease
  } ButtonEventType;

  enum ButtonFlag
  {
    ButtonFlagNone = 0,
    ButtonFlagRepeated = 1,
    ButtonFlagLongPress = 2
  };

  enum _ButtonState
  {
    _ButtonStateNone = 0,
    _ButtonStateDown,
    _ButtonStatePressed,
    _ButtonStateRepeating
  };

#define kEventQueueSize 16
#define kDefaultDebounceDelay 10

  class Button: public Runnable
  {
    public:
      Button();
      ~Button();

      void setup(_GpioPort *port, const uint8_t &pins, const PinType &type = PinTypeWeakPullup);

      uint32_t getDebounceDelay() const;
      void setDebounceDelay(const uint32_t &delay);

      uint32_t getRepeatDelay() const;
      void setRepeatDelay(const uint32_t &delay);

      uint32_t getRepeatInterval() const;
      void setRepeatInterval(const uint32_t &interval);

      void setRepeatParameters(const uint32_t &delay, const uint32_t &interval);

      uint32_t getLongPressTimeout();
      void setLongPressTimeout(const uint32_t &timeout);

      template<typename O, typename T> void setEventHandler(O *obj, void (T::*func)(uint8_t, uint8_t), const ButtonEventType &type);

      void handleInterrupt(uint8_t pins);
      void runLoop();

    private:
      _GpioPort *gpioPort = NULL;
      uint8_t enabledPins = 0;
      PinType pinType = PinTypeStd;
      uint8_t knownButtonMask = 0;
      volatile uint8_t eventButtonStateQueue[kEventQueueSize];
      volatile uint32_t eventTimeStampQueue[kEventQueueSize];
      volatile uint8_t eventQueueInsertIndex = 0;
      uint8_t eventQueueProcessedIndex = 0;
      uint32_t buttonDownTimeStamps[8];
      uint8_t buttonStates[8];
      uint32_t debounceDelay = kDefaultDebounceDelay;
      uint32_t repeatDelay = 0;
      uint32_t repeatInterval = 0;
      uint32_t longPressTimeout = 0;
      uint8_t activeButtonCount = 0;

      fastdelegate::FastDelegate2<uint8_t, uint8_t> onPressDelegate;
      fastdelegate::FastDelegate2<uint8_t, uint8_t> onReleaseDelegate;
  };

  inline Button::Button()
  {
    for (uint8_t i = 0; i < kEventQueueSize; i++) {
      eventButtonStateQueue[i] = 0;
      eventTimeStampQueue[i] = 0;
    }

    for (uint8_t i = 0; i < 8; i++) {
      buttonDownTimeStamps[i] = 0;
      buttonStates[i] = _ButtonStateNone;
    }

    Runtime::addToRunLoop(this);
  }

  inline Button::~Button()
  {
    Runtime::removeFromRunLoop(this);
  }

  inline void Button::setup(_GpioPort *port, const uint8_t &pins, const PinType &type)
  {
    gpioPort = port;
    enabledPins = pins;
    pinType = type;

    gpioPort->setup(enabledPins, PinDirectionInput, PinStrength2ma, type);
    if (!gpioPort->isInterruptHandlerSet()) {
      gpioPort->setInterruptHandler(this, &Button::handleInterrupt);
    }
    gpioPort->enableInterrupts(enabledPins, PinInterruptBothEdges);
  }

  inline void Button::handleInterrupt(uint8_t changedPins)
  {
    // get the pins value
    uint8_t portValue = gpioPort->get(enabledPins);

    // we get low for the pressed down state if the pin was pulled up so invert the result
    if (pinType == PinTypeWeakPullup) {
      portValue = portValue ^ enabledPins;
    }

    // add the event to the queue
    eventButtonStateQueue[eventQueueInsertIndex] = portValue;
    eventTimeStampQueue[eventQueueInsertIndex] = SystemTimer::milliseconds();

    // increment the insert index
    if (eventQueueInsertIndex < kEventQueueSize - 1) {
      eventQueueInsertIndex++;
    }
    else {
      eventQueueInsertIndex = 0;
    }
  }

  inline void Button::runLoop()
  {
    // process events in the queue
    while (eventQueueProcessedIndex != eventQueueInsertIndex) {

      // get the new button states and check for changes
      uint8_t newButtonMask = eventButtonStateQueue[eventQueueProcessedIndex];
      uint8_t buttonStateChanges = newButtonMask ^ knownButtonMask;
      if (buttonStateChanges > 0) {

        // cycle trough the changed pins
        for (uint8_t pinNumber = 0; pinNumber < 8; pinNumber++) {

          // skip the pins that doesn't changed
          uint8_t bitMask = 1 << pinNumber;
          if ((buttonStateChanges & bitMask) == 0) {
            continue;
          }

          // check the button state
          uint32_t eventTimeStamp = eventTimeStampQueue[eventQueueProcessedIndex];
          if ((newButtonMask & bitMask) > 0) {

            // PRESS detected

            // store the event
            buttonDownTimeStamps[pinNumber] = eventTimeStamp;
            buttonStates[pinNumber] = _ButtonStateDown;
            activeButtonCount++;

          }
          else {

            // RELEASE detected

            // check if it was a bounce or a legit release
            if (SystemTimer::distance(buttonDownTimeStamps[pinNumber], eventTimeStamp) > debounceDelay) {

              // seems a valid button press-release
              uint8_t buttonState = buttonStates[pinNumber];

              // report the down and press events is not reported already
              if (buttonState < _ButtonStatePressed) {
                if (onPressDelegate) {
                  onPressDelegate(1 << pinNumber, ButtonFlagNone);
                }
              }

              // report the release event
              if (onReleaseDelegate) {
                onReleaseDelegate(1 << pinNumber, ButtonFlagNone);
              }

            }

            // clean up
            buttonDownTimeStamps[pinNumber] = 0;
            buttonStates[pinNumber] = _ButtonStateNone;
            activeButtonCount--;
          }

        }

        // store the processed new buttons mask
        knownButtonMask = newButtonMask;
      }

      // increment the processed index
      if (eventQueueProcessedIndex < kEventQueueSize - 1) {
        eventQueueProcessedIndex++;
      }
      else {
        eventQueueProcessedIndex = 0;
      }

    } // end while

    // events processed, we have a clear state here

    // check if we have at least one active button, return otherwise
    if (activeButtonCount == 0) {
      return;
    }

    // cycle trough the active pins
    for (uint8_t pinNumber = 0; pinNumber < 8; pinNumber++) {

      uint8_t buttonState = buttonStates[pinNumber];

      if (buttonState == _ButtonStateNone) {
        continue;
      }

      uint32_t downTimeStamp = buttonDownTimeStamps[pinNumber];

      // check if it's a newly pressed button whose debounce delay just passed
      if (buttonState == _ButtonStateDown && SystemTimer::since(downTimeStamp) > debounceDelay) {
        buttonStates[pinNumber] = _ButtonStatePressed;
        if (onPressDelegate) {
          onPressDelegate(1 << pinNumber, ButtonFlagNone);
        }
        continue;
      }

      // check repeat status
      if ((repeatDelay > 0 && buttonState == _ButtonStatePressed && SystemTimer::since(downTimeStamp) > repeatDelay) ||
          (repeatInterval > 0 && buttonState == _ButtonStateRepeating && SystemTimer::since(downTimeStamp) > repeatInterval)) {
        buttonStates[pinNumber] = _ButtonStateRepeating;
        buttonDownTimeStamps[pinNumber] = SystemTimer::milliseconds();
        if (onPressDelegate) {
          onPressDelegate(1 << pinNumber, ButtonFlagRepeated);
        }
        continue;
      }

      if (longPressTimeout > 0 && buttonState == _ButtonStatePressed && SystemTimer::since(downTimeStamp) > longPressTimeout) {
        buttonStates[pinNumber] = _ButtonStateRepeating;
        if (onPressDelegate) {
          onPressDelegate(1 << pinNumber, ButtonFlagLongPress);
        }
      }

    }

  }

  // getters and setters

  inline uint32_t Button::getDebounceDelay() const
  {
    return debounceDelay;
  }

  inline void Button::setDebounceDelay(const uint32_t &delay)
  {
    debounceDelay = delay;
  }

  inline uint32_t Button::getRepeatDelay() const
  {
    return repeatDelay;
  }

  inline void Button::setRepeatDelay(const uint32_t &delay)
  {
    repeatDelay = delay;
    longPressTimeout = 0;
  }

  inline uint32_t Button::getRepeatInterval() const
  {
    return repeatInterval;
  }

  inline void Button::setRepeatInterval(const uint32_t &interval)
  {
    repeatInterval = interval;
    longPressTimeout = 0;
  }

  inline void Button::setRepeatParameters(const uint32_t &delay, const uint32_t &interval)
  {
    repeatDelay = delay;
    repeatInterval = interval;
    longPressTimeout = 0;
  }

  inline uint32_t Button::getLongPressTimeout()
  {
    return longPressTimeout;
  }

  inline void Button::setLongPressTimeout(const uint32_t &timeout)
  {
    longPressTimeout = timeout;
    repeatDelay = repeatInterval = 0;
  }

  template<typename O, typename T>
  inline void Button::setEventHandler(O *obj, void (T::*func)(uint8_t, uint8_t), const ButtonEventType &type)
  {
    if (type == ButtonEventTypePress) {
      onPressDelegate = fastdelegate::MakeDelegate(obj, func);
    }
    else if (type == ButtonEventTypeRelease) {
      onReleaseDelegate = fastdelegate::MakeDelegate(obj, func);
    }
  }


}  // namespace tm4cpp

