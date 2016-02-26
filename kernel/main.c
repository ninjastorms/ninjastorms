
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

#include "main.h"

#include "kernel/drivers/button.h"
#include "kernel/demo/demo_motor.h"
#include "kernel/demo/demo_led.h"
#include "kernel/scheduler.h"

#include <stdio.h>

static void
func_task_a (void)
{
  while(1)
    demo_motor();
}

static void
func_task_b (void)
{
  while(1)
    demo_led();
}

static void
controller_task (void)
{
  unsigned int button_last_state = BUTTON_UP;
  unsigned int mode = 0;
  while(1)
    {
      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        {
          if (button_last_state == BUTTON_UP)
            {
              switch(mode)
                {
                case 0:
                  add_task(func_task_b);
                  mode++;
                  break;
                case 1:
                  add_task(func_task_a);
                  mode++;
                  break;
                default:
                  mode++;
                  //TODO: shutdown
                }
            }
          button_last_state = BUTTON_DOWN;
        }
      else
        {
          button_last_state = BUTTON_UP;
        }
    }
}

int
kernel_main (void)
{
  puts("This is ninjastorms OS");
  puts("  shuriken ready");

  add_task(&controller_task);
  start_scheduler();

  puts("All done. ninjastorms out!");

  return 0;
}
