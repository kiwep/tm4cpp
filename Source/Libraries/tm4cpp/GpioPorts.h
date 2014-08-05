/*
 * GpioPorts.h
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include "tivaware/inc/hw_memmap.h"
#include "tivaware/inc/hw_ints.h"
#include "tivaware/driverlib/sysctl.h"
#include "tivaware/driverlib/gpio.h"
#include "InterruptNumbers.h"

#define GEN_PORT_S(name, idx, sys, base, intp) \
  struct name { \
    static const uint8_t  eventIndex = idx; \
    static const uint32_t peripheral = sys; \
    static const uint32_t basePort = base; \
    static const uint32_t intPort = intp; \
  }

namespace tm4cpp
{
  typedef enum Pin
  {
    Pin0 = GPIO_PIN_0,
    Pin1 = GPIO_PIN_1,
    Pin2 = GPIO_PIN_2,
    Pin3 = GPIO_PIN_3,
    Pin4 = GPIO_PIN_4,
    Pin5 = GPIO_PIN_5,
    Pin6 = GPIO_PIN_6
  } Pin;

  typedef enum PinDirection
  {
    PinDirectionInput = GPIO_DIR_MODE_IN,
    PinDirectionOutput = GPIO_DIR_MODE_OUT,
    PinDirectionHardware = GPIO_DIR_MODE_HW
  } PinDirection;

  typedef enum PinStrength
  {
    PinStrength2ma = GPIO_STRENGTH_2MA,       // 2mA drive strength
    PinStrength4ma = GPIO_STRENGTH_4MA,       // 4mA drive strength
    PinStrength6ma = GPIO_STRENGTH_6MA,       // 6mA drive strength
    PinStrength8ma = GPIO_STRENGTH_8MA,       // 8mA drive strength
    PinStrength8maSc = GPIO_STRENGTH_8MA_SC,  // 8mA drive with slew rate control
    PinStrength10ma = GPIO_STRENGTH_10MA,     // 10mA drive strength
    PinStrength12ma = GPIO_STRENGTH_12MA      // 12mA drive strength
  } PinStrength;

  typedef enum PinType
  {
    PinTypeStd = GPIO_PIN_TYPE_STD,                // Push-pull
    PinTypeWeakPullup = GPIO_PIN_TYPE_STD_WPU,     // Push-pull with weak pull-up
    PinTypeWeakPulldown = GPIO_PIN_TYPE_STD_WPD,   // Push-pull with weak pull-down
    PinTypeOpenDrain = GPIO_PIN_TYPE_OD,           // Open-drain
    PinTypeAnalog = GPIO_PIN_TYPE_ANALOG,          // Analog comparator
    PinTypeWakeupHigh = GPIO_PIN_TYPE_WAKE_HIGH,   // Hibernate wake, high
    PinTypeWakeupLow = GPIO_PIN_TYPE_WAKE_LOW      // Hibernate wake, low
  } PinType;

  typedef enum PinInterrupt
  {
    PinInterruptFallingEdge = GPIO_FALLING_EDGE,   // Interrupt on falling edge
    PinInterruptRisingEdge = GPIO_RISING_EDGE,     // Interrupt on rising edge
    PinInterruptBothEdges = GPIO_BOTH_EDGES,       // Interrupt on both edges
    PinInterruptLowLevel = GPIO_LOW_LEVEL,         // Interrupt on low level
    PinInterruptHighLevel = GPIO_HIGH_LEVEL,       // Interrupt on high level
    PinInterruptDiscrete = GPIO_DISCRETE_INT,      // Interrupt for individual pins
  } PinInterrupt;

  namespace gpio
  {
    GEN_PORT_S(A, intr::gpio::A, SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE, INT_GPIOA);
    GEN_PORT_S(B, intr::gpio::B, SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE, INT_GPIOB);
    GEN_PORT_S(C, intr::gpio::C, SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE, INT_GPIOC);
    GEN_PORT_S(D, intr::gpio::D, SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE, INT_GPIOD);
    GEN_PORT_S(E, intr::gpio::E, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, INT_GPIOE);
    GEN_PORT_S(F, intr::gpio::F, SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE, INT_GPIOF);
    GEN_PORT_S(G, intr::gpio::G, SYSCTL_PERIPH_GPIOG, GPIO_PORTG_BASE, INT_GPIOG);
    GEN_PORT_S(H, intr::gpio::H, SYSCTL_PERIPH_GPIOH, GPIO_PORTH_BASE, INT_GPIOH);
    GEN_PORT_S(J, intr::gpio::J, SYSCTL_PERIPH_GPIOJ, GPIO_PORTJ_BASE, INT_GPIOJ);
    GEN_PORT_S(K, intr::gpio::K, SYSCTL_PERIPH_GPIOK, GPIO_PORTK_BASE, INT_GPIOK);
    GEN_PORT_S(L, intr::gpio::L, SYSCTL_PERIPH_GPIOL, GPIO_PORTL_BASE, INT_GPIOL);
    GEN_PORT_S(M, intr::gpio::M, SYSCTL_PERIPH_GPIOM, GPIO_PORTM_BASE, INT_GPIOM);
    GEN_PORT_S(N, intr::gpio::N, SYSCTL_PERIPH_GPION, GPIO_PORTN_BASE, INT_GPION);

    // TODO: handle board differences etc
//    GEN_PORT_S(P, intr::gpio::P, SYSCTL_PERIPH_GPIOP, GPIO_PORTP_BASE, INT_GPIOP);
//    GEN_PORT_S(Q, intr::gpio::Q, SYSCTL_PERIPH_GPIOQ, GPIO_PORTQ_BASE, INT_GPIOQ);
//    GEN_PORT_S(R, intr::gpio::R, SYSCTL_PERIPH_GPIOR, GPIO_PORTR_BASE, INT_GPIOR);
//    GEN_PORT_S(S, intr::gpio::S, SYSCTL_PERIPH_GPIOS, GPIO_PORTS_BASE, INT_GPIOS);
  }
}

