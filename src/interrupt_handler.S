#include <memory.h>

.section .text

// import
.globl puts
.globl schedule

// export
.globl irq_handler
.type irq_handler STT_FUNC
.globl load_current_task_state
.type load_current_task_state STT_FUNC


irq_handler:
  // for less latency save only registers that will change
  push  {r0-r12, lr}

  bl  save_current_task_state
  bl  schedule

#ifdef QEMU
  // clear interrupt
  ldr  r0, =TIMER1_INTCLR_ASM
  mov  r1, #0x1
  str  r1, [r0]
#endif

#ifndef QEMU
  // clear interrupt
  ldr r0, =TIMER0_INTCTLSTAT_ASM
  mov r1, PRDINTSTAT34_ASM
  str r1, [r0]
#endif

  ldr  r0, =message
  bl   puts

  pop  {r0-r12, lr}

  b  load_current_task_state


save_current_task_state:
  // save r0-r12
  ldr  r4, =current_task // load current_task
  ldr  r0, [r4]         // dereference current_task, to get the task_struct
  mov  r2, sp           // save sp to iterate over it
  mov  r3, #13          // iteration count; r0-r12 is 13 registers, so while i<13
save_registers_loop:  
  ldr  r1, [r2], #4     // load saved register from stack
  str  r1, [r0], #4     // save it to the struct
  subs r3, #1           // i--
  bne  save_registers_loop
  // save sp, lr
  stm  r0, {sp, lr}^   // save user mode sp and lr to the struct
  // save pc
  ldr  r1, [r2]         // load saved lr from stack
  sub  r1, #4           // because lr is the old pc+4
  str  r1, [r0, #8]     // save r1 to the pc field in the task_struct
  // save cpsr
  mrs  r1, spsr         // save cpsr of the current task (in spsr atm)
  str  r1, [r0, #12]    // r0 still points to sp, so skip sp, lr, pc (i.e. 12)   

  bx   lr



load_current_task_state:
  ldr  r4, =current_task // load current_task
  ldr  r0, [r4]          // dereference current_task, to get the task_struct
  ldr  r5, [r0, #64]     // load cpsr from task_struct to r5 (i.e. task_struct+64)
  msr  spsr, r5          // copy r5 to spsr
  add  lr, r0, #60       // load address of saved pc into lr
  ldm  r0, {r0-r14}^     // load saved registers into user mode registers ((don't) trust the carrot!)
  ldm  lr, {pc}^         // return to loaded task and restore cpsr from spsr
  

.section .data

message:
  .string "IRQ happened!"

