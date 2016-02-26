
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>

#if BOARD_QEMU
#define UART_THR (volatile char*)(0x101f1000)
#endif

#if BOARD_EV3
#define UART_THR (volatile char*)(0x01D0C000)
#define UART_LSR (volatile char*)(0x01D0C014)
#endif

int
putchar (int c)
{
  if (c == '\n')
    putchar('\r');

  #if BOARD_EV3
  while (!(*UART_LSR & (1 << 5)));
  #endif

  *UART_THR = c;
  return c;
}
