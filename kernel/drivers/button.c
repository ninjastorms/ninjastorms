
/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#include "button.h"

#include "kernel/drivers/gpio.h"

#include <stdio.h>

/* this struct holds the information on the Button pins
 *
 * each Button has only a single input pin
 */
struct button_info
{
  unsigned int pin;
};
typedef struct button_info button_info;

button_info buttons[] =
{
  { GPIO_PIN(6,  6) },
  { GPIO_PIN(7, 12) },
  { GPIO_PIN(7, 15) },
  { GPIO_PIN(7, 14) },
  { GPIO_PIN(1, 13) },
  { GPIO_PIN(6, 10) }
};

button_state
button_get_state (button_id button)
{
  return gpio_get(buttons[button].pin);
}

/* initialize the gpio pins necessary for Button functions
 * this is done automatically on startup
 */
static void
__attribute((constructor))
button_init (void)
{
  gpio_init_inpin(buttons[0].pin);
  gpio_init_inpin(buttons[1].pin);
  gpio_init_inpin(buttons[2].pin);
  gpio_init_inpin(buttons[3].pin);
  gpio_init_inpin(buttons[4].pin);
  gpio_init_inpin(buttons[5].pin);

  // disable pull-up for pin group 25 - required for left button
  *((volatile unsigned int*)(SYSCFG1_BASE + 0x10)) &= 0xFDFFFFFF;
}
