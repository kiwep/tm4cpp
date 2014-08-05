#
# TM4C++
# Project Makefile
#

#
# Build parameters
#
BuildConfig   ?= DEBUG
BuildFileName ?= app

cpuType     := cortex-m4
floatAbi    := hard
instrSet    := armv7e-m/fpu
memoryModel := 256k_32k

generateMap := No
printfFloat := No
scanfFloat  := No
warnLevel   := all

#
# Directories and build tools
#
ifeq ($(BuildConfig),RELEASE)
	buildConfigStr := Release
	CFLAGS := -Os
else
	buildConfigStr := Debug
	CFLAGS := -g -O0 -DDEBUG
	ifeq ($(BuildConfig),QEMU)
		buildConfigStr := Qemu
		CFLAGS += -DTARGET_QEMU
	else
	endif
endif

ARMToolRoot   ?= ~/Development/arm
triple        := arm-none-eabi
sourceDir     := Source
buildDir      := .build/$(buildConfigStr)
productTarget := Product/$(buildConfigStr)/$(BuildFileName).bin
linkDefPath   := $(sourceDir)/Libraries/MCU/linker

CFLAGS += -nostdlib
CFLAGS += -ffreestanding
CFLAGS += -fno-exceptions
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -target $(triple)
CFLAGS += -mcpu=$(cpuType)
CFLAGS += -mfloat-abi=$(floatAbi)
CFLAGS += -mthumb
CFLAGS += -MMD -MP
CFLAGS += -W$(warnLevel)
CFLAGS += -I$(ARMToolRoot)/arm-none-eabi/include
CFLAGS += -I$(ARMToolRoot)/arm-none-eabi/include/c++/4.8.4/arm-none-eabi/$(instrSet)
CFLAGS += -I$(ARMToolRoot)/arm-none-eabi/include/c++/4.8.4/
CFLAGS += -Dgcc

LFLAGS := --gc-sections
LFLAGS += -T $(linkDefPath)/$(memoryModel).ld
LFLAGS += -T $(linkDefPath)/tm4c12x.ld
LFLAGS += --library-path $(ARMToolRoot)/arm-none-eabi/lib/$(instrSet)
LFLAGS += --library-path $(ARMToolRoot)/lib/gcc/arm-none-eabi/4.8.4/$(instrSet)
LFLAGS += --library-path $(ARMToolRoot)/lib

LGROUPS := --start-group -lgcc -lc_s -lrdimon_s -lstdc++_s --end-group

ifeq ($(generateMap),Yes)
	LFLAGS += -Map $(buildDir)/linker.map
endif

ifeq ($(printfFloat),Yes)
	CFLAGS += -u _printf_float
	LFLAGS += -u _printf_float
endif

ifeq ($(scanfFloat),Yes)
	CFLAGS += -u _scanf_float
	LFLAGS += -u _scanf_float
endif

CC      := $(ARMToolRoot)/bin/clang
CXX     := $(ARMToolRoot)/bin/clang++
AS      := $(ARMToolRoot)/bin/$(triple)-as
LD      := $(ARMToolRoot)/bin/$(triple)-ld
SIZE    := $(ARMToolRoot)/bin/$(triple)-size
OBJCOPY := $(ARMToolRoot)/bin/$(triple)-objcopy
GDB     := $(ARMToolRoot)/bin/$(triple)-gdb
FLASH   := $(ARMToolRoot)/bin/lm4flash
QEMU    := /usr/local/bin/qemu-system-arm

#
#
#

productElf := $(productTarget:.bin=.elf)
rfind = $(shell if [ -d '$(1)' ]; then find '$(1)' -name '$(2)'; fi)
sources := $(call rfind,./$(sourceDir),*.c) $(call rfind,./$(sourceDir),*.cpp)
objFileList := $(addsuffix .o,$(patsubst ./$(sourceDir)/%,$(buildDir)/%, $(sources)))
sourceDirList := $(filter-out .,$(shell find ./$(sourceDir) -type d))
dSources := $(call rfind,$(buildDir),*.d)

CFLAGS += $(patsubst %,-I%,$(sourceDirList))
LFLAGS += -L$(buildDir)

#
#
#

all: $(productTarget)

all-clean: clean all

flash: all
	@$(FLASH) $(productTarget)

run: all
	@$(QEMU) -machine lm3s6965evb -cpu cortex-m3 -nographic -semihosting -kernel $(productElf)

gdb: all
	@$(GDB) -ex "target ext localhost:3333" -ex "mon reset halt" -ex "mon arm semihosting enable" -ex "load" $(productElf)

.PHONY: clean
clean:
	@rm -rf .build/*
	@rm -rf Product/*

#
#
#

$(productTarget): $(productElf)
	@mkdir -p $(dir $@)
	@$(OBJCOPY) -O binary $< $@
	@echo $$'\e[32m'"[completed] resulting binary size:"$$'\e[32;1m' `stat -f %z $@` $$'\e[0m'$$'\e[32m'"bytes"$$'\e[0m'
	
	
# @$(SIZE) $(productElf)

$(productElf): $(objFileList)
	@mkdir -p $(dir $@)
	@echo $$'\e[33m'"[linking  ] $(notdir $(productElf))"$$'\e[0m'
	@$(LD) $(LFLAGS) $(objFileList) $(LGROUPS) -o $(productElf)

$(buildDir)/%.c.o : $(sourceDir)/%.c
	@mkdir -p "$(dir $@)"
	@echo $$'\e[33m'"[compiling] $(notdir $<)"$$'\e[0m'
	@$(CC) $(CFLAGS) -o $@ -c $<

$(buildDir)/%.cpp.o : $(sourceDir)/%.cpp
	@mkdir -p "$(dir $@)"
	@echo $$'\e[33m'"[compiling] $(notdir $<)"$$'\e[0m'
	@$(CXX) -std=c++11 -fno-rtti $(CFLAGS) -o $@ -c $<

include $(dSources:.o=.d)
