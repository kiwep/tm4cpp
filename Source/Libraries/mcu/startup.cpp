/*
 * startup.c
 *
 *  Created on: 2014.07.20.
 *      Author: kiwep
 */

#include "mcu.h"
#include "main.h"

using namespace tm4cpp;

// Application entry point
//extern int main(void);

// Addresses pulled in from the linker script
extern uint32_t _text_end;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;

/*
 * Reset Interrupt handler
 * This methods gets called by the MCU on soft and hard reset (eg. startup)
 */
extern "C" void ResetISR(void)
{
  uint8_t *src, *dst, *end;

  // Copy the data segment initializers from flash to SRAM
  src = (uint8_t *) &_text_end;
  dst = (uint8_t *) &_data_start;
  end = (uint8_t *) &_data_end;
  while (dst < end) {
    *dst++ = *src++;
  }

  // Zero fill the bss segment
  __asm("  ldr     r0, =_bss_start\n"
      "    ldr     r1, =_bss_end\n"
      "    mov     r2, #0\n"
      "    .thumb_func\n"
      "zero_loop:\n"
      "    cmp     r0, r1\n"
      "    it      lt\n"
      "    strlt   r2, [r0], #4\n"
      "    blt     zero_loop");

#ifndef TARGET_QEMU

  // Enable FPU
  MAP_FPUEnable();
  MAP_FPULazyStackingEnable();

  // Setup clock
#if MCU_CLOCK == CLK80
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
#elif MCU_CLOCK == CLK66
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_3 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
#elif MCU_CLOCK == CLK50
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
#else
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
#endif

  // Initialize the system tick timer
  SystemTimer::initialize();

#endif

  // Initialize the interrupt router helper
  InterruptRouter::initialize();

  // Start application
  Main mainApp;
  while(1) {
    Runtime::runLoop();
    mainApp.runLoop();
  }

}

/**
 * TivaWare Driverlib assertion stub
 */
#ifdef DEBUG
extern "C" void __error__(char *pcFilename, uint32_t ui32Line) {}
#endif
