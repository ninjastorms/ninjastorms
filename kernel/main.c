#include <stdio.h>

#include "kernel/drivers/button.h"
#include "kernel/demo/demo_motor.h"
#include "kernel/demo/demo_led.h"
#include "kernel/scheduler.h"

void func_task_a(void) {
  while(1) {
    demo_motor();
  }
}

void func_task_b(void) {
  while(1) {
    demo_led();
  }
}

void controller_task(void) {
  unsigned int button_last_state = BUTTON_UP;
  unsigned int mode = 0;
  while(1) {
    if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN) {
      if (button_last_state == BUTTON_UP) {
        switch(mode) {
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
    } else {
      button_last_state = BUTTON_UP;
    }
  }
}

int kernel_main (void)
{
  puts("This is ninjastorms OS");
  puts("  shuriken ready");

  add_task(&controller_task);
  start_scheduler();

  puts("All done. ninjastorms out!");

  return 0;
}
