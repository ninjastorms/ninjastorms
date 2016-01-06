
#include <stdio.h>
#include <memory.h>

#include "feedback.h"
#include "interrupt.h"
//#include "segway.c"

#include <libp/led.h>
#include <libp/button.h>
#include <libp/sensor.h>
#include <libp/motor.h>

#include <libp/gpio.h>
#include <libp/spi.h>

#define REG_SP 13
#define REG_LR 14
#define REG_PC 15

#define CPSR_MODE_SVC 0x13
#define CPSR_ABORT (1 << 8)

void init_timer(void) {
  *TIMER1_CTRL &= ~(1 << 7); // unset TimerEn(abled)

  *TIMER1_CTRL |= 1 << 6;    // set periodic-mode
  *TIMER1_CTRL |= 1 << 5;    // set IntEnable
  *TIMER1_CTRL |= 1 << 1;    // set 32-bit mode
  //*TIMER1_CTRL |= 1 << 0;    // set OneShot-Mode
  *TIMER1_CTRL &= ~(1 << 0);    // set Wrapping-Mode

  *TIMER1_LOAD = 0x5000;     // load
  *TIMER1_CTRL |= 1 << 7;    // set TimerEn(abled)
  return;
}

typedef struct task {
	unsigned int reg[16];
	unsigned int cpsr;
} task_t;

void init_task(task_t *task, unsigned int entrypoint, unsigned int stackbase) {
	int i = 0;
	for(i = 0; i<16; i++) {
		task->reg[i] = 0;
	}
				
	task->reg[REG_SP] = stackbase;
	task->reg[REG_PC] = entrypoint;

  task->cpsr = 0;
	task->cpsr |= CPSR_ABORT || CPSR_MODE_SVC;
}

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

  init_task(&task_a, (unsigned int)func_task_a, TASK_A_STACK_ADDRESS);
  init_task(&task_b, (unsigned int)func_task_b, TASK_B_STACK_ADDRESS);	

  init_interrupt_handling();
  
  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");
  
  feedback_flash_green();

  init_timer();
  while(1) {
    unsigned int i = 0xfffffff;
    while(i != 0) {
      i--;
    }
    printf("Timer: %x\n", *TIMER1_VALUE);
  }
  //run_segway();

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  return 0;
}

