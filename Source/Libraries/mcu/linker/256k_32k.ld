/*
 * 256k_32k.ld
 *
 * Linker script for the TM4C12x MCUs
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
ENTRY(ResetISR)

/* Specify the memory areas */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
  RAM  (xrw) : ORIGIN = 0x20000000, LENGTH = 32K
}

_heap_size = 8K;
