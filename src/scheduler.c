#include "scheduler.h"
#include "interrupt.h"
#include "interrupt_handler.h"

#include <stdio.h>
#include <memory.h>

#define REG_SP 13
#define REG_LR 14
#define REG_PC 15

#define CPSR_MODE_SVC  0x13
#define CPSR_MODE_USER 0x10

#ifdef QEMU
#define TIMER_LOAD_VALUE 0x2000
#endif

#ifndef QEMU
#define TIMER_LOAD_VALUE 0xFFFFF
#endif

task_t *current_task;
task_t *other_task;

void init_task(task_t *task, unsigned int entrypoint, unsigned int stackbase) {
  int i;
  for(i = 0; i<16; i++) {
    task->reg[i] = 0;
  }
				
  task->reg[REG_SP] = stackbase;
  task->reg[REG_PC] = entrypoint;

  task->cpsr = CPSR_MODE_USER;
}

void schedule(void) {
  task_t *temp_task;

  temp_task = current_task;
  current_task = other_task;
  other_task = temp_task;
}

void init_timer(void) {
#ifdef QEMU
  *TIMER1_CTRL &= ~(1 << 7);        // disable timer
  *TIMER1_CTRL |= 1 << 6;           // set periodic-mode
  *TIMER1_INTCLR = (char)0x1;       // clear interrupts
  *TIMER1_CTRL |= 1 << 5;           // set IntEnable
  *TIMER1_CTRL |= 1 << 1;           // set 32-bit mode
  *TIMER1_CTRL &= ~(1 << 0);        // set Wrapping-Mode
  *TIMER1_LOAD  = TIMER_LOAD_VALUE; // set timer period
  *TIMER1_CTRL |= 1 << 7;           // start timer
#endif

#ifndef QEMU
  *TIMER0_TCR  &= ~ENAMODE34;          // disable timer
  *TIMER0_TGCR |= ~TIM34RS_REMOVE;     // reset timer
  *TIMER0_TGCR &= ~TIMMODE;            // reset mode bits
  *TIMER0_TGCR |= TIMMODE_UNCHAINED;   // set dual 32 bit unchained mode
  *TIMER0_TGCR |= TIM34RS_REMOVE;      // remove timer from reset
  *TIMER0_PRD34 = TIMER_LOAD_VALUE;    // set timer period
  *TIMER0_TGCR &= ~PSC34;              // reset prescaler
  *TIMER0_TGCR |= PSC34_VALUE;         // set prescaler
  *TIMER0_INTCTLSTAT |= PRDINTSTAT34;  // clear interrupts
  *TIMER0_INTCTLSTAT |= PRDINTEN34;    // enable interrupts
  *TIMER0_TCR  |= ENAMODE34_CONTIN;    // set continuously-mode, start timer
#endif

  return;
}

void start_scheduler(task_t *tasks[]) {
  current_task = tasks[0];
  other_task = tasks[1];
	
  init_interrupt_handling();
  init_timer();

  load_current_task_state();
}
