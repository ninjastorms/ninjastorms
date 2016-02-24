
## process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = -I./include
AM_CCASFLAGS = $(AM_CFLAGS) $(CFLAGS)

noinst_LTLIBRARIES = \
    libc/libc.la \
    libp/libp.la

libc_libc_la_SOURCES = \
    libc/errno/errno.c \
    libc/stdio/printf.c \
    libc/stdio/putchar.c \
    libc/stdio/puts.c \
    libc/stdio/vprintf.c \
    libc/string/memcmp.c \
    libc/string/memcpy.c \
    libc/string/memset.c

libp_libp_la_SOURCES = \
    libp/adc.c libp/adc.h \
    libp/button.c libp/button.h \
    libp/gpio.c libp/gpio.h \
    libp/led.c libp/led.h \
    libp/motor.c libp/motor.h \
    libp/pininfo.c libp/pininfo.h \
    libp/sensor.c libp/sensor.h \
    libp/spi.c libp/spi.h \
    libp/timer.c libp/timer.h

libp_libp_la_LIBADD = \
    libc/libc.la

noinst_PROGRAMS = ev3ninja

ev3ninja_SOURCES = \
    src/demo_led.c src/demo_led.h \
    src/demo_motor.c src/demo_motor.h \
    src/ev3ninja.c src/ev3ninja.h \
    src/feedback.c src/feedback.h \
    src/interrupt.c src/interrupt.h \
    src/interrupt_handler.S src/interrupt_handler.h \
    src/boot.S \
    src/scheduler.c src/scheduler.h

ev3ninja_LDADD = \
    libc/libc.la \
    libp/libp.la

ev3ninja_LDFLAGS = -T src/link-arm-eabi.ld
