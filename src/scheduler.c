#include "scheduler.h"

#define REG_SP 13
#define REG_LR 14
#define REG_PC 15

#define CPSR_MODE_SVC 0x13
#define CPSR_ABORT (1 << 8)

task_t *current_task;
task_t *other_task;

void init_task(task_t *task, unsigned int entrypoint, unsigned int stackbase) {
  int i;
  for(i = 0; i<16; i++) {
    task->reg[i] = 0;
  }
				
  task->reg[REG_SP] = stackbase;
  task->reg[REG_PC] = entrypoint;

  task->cpsr = 0;
  task->cpsr |= CPSR_ABORT || CPSR_MODE_SVC;
}
void start_scheduler(task_t *tasks[]) {
  current_task = tasks[0];
  other_task = tasks[1];
	
  init_interrupt_handling();
  init_timer();

  load_task(current_task);
}

void load_task(task_t *task) {
  //
}

void init_timer(void) {
  *TIMER1_CTRL &= ~(1 << 7); // unset TimerEn(abled)

  *TIMER1_CTRL |= 1 << 6;    // set periodic-mode
  *TIMER1_INTCLR = (char)0x1; // clear interrupts
  *TIMER1_CTRL |= 1 << 5;    // set IntEnable
  *TIMER1_CTRL |= 1 << 1;    // set 32-bit mode
  //*TIMER1_CTRL |= 1 << 0;    // set OneShot-Mode
  *TIMER1_CTRL &= ~(1 << 0);    // set Wrapping-Mode

  *TIMER1_LOAD = 0x5000;     // load
  *TIMER1_CTRL |= 1 << 7;    // set TimerEn(abled)
  return;
}
