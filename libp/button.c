
#include "button.h"
#include "gpio.h"

#include <stdio.h>

unsigned int buttons[] = 
{
  GPIO_PIN(6,  6),
  GPIO_PIN(7, 12),
  GPIO_PIN(7, 15),
  GPIO_PIN(7, 14),
  GPIO_PIN(1, 13),
  GPIO_PIN(6, 10)
};


void 
button_init (void)
{
  gpio_init_inpin(buttons[0]);
  gpio_init_inpin(buttons[1]);
  gpio_init_inpin(buttons[2]);
  gpio_init_inpin(buttons[3]);
  gpio_init_inpin(buttons[4]);
  gpio_init_inpin(buttons[5]);

  // disable pull-up for pin group 25 - required for left button
  *((volatile unsigned int*)(SYSCFG1_BASE + 0x10)) &= 0xFDFFFFFF;
}

button_state
button_get_state (button_id button)
{
  return gpio_get(buttons[button]);
}
