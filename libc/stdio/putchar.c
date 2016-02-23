
#include <stdio.h>

#ifdef QEMU
#define UART_THR (volatile char*)(0x101f1000)
#endif

#ifndef QEMU
#define UART_THR (volatile char*)(0x01D0C000)
#define UART_LSR (volatile char*)(0x01D0C014)
#endif

int
putchar (int c)
{
  if (c == '\n')
    putchar('\r');

  #ifndef QEMU
  while (!(*UART_LSR & (1 << 5)));
  #endif

  *UART_THR = c;
  return c;
}
