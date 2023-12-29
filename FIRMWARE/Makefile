CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -O3 -ffunction-sections -fdata-sections \
            -I. -Iinclude -Icmsis_core/CMSIS/Core/Include -Icmsis_m0/Include -DSTM32G030xx \
            -mcpu=cortex-m0plus -mthumb  $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c syscalls.c 
SOURCES += $(wildcard drivers/**/*.c)
SOURCES += cmsis_m0/Source/Templates/gcc/startup_stm32g030xx.s # ST startup file. Compiler-dependent!

HEADERS = $(wildcard drivers/**/*.h)
HEADERS += util.h

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -rf
endif

build: firmware.bin

firmware.elf: cmsis_core cmsis_m0 $(HEADERS) link.ld Makefile $(SOURCES) 
	@echo "$(SOURCES)"
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --connect-under-reset write $< 0x8000000

cmsis_core:
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_m0:
	git clone --depth 1 -b v1.4.3 https://github.com/STMicroelectronics/cmsis_device_g0 $@

debug: flash
	openocd -f interface/stlink.cfg -f target/stm32g0x.cfg -c 'init; reset halt'

clean:
	$(RM) firmware.* cmsis_*
