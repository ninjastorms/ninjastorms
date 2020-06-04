
/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#include "scheduler.h"
#include "kernel/utilities.h"
#include "kernel/memory.h"
#include "kernel/drivers/timer.h"
#include "kernel/interrupt.h"
#include "kernel/interrupt_handler.h"

#include <errno.h>

#define CPSR_MODE_SVC  0x13
#define CPSR_MODE_USER 0x10

#if BOARD_VERSATILEPB
#  define TIMER_LOAD_VALUE 0x2000
#endif

#if BOARD_EV3
#  define TIMER_LOAD_VALUE 0x10000
#endif

#define MAX_TASK_NUMBER 16

int task_count   = 0;
int buffer_start = 0;
int buffer_end   = 0;
int isRunning    = 0;
task_t tasks[MAX_TASK_NUMBER] = { 0 };
task_t* ring_buffer[MAX_TASK_NUMBER] = { 0 };
task_t* current_task = (void*)0;

// TODO: disable interrupts during insertion
void
ring_buffer_insert (task_t *task)
{
  int new_end = (buffer_end + 1) % MAX_TASK_NUMBER;
  if (new_end != buffer_start)
    {
      ring_buffer[buffer_end] = task;
      buffer_end = new_end;
    }
}

task_t*
ring_buffer_remove (void)
{
  if (buffer_start == buffer_end)
    return 0;

  task_t* task = ring_buffer[buffer_start];
  buffer_start = (buffer_start + 1) % MAX_TASK_NUMBER;
  return task;
}

void
init_task (task_t *task, void *entrypoint, unsigned int stackbase)
{
  int i;
  for(i = 0; i<13; i++)
    task->reg[i] = i;

  task->sp = stackbase;
  task->lr = 0;
  task->lr = 0;
  task->pc = (unsigned int) entrypoint;

  task->cpsr = CPSR_MODE_USER;
}

int
add_task (void *entrypoint)
{
  if (!is_privileged()){
      errno = EPERMISSION;
      return -1;
  }
  if (task_count >= MAX_TASK_NUMBER){
    errno = ETOOMANYTASKS;
    return -1;
  }

  unsigned int stackbase = TASK_STACK_BASE_ADDRESS - STACK_SIZE*task_count;
  init_task(&tasks[task_count], entrypoint, stackbase);
  ring_buffer_insert(&tasks[task_count]);
  task_count++;
  return 0;
}

void
schedule (void)
{
  ring_buffer_insert(current_task);
  current_task = ring_buffer_remove();
}

void
start_scheduler (void)
{
  if (!isRunning)
    {
      current_task = ring_buffer_remove();
      isRunning = 1;
      timer_stop();
      init_interrupt_handling();
      timer_start(TIMER_LOAD_VALUE);
      load_current_task_state();
    }
}
