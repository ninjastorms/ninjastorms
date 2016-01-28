#include <stdio.h>
#include <memory.h>

#include "demo_motor.h"
#include "demo_led.h"
#include "scheduler.h"

void func_task_a(void) {
  while(1) {
    control_motor();
  }
}
 
void func_task_b(void) {
  while(1) {
    puts("beginning of lightshow");
    lightshow();
  }
}


int ev3ninja_main (void)
{
  static task_t task_a;
  static task_t task_b;
  task_t *tasks[] = { &task_a, &task_b };

  init_task(&task_a, (unsigned int)func_task_a, TASK_A_STACK_ADDRESS);
  init_task(&task_b, (unsigned int)func_task_b, TASK_B_STACK_ADDRESS);	

  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");

  start_scheduler(tasks);

  while(1);

  puts("All done. ev3ninja out!");

  return 0;
}
