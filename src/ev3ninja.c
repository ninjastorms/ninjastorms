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

void func_task_a(void) {
  volatile unsigned int i;
  puts("Heyho! I am currently executing Task A\n");
  while(1) {
    putchar('A');
    for(i=0;i<65000;i++);
  }
}

void func_task_b(void) {
  volatile unsigned int i;
  puts("Heyho! I am currently executing Task B\n");
  while(1) {
    putchar('B');
    for(i=0;i<65000;i++);
  }
}

int ev3ninja_main (void)
{
  static task_t task_a;
  static task_t task_b;
  //task_t *tasks[] = { &task_a, &task_b };

  init_task(&task_a, (unsigned int)func_task_a, TASK_A_STACK_ADDRESS);
  init_task(&task_b, (unsigned int)func_task_b, TASK_B_STACK_ADDRESS);	

  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");

  printf("%x\t%x\t%x\n", *TIMER0_TIM34, *TIMER0_INTCTLSTAT, *AINTC_SECR1);
  *AINTC_SECR1 = 0x400000;
  printf("%x\t%x\t%x\n", *TIMER0_TIM34, *TIMER0_INTCTLSTAT, *AINTC_SECR1);
  
  //asm("SWI 1");
  //start_scheduler(tasks);
  init_interrupt_handling();
  init_timer();

  volatile unsigned int i;
  volatile unsigned int j;
  while (1) {
    //puts("reset interrupt\n");
    //*TIMER0_INTCTLSTAT |= PRDINTSTAT34;
    for(i=0;i<50;i++) {
      for(j=0;j<65000;j++);
      printf("%x\t%x\t%x\n", *TIMER0_TIM34, *TIMER0_INTCTLSTAT, *AINTC_SECR1);
    }
  }
  
  feedback_flash_green();

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  return 0;
}
