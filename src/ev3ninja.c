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

void func_task_a(void) {
  volatile unsigned int i;
  puts("Heyho! I am currently executing Task A\n");
  while(1) {
    putchar('A');
    for(i=0;i<65000;i++);
    puts("Hi, I am task A");
    wait();
  }
}
 
void func_task_b(void) {
  volatile unsigned int i;
  puts("Heyho! I am currently executing Task B\n");
  while(1) {
    putchar('B');
    for(i=0;i<65000;i++);
    puts("Hi, I am task B");
    wait();
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

  puts("irq");
  *(unsigned int*) IRQ_STACK_ADDRESS = 0xdeadbeef;
  *(unsigned int*) (IRQ_STACK_ADDRESS-4) = 0xdeadbee1;
  *(unsigned int*) (IRQ_STACK_ADDRESS-8) = 0xdeadbee2;
  printf("sp content: %x\n", *(unsigned int*) IRQ_STACK_ADDRESS);
  printf("sp content: %x\n", *(unsigned int*) (IRQ_STACK_ADDRESS-4));
  printf("sp content: %x\n", *(unsigned int*) (IRQ_STACK_ADDRESS-8));
  puts("svc");
  *(unsigned int*) SVC_STACK_ADDRESS = 0xdeadbeef;
  *(unsigned int*) (SVC_STACK_ADDRESS-4) = 0xdeadbee1;
  *(unsigned int*) (SVC_STACK_ADDRESS-8) = 0xdeadbee2;
  printf("sp content: %x\n", *(unsigned int*) SVC_STACK_ADDRESS);
  printf("sp content: %x\n", *(unsigned int*) (SVC_STACK_ADDRESS-4));
  printf("sp content: %x\n", *(unsigned int*) (SVC_STACK_ADDRESS-8));
  puts("task a");
  *(unsigned int*) TASK_A_STACK_ADDRESS = 0xdeadbeef;
  *(unsigned int*) (TASK_A_STACK_ADDRESS-4) = 0xdeadbee1;
  *(unsigned int*) (TASK_A_STACK_ADDRESS-8) = 0xdeadbee2;
  printf("sp content: %x\n", *(unsigned int*) TASK_A_STACK_ADDRESS);
  printf("sp content: %x\n", *(unsigned int*) (TASK_A_STACK_ADDRESS-4));
  printf("sp content: %x\n", *(unsigned int*) (TASK_A_STACK_ADDRESS-8));
  puts("task b");
  *(unsigned int*) TASK_B_STACK_ADDRESS = 0xdeadbeef;
  *(unsigned int*) (TASK_B_STACK_ADDRESS-4) = 0xdeadbee1;
  *(unsigned int*) (TASK_B_STACK_ADDRESS-8) = 0xdeadbee2;
  printf("sp content: %x\n", *(unsigned int*) TASK_B_STACK_ADDRESS);
  printf("sp content: %x\n", *(unsigned int*) (TASK_B_STACK_ADDRESS-4));
  printf("sp content: %x\n", *(unsigned int*) (TASK_B_STACK_ADDRESS-8));

  unsigned int sp = 0xdaed;
  asm volatile ("mov %0, sp" : "=r" (sp) :);
  printf("sp: %x\n", sp);

  init_interrupt_handling();

  asm(
    "mrs  r0, cpsr\n"
    "bic  r0, #0x1f\n" // Clear mode bits
    "orr  r0, #0x12\n" // Select IRQ mode
    "msr  cpsr, r0\n"  // Enter IRQ mode
  );

  puts("me work");

  asm(
    "bic  r0, #0x1f\n" // Clear mode bits
    "orr  r0, #0x13\n" // Select SVC mode
    "msr  cpsr, r0\n"  // Enter SVC mode
  );
  //asm("SWI 1");
  //start_scheduler(tasks);
  init_timer();

  while (1) {
    //puts("reset interrupt\n");
    //*TIMER0_INTCTLSTAT |= PRDINTSTAT34;
    printf("%x\t%x\t%x\n", *TIMER0_TIM34, *TIMER0_INTCTLSTAT, *AINTC_SECR1);
    wait();
  }
  
  feedback_flash_green();

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  return 0;
}
