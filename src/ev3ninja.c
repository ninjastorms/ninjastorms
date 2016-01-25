#include <stdio.h>
#include <memory.h>

#include "feedback.h"
#include "scheduler.h"
#include "interrupt_handler.h"
#include "interrupt.h"

#include <libp/led.h>
#include <libp/button.h>
#include <libp/sensor.h>
#include <libp/motor.h>

#include <libp/gpio.h>
#include <libp/spi.h>

void wait(void) {
  volatile unsigned long i;
  for(i=0;i<65000000;i++);
}

void actual_wait(void) {
  wait();
}

void func_task_a(void) {
  while(1) {
    puts("Hi, I am task A");
    actual_wait();
  }
}

void func_task_b(void) {
  while(1) {
    puts("Hi, I am task B");
    actual_wait();
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

  //asm("SWI 1");
  start_scheduler(tasks);
  
  feedback_flash_green();

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  return 0;
}
