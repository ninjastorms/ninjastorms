
#include <stdio.h>

#define UART_THR (char*volatile)(0x01D0C000)
#define UART_LSR (char*volatile)(0x01D0C014)

int
putchar (int c)
{
  if (c == '\n')
    putchar('\r');

  while (!(*UART_LSR & (1 << 5)));

  *UART_THR = c;
  return c;
}
