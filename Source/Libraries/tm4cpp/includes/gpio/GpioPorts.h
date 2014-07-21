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

#define GEN_PORT_S(name, sys, base) struct name { \
    static const uint32_t peripheral = sys; \
    static const uint32_t basePort = base; \
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

    enum direction
    {
      Input = GPIO_DIR_MODE_IN,
      Output = GPIO_DIR_MODE_OUT,
      Hardware = GPIO_DIR_MODE_HW
    };

    GEN_PORT_S(A, SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE);
    GEN_PORT_S(B, SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE);
    GEN_PORT_S(C, SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE);
    GEN_PORT_S(D, SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE);
    GEN_PORT_S(E, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE);
    GEN_PORT_S(F, SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE);
    GEN_PORT_S(G, SYSCTL_PERIPH_GPIOG, GPIO_PORTG_BASE);
    GEN_PORT_S(H, SYSCTL_PERIPH_GPIOH, GPIO_PORTH_BASE);
    GEN_PORT_S(J, SYSCTL_PERIPH_GPIOJ, GPIO_PORTJ_BASE);
    GEN_PORT_S(K, SYSCTL_PERIPH_GPIOK, GPIO_PORTK_BASE);
    GEN_PORT_S(L, SYSCTL_PERIPH_GPIOL, GPIO_PORTL_BASE);
    GEN_PORT_S(M, SYSCTL_PERIPH_GPIOM, GPIO_PORTM_BASE);
    GEN_PORT_S(N, SYSCTL_PERIPH_GPION, GPIO_PORTN_BASE);
    GEN_PORT_S(P, SYSCTL_PERIPH_GPIOP, GPIO_PORTP_BASE);
    GEN_PORT_S(Q, SYSCTL_PERIPH_GPIOQ, GPIO_PORTQ_BASE);
    GEN_PORT_S(R, SYSCTL_PERIPH_GPIOR, GPIO_PORTR_BASE);
    GEN_PORT_S(S, SYSCTL_PERIPH_GPIOS, GPIO_PORTS_BASE);
    GEN_PORT_S(T, SYSCTL_PERIPH_GPIOT, GPIO_PORTT_BASE);

  }
}

