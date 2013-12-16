
#include <stdio.h>

#include "startup.h"
#include "util.h"

#include <libp/led.h>
#include <libp/button.h>

void rotate_lights(void);

int 
ev3ninja_main (__unused int argc, __unused char *argv[])
{
  startup();

  puts("hello world");

  printf("argc = %i\n", argc);

  int i;
  for (i = 0; i <= argc; ++i)
    printf("argv[%i] = \"%s\"\n", i, argv[i]);

  while (1)
    {
      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        rotate_lights();
      volatile unsigned int timer = 0;
      while (timer < 1024 * 512) ++timer;
    }

  puts("All done. ev3ninja out!");

  return 0;
}

void 
rotate_lights (void)
{
  unsigned int state = 0;
  while (1)
    {
      led_set(LED_LEFT, state + 2);
      led_set(LED_RIGHT, state);

      volatile unsigned int timer = 0;
      while (timer < 1024 * 512) ++timer;
      state = (state + 1) % 4;

      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        {
          led_set(LED_LEFT, LED_BLACK);
          led_set(LED_RIGHT, LED_BLACK);
          return;
        }
    }
}
