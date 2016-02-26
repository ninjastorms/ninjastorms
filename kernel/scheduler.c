#include "kernel/memory.h"
#include "kernel/drivers/timer.h"
#include "kernel/scheduler.h"
#include "kernel/interrupt.h"
#include "kernel/interrupt_handler.h"

#define REG_SP 13
#define REG_LR 14
#define REG_PC 15

#define CPSR_MODE_SVC  0x13
#define CPSR_MODE_USER 0x10

#if BOARD_QEMU
#define TIMER_LOAD_VALUE 0x2000
#endif

#if BOARD_EV3
#define TIMER_LOAD_VALUE 0x10000
#endif

#define MAX_TASK_NUMBER 16

int task_count   = 0;
int buffer_start = 0;
int buffer_end   = 0;
int isRunning    = 0;
task_t tasks[MAX_TASK_NUMBER];
task_t* ring_buffer[MAX_TASK_NUMBER];
task_t* current_task;

// TODO: disable interrupts during insertion
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
    task->reg[i] = i;
  }

  task->reg[REG_SP] = stackbase;
  task->reg[REG_PC] = (unsigned int) entrypoint;

  task->cpsr = CPSR_MODE_USER;
}

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

void start_scheduler() {
  if (!isRunning) {
    current_task = ring_buffer_remove();
    isRunning = 1;
    timer_stop();
    init_interrupt_handling();
    timer_start(TIMER_LOAD_VALUE);
    load_current_task_state();
  }
}
