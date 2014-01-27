
#include "feedback.h"

#include <libp/led.h>

void
feedback_wait (unsigned long ticks)
{
  volatile unsigned int timer = 0;
  while (timer < ticks) ++timer;
}

void
feedback_flash_green (void)
{
  unsigned int i;
  for (i = 0; i < 6; ++i)
    {
      led_set(LED_BOTH, (i & 1 ? LED_BLACK : LED_GREEN));
      feedback_wait(1024 * 1024);
    }
}

void
feedback_flash_red (void)
{
  unsigned int i;
  for (i = 0; i < 6; ++i)
    {
      led_set(LED_BOTH, (i & 1 ? LED_RED : LED_GREEN));
      feedback_wait(1024 * 1024);
    }
}

