/*
 * tm4c12x.ld
 *
 * Linker script for the TM4C12x MCUs
 *
 *  Created on: 2014.07.20.
 *      Author: Peter Kovacs ~ k'wep
 */

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _text_start = .;
        KEEP(*(.isr_vector))
        *(.text .text* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(SORT(.preinit_array*)))
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        . = ALIGN(4);
        _text_end = .;
    } > FLASH

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        . = ALIGN(4);
        _data_start = .;
        *(vtable)
        *(.data .data* .gnu.linkonce.d.*)
        _data_end = .;
    } > RAM

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss .bss*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
        . = ALIGN(8);
    } > RAM

    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > RAM
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);
    .heap :
    {
      __end__ = .;
      PROVIDE(end = .);
      PROVIDE(_heap_start = end );
      . = . + _heap_size;
      PROVIDE(_heap_end = .);
    } > RAM

    .ARM.attributes 0 : { *(.ARM.attributes) }

    .stack_dummy (COPY):
    {
      _stack_end = .;
      *(.stack*)
    } > RAM

    _stack_start = ORIGIN(RAM) + LENGTH(RAM);
    _stack_size = _stack_start - SIZEOF(.stack_dummy);
    PROVIDE(__stack = _start_stack);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(_stack_size >= _heap_end, "region RAM overflowed with stack")

}

_end = . ;
