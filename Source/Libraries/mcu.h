/*
 * mcu.h
 *
 *  Created on: 2014.07.20.
 *      Author: kiwep
 */

#pragma once

// Generic CPU clock seeds
#define CLK80 1   // 80MHz
#define CLK66 2   // 66Mhz
#define CLK50 3   // 50Mhz
#define CLK33 4   // 33Mhz
#define CLK16 5   // 16Mhz

// MCU settings
#define TARGET_IS_TM4C123_RB1
#define PART_TM4C123GH6PM
#define MCU_CLOCK CLK80

// Library includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

// TivaWare header includes
#include "tivaware/inc/hw_sysctl.h"
#include "tivaware/inc/hw_memmap.h"
#include "tivaware/inc/hw_types.h"
#include "tivaware/inc/hw_gpio.h"
#include "tivaware/inc/hw_uart.h"
#include "tivaware/inc/hw_ints.h"

// TivaWare Driverlib includes
#include "tivaware/driverlib/rom.h"
#include "tivaware/driverlib/rom_map.h"
#include "tivaware/driverlib/gpio.h"
#include "tivaware/driverlib/sysctl.h"
#include "tivaware/driverlib/interrupt.h"

// Debug logger
#ifdef DEBUG
#define _log(x...) printf(x)
#else
#define _log(x...) {}
#endif

// TM4C++ header files
#include "tm4cpp/framework.h"
#include "tm4cpp/gpio.h"
#include "tm4cpp/timing.h"
