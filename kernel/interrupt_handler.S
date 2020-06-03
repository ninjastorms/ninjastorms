
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

#include "kernel/memory.h"

.section .text

// import
.globl schedule

// export
.globl irq_handler_swi
.globl irq_handler_timer
.type irq_handler STT_FUNC
.globl load_current_task_state
.type load_current_task_state STT_FUNC


irq_handler_timer:
  // save registers
  push  {r0-r2, lr}

  mov  r0, sp   // set argument of save_current_task_state
  bl  save_current_task_state
  bl  schedule

#if BOARD_VERSATILEPB
  // clear interrupt
  ldr  r0, =TIMER1_INTCLR_ASM
  mov  r1, #0x1
  str  r1, [r0]
#endif

#if BOARD_EV3
  // clear interrupt
  ldr r0, =TIMER0_INTCTLSTAT_ASM
  mov r1, CLEARTIMER34_ASM
  str r1, [r0]
  // clear interrupt in interrupt controller
  ldr r0, =AINTC_SECR1_ASM
  mov r1, T64P0_TINT34_ASM
  str r1, [r0]
#endif

  pop  {r0-r2, lr}

  b  load_current_task_state
  

// the first parameter (r0) of save_current_task_state
// contains the address to the saved registers
save_current_task_state:
  ldr  r2, =current_task // load current_task
  ldr  r2, [r2]          // dereference current_task, to get the task_struct
  // save r0-r2
  ldr  r1, [r0], #4     // load saved r0 from stack
  str  r1, [r2], #4     // save it to the struct
  ldr  r1, [r0], #4     // load saved r1 from stack
  str  r1, [r2], #4     // save it to the struct
  ldr  r1, [r0], #4     // load saved r2 from stack
  str  r1, [r2], #4     // save it to the struct
  // save r3-r12, sp, lr
  stm  r2!, {r3-r12, sp, lr}^   // uses sp and lr from user mode because of the carot
  // save pc
  ldr  r1, [r0]         // load saved lr from stack
  sub  r1, #4           // because lr is the old pc+4
  str  r1, [r2], #4     // save r1 to the pc field in the task_struct
  // save cpsr
  mrs  r1, spsr         // save cpsr of the current task (in spsr atm)
  str  r1, [r2], #4     // save it to the struct

  bx   lr


load_current_task_state:
  ldr  r0, =current_task // load current_task
  ldr  r0, [r0]          // dereference current_task, to get the task_struct
  ldr  r1, [r0, #64]     // load cpsr from task_struct to r1 (i.e. task_struct+64)
  msr  spsr, r1          // copy r1 to spsr
  add  lr, r0, #60       // load address of saved pc into lr
  ldm  r0, {r0-r14}^     // load saved registers into user mode registers ((do not) trust the caret!)
  ldm  lr, {pc}^         // return to loaded task and restore cpsr from spsr
