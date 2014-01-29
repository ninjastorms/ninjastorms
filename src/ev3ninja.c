
#include <stdio.h>

#include "feedback.h"

#include <libp/led.h>
#include <libp/button.h>
#include <libp/sensor.h>
#include <libp/motor.h>

#include <libp/gpio.h>
#include <libp/spi.h>

/* TODO:: get __attribute__((constructor)), __attribute__((destructor)) working and remove manual init/fini code */
extern void bss_init(void);
extern void spi_init(void);
extern void adc_init(void);
extern void led_init(void);
extern void button_init(void);
extern void sensor_init(void);
extern void motor_init(void);

static void
ev3ninja_init (void)
{
  bss_init();
  spi_init();
  adc_init();
  led_init();
  button_init();
  sensor_init();
  motor_init();
}

extern void spi_fini(void);

static void
ev3ninja_fini (void)
{
  spi_fini();
}

int 
ev3ninja_main (void)
{
  ev3ninja_init();

  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");
  feedback_flash_green();

  unsigned short speed = 0x7FFF; // stop
  int t = 0x7FFF;

  while (1)
    {
      if (button_get_state(BUTTON_TOP) == BUTTON_DOWN)
        {
          speed = (speed == 0xFFFF ? 0xFFFF : speed + 1);
        }

      if (button_get_state(BUTTON_BOTTOM) == BUTTON_DOWN)
        {
          speed = (speed == 0x0000 ? 0x0000 : speed - 1);
        }

      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        {
          speed = 0x7FFF;
          t = 0x7FFF;
        }

      t += ((int)speed) - 0x7FFF;
      if (t > 0xFFFF)
        {
          motor_set_state(MOTOR_PORT_2, 2);
          t -= 0x7FFF;
        }
      else if (t < 0x0000)
        {
          motor_set_state(MOTOR_PORT_2, 0);
          t += 0x7FFF;
        }
      else
        {
          motor_set_state(MOTOR_PORT_2, 1);
        }
    }

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  ev3ninja_fini();

  return 0;
}

/* bss init */
extern unsigned long __bss_start, _end;

void
__attribute__((constructor)) // <- does not work yet
bss_init (void)
{
  /* Zero out BSS */
  unsigned char * cp = (unsigned char *) &__bss_start;
  while (cp < (unsigned char *)&_end)
    *cp++ = 0;
}

