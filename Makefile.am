
## process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = -I./include
AM_CCASFLAGS = $(AM_CFLAGS) $(CFLAGS)
AM_LDFLAGS = --oformat=binary

noinst_LTLIBRARIES = libc/libc.la

libc_libc_la_SOURCES = \
    libc/errno/errno.c \
    libc/stdio/printf.c \
    libc/stdio/putchar.c \
    libc/stdio/puts.c \
    libc/stdio/vprintf.c \
    libc/string/memcmp.c \
    libc/string/memcpy.c \
    libc/string/memset.c

noinst_PROGRAMS = ninjastorms

ninjastorms_SOURCES = \
    kernel/boot/start.S \
    kernel/main.c kernel/main.h \
    kernel/scheduler.c kernel/scheduler.h \
    kernel/interrupt.c kernel/interrupt.h \
    kernel/interrupt_handler.S kernel/interrupt_handler.h \
    kernel/feedback.c kernel/feedback.h \
    kernel/demo/demo_led.c kernel/demo/demo_led.h \
    kernel/demo/demo_motor.c kernel/demo/demo_motor.h \
    kernel/drivers/adc.c kernel/drivers/adc.h \
    kernel/drivers/button.c kernel/drivers/button.h \
    kernel/drivers/gpio.c kernel/drivers/gpio.h \
    kernel/drivers/led.c kernel/drivers/led.h \
    kernel/drivers/motor.c kernel/drivers/motor.h \
    kernel/drivers/pininfo.c kernel/drivers/pininfo.h \
    kernel/drivers/sensor.c kernel/drivers/sensor.h \
    kernel/drivers/spi.c kernel/drivers/spi.h \
    kernel/drivers/timer.c kernel/drivers/timer.h

ninjastorms_LDADD = libc/libc.la
ninjastorms_LDFLAGS = -T kernel/link-arm-eabi.ld -Ttext $(LOADADDR)

# workaround for generating plain binaries for arm targets
all-local: $(noinst_PROGRAMS)
	if file $< | grep executable >/dev/null; then $(STRIP) -O binary $<; fi
