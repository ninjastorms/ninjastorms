
#include <stdio.h>

#include "feedback.h"

#include <libp/led.h>
#include <libp/button.h>
#include <libp/sensor.h>
#include <libp/motor.h>

#include <libp/gpio.h>
#include <libp/spi.h>

int 
ev3ninja_main (void)
{
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

  return 0;
}

