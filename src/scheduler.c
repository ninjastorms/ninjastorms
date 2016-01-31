#include "scheduler.h"
#include "interrupt.h"
#include "interrupt_handler.h"

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
// with the value 0x100000 you can feel the delay
// TODO: calculate conversion to milliseconds
#define TIMER_LOAD_VALUE 0x10000
#endif

#define MAX_TASK_NUMBER 16

// the SVC task's task object is tasks[0] and don't have to be initialised
// this is done in save_current_task_state() during the first context switch

int task_count = 0;
int buffer_start = 0;
int buffer_end = 0;
task_t* ring_buffer[MAX_TASK_NUMBER];
task_t tasks[MAX_TASK_NUMBER];
task_t* current_task;
int isRunning = 0;

void ring_buffer_insert(task_t* task) {
  int new_end = (buffer_end + 1) % MAX_TASK_NUMBER;
  if (new_end != buffer_start) {
    ring_buffer[buffer_end] = task;
    buffer_end = new_end;
  }
}

task_t* ring_buffer_remove(void) {
  if (buffer_start == buffer_end) {
    return 0;
  }
  task_t* task = ring_buffer[buffer_start];
  buffer_start = (buffer_start + 1) % MAX_TASK_NUMBER;
  return task;
}

void init_task(task_t* task, void* entrypoint, unsigned int stackbase) {
  int i;
  for(i = 0; i<16; i++) {
    task->reg[i] = 0;
  }
        
  task->reg[REG_SP] = stackbase;
  task->reg[REG_PC] = (unsigned int) entrypoint;

  task->cpsr = CPSR_MODE_USER;
}

// context switch while add_task may fuck up something
void add_task(void* entrypoint) {
  if (task_count >= MAX_TASK_NUMBER) {
    return;
  }
  unsigned int stackbase = TASK_STACK_BASE_ADDRESS - STACK_SIZE*task_count;
  init_task(&tasks[task_count], entrypoint, stackbase);
  ring_buffer_insert(&tasks[task_count]);
  task_count++;
}

void schedule(void) {
  ring_buffer_insert(current_task);
  current_task = ring_buffer_remove();
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
  *TIMER0_TGCR &= ~TIM34RS_REMOVE;     // reset timer
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

void stop_timer(void) {
#ifdef QEMU
  *TIMER1_CTRL &= ~(1 << 7);        // disable timer
  *TIMER1_INTCLR = (char)0x1;       // clear interrupts
#endif
#ifndef QEMU
  *TIMER0_TCR  &= ~ENAMODE34;          // disable timer
  *TIMER0_INTCTLSTAT |= PRDINTSTAT34;  // clear interrupts
#endif
}

void start_scheduler() {
  if (!isRunning) {
    current_task = ring_buffer_remove();
    isRunning = 1;
    stop_timer();
    init_interrupt_handling();
    init_timer();
    load_current_task_state();
  }
}
