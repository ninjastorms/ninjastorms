
dnl process this file with autoconf to produce configure script

AC_PREREQ([2.69])
AC_INIT([ev3ninja], [0.2], [andreas@grapentin.org])
AC_CONFIG_SRCDIR([src/ev3ninja.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AM_PROG_AS
AM_PROG_AR

AC_CANONICAL_HOST

case $host_cpu in
  arm)
    AX_APPEND_FLAG([-msoft-float -marm -mabi=aapcs-linux -march=armv5te -mno-thumb-interwork])
    ;;
  *)
    AC_MSG_ERROR([build is not supported on $host_cpu])
    ;;
esac

AC_ARG_ENABLE

# make the binary freestanding
AX_APPEND_FLAG([-nostdlib -nostartfiles -ffreestanding])

# add -fno-delete-null-pointer-checks if the compiler accepts it
# this is required to write the interrupt vector table to 0x0 with gcc>4.9
AX_CHECK_COMPILE_FLAG([-fno-delete-null-pointer-checks], [
  AX_APPEND_FLAG([-fno-delete-null-pointer-checks])
])

LT_INIT

AC_CONFIG_FILES(Makefile)

AC_OUTPUT
