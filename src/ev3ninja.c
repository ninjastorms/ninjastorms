#include <stdio.h>
#include <memory.h>

#include "feedback.h"
#include "scheduler.h"

#include <libp/led.h>
#include <libp/button.h>
#include <libp/sensor.h>
#include <libp/motor.h>

#include <libp/gpio.h>
#include <libp/spi.h>

void func_task_a(void) {
	while(1)
		puts("Heyho! I am currently executing Task A\n");
}

void func_task_b(void) {
	while(1)
		puts("Heyho! I am currently executing Task B\n");
}

int ev3ninja_main (void)
{
  static task_t task_a;
  static task_t task_b;
  task_t *tasks[] = { &task_a, &task_b };

  init_task(&task_a, (unsigned int)func_task_a, TASK_A_STACK_ADDRESS);
  init_task(&task_b, (unsigned int)func_task_b, TASK_B_STACK_ADDRESS);	

  start_scheduler(tasks);
  
  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");
  
  feedback_flash_green();

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  return 0;
}

