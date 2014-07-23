/*
 * GpioPorts.h
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "tivaware/inc/hw_memmap.h"
#include "tivaware/driverlib/sysctl.h"
#include "tivaware/driverlib/gpio.h"
#include "../interrupts/InterruptNumbers.h"

#define GEN_PORT_S(name, idx, sys, base, intp) \
  struct name { \
    static const uint8_t  eventIndex = idx; \
    static const uint32_t peripheral = sys; \
    static const uint32_t basePort = base; \
    static const uint32_t intPort = intp; \
  }

namespace tm4cpp
{

  namespace gpio
  {

    enum Pin
    {
      Pin0 = GPIO_PIN_0,
      Pin1 = GPIO_PIN_1,
      Pin2 = GPIO_PIN_2,
      Pin3 = GPIO_PIN_3,
      Pin4 = GPIO_PIN_4,
      Pin5 = GPIO_PIN_5,
      Pin6 = GPIO_PIN_6
    };

    enum Direction
    {
      Input = GPIO_DIR_MODE_IN,
      Output = GPIO_DIR_MODE_OUT,
      Hardware = GPIO_DIR_MODE_HW
    };

    enum Strength
    {
      Strength2ma = GPIO_STRENGTH_2MA,       // 2mA drive strength
      Strength4ma = GPIO_STRENGTH_4MA,       // 4mA drive strength
      Strength6ma = GPIO_STRENGTH_6MA,       // 6mA drive strength
      Strength8ma = GPIO_STRENGTH_8MA,       // 8mA drive strength
      Strength8maSc = GPIO_STRENGTH_8MA_SC,  // 8mA drive with slew rate control
      Strength10ma = GPIO_STRENGTH_10MA,     // 10mA drive strength
      Strength12ma = GPIO_STRENGTH_12MA      // 12mA drive strength
    };

    enum Type
    {
      TypeStd = GPIO_PIN_TYPE_STD,                // Push-pull
      TypeWeakPullup = GPIO_PIN_TYPE_STD_WPU,     // Push-pull with weak pull-up
      TypeWeakPulldown = GPIO_PIN_TYPE_STD_WPD,   // Push-pull with weak pull-down
      TypeOpenDrain = GPIO_PIN_TYPE_OD,           // Open-drain
      TypeAnalog = GPIO_PIN_TYPE_ANALOG,          // Analog comparator
      TypeWakeupHigh = GPIO_PIN_TYPE_WAKE_HIGH,   // Hibernate wake, high
      TypeWakeupLow = GPIO_PIN_TYPE_WAKE_LOW      // Hibernate wake, low

    };

    enum IntEvent
    {
      FallingEdge = GPIO_FALLING_EDGE,        // Interrupt on falling edge
      RisingEdge = GPIO_RISING_EDGE,          // Interrupt on rising edge
      BothEdges = GPIO_BOTH_EDGES,            // Interrupt on both edges
      LowLevel = GPIO_LOW_LEVEL,              // Interrupt on low level
      HighLevel = GPIO_HIGH_LEVEL,            // Interrupt on high level
      DiscreteInterrupt = GPIO_DISCRETE_INT,  // Interrupt for individual pins
    };

    GEN_PORT_S(A, intr::gpioA, SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE, INT_GPIOA);
    GEN_PORT_S(B, intr::gpioB, SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE, INT_GPIOB);
    GEN_PORT_S(C, intr::gpioC, SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE, INT_GPIOC);
    GEN_PORT_S(D, intr::gpioD, SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, INT_GPIOD);
    GEN_PORT_S(E, intr::gpioE, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, INT_GPIOE);
    GEN_PORT_S(F, intr::gpioF, SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE, INT_GPIOF);
    GEN_PORT_S(G, intr::gpioG, SYSCTL_PERIPH_GPIOG, GPIO_PORTG_BASE, INT_GPIOG);
    GEN_PORT_S(H, intr::gpioH, SYSCTL_PERIPH_GPIOH, GPIO_PORTH_BASE, INT_GPIOH);
    GEN_PORT_S(J, intr::gpioJ, SYSCTL_PERIPH_GPIOJ, GPIO_PORTJ_BASE, INT_GPIOJ);
    GEN_PORT_S(K, intr::gpioK, SYSCTL_PERIPH_GPIOK, GPIO_PORTK_BASE, INT_GPIOK);
    GEN_PORT_S(L, intr::gpioL, SYSCTL_PERIPH_GPIOL, GPIO_PORTL_BASE, INT_GPIOL);
    GEN_PORT_S(M, intr::gpioM, SYSCTL_PERIPH_GPIOM, GPIO_PORTM_BASE, INT_GPIOM);
    GEN_PORT_S(N, intr::gpioN, SYSCTL_PERIPH_GPION, GPIO_PORTN_BASE, INT_GPION);

    // TODO: handle board differences etc
//    GEN_PORT_S(P, intr::gpioP, SYSCTL_PERIPH_GPIOP, GPIO_PORTP_BASE, INT_GPIOP);
//    GEN_PORT_S(Q, intr::gpioQ, SYSCTL_PERIPH_GPIOQ, GPIO_PORTQ_BASE, INT_GPIOQ);
//    GEN_PORT_S(R, intr::gpioR, SYSCTL_PERIPH_GPIOR, GPIO_PORTR_BASE, INT_GPIOR);
//    GEN_PORT_S(S, intr::gpioS, SYSCTL_PERIPH_GPIOS, GPIO_PORTS_BASE, INT_GPIOS);

  }
}

