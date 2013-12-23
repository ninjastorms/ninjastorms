
#include <stdio.h>

#include "startup.h"
#include "util.h"

#include <libp/led.h>
#include <libp/button.h>

void rotate_lights(void);
void signal_ready(void);
void wait(void);

int 
ev3ninja_main (__unused int argc, __unused char *argv[])
{
  startup();

  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");

  signal_ready();

  while (1)
    {
      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        rotate_lights();
      wait();
    }

  puts("All done. ev3ninja out!");

  return 0;
}

void 
wait (void)
{
  volatile unsigned int timer = 0;
  while (timer < 1024 * 512) ++timer;
}

void 
rotate_lights (void)
{
  unsigned int state = 0;
  while (1)
    {
      led_set(LED_LEFT, state + 2);
      led_set(LED_RIGHT, state);

      wait();

      state = (state + 1) % 4;

      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        {
          led_set(LED_LEFT, LED_BLACK);
          led_set(LED_RIGHT, LED_BLACK);
          return;
        }
    }
}

void
signal_ready (void)
{
  led_set(LED_LEFT, LED_GREEN);
  led_set(LED_RIGHT, LED_GREEN);

  wait();

  led_set(LED_LEFT, LED_BLACK);
  led_set(LED_RIGHT, LED_BLACK);

  wait();

  led_set(LED_LEFT, LED_GREEN);
  led_set(LED_RIGHT, LED_GREEN);
  
  wait();

  led_set(LED_LEFT, LED_BLACK);
  led_set(LED_RIGHT, LED_BLACK);

  wait();

  led_set(LED_LEFT, LED_GREEN);
  led_set(LED_RIGHT, LED_GREEN);

  wait();
 
  led_set(LED_LEFT, LED_BLACK);
  led_set(LED_RIGHT, LED_BLACK);
}
