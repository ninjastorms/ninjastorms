
dnl process this file with autoconf to produce configure script

AC_PREREQ([2.69])
AC_INIT([ninjastorms], [0.2], [andreas@grapentin.org])
AC_CONFIG_SRCDIR([kernel/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AM_PROG_AS
AM_PROG_AR

AC_CANONICAL_HOST

dnl make sure we configure a cross build for an arm target
case $host_cpu in
  arm)
    ;;
  *)
    AC_MSG_ERROR([unsupported host '$host'])
    ;;
esac

dnl figure out which board we configure for and set the according variables
case $BOARD in
  ev3)
    AC_SUBST(LOADADDR, [0xC1000000])
    AC_DEFINE_UNQUOTED(BOARD_EV3, 1, [Configured for EV3])
    ;;
  versatilepb)
    AC_SUBST(LOADADDR, [0x00010000])
    AC_DEFINE_UNQUOTED(BOARD_VERSATILEPB, 1, [Configured for qemu VersatilePB])
    ;;
  *)
    AC_MSG_ERROR([unsupported board '$BOARD'])
    ;;
esac

# add -fno-delete-null-pointer-checks if the compiler accepts it
# this is required to write the interrupt vector table to 0x0 with gcc>4.9
AX_CHECK_COMPILE_FLAG([-fno-delete-null-pointer-checks], [
  AX_APPEND_FLAG([-fno-delete-null-pointer-checks])
])

LT_INIT

AC_CONFIG_FILES(Makefile)

AC_OUTPUT
