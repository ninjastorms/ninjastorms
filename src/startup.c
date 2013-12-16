
#include <libp/led.h>

extern unsigned long __bss_start, _end;

void
startup (void)
{
  /* Zero out BSS */
  unsigned char * cp = (unsigned char *) &__bss_start;
  while (cp < (unsigned char *)&_end)
    *cp++ = 0;

  /* initialize LED gpio pins */
  led_init();
}

