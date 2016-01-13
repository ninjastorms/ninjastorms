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


//.extern current_task
//.extern other_task



irq_handler:
  // for less latency save only registers that will change
  push  {r0-r12, lr}

  //ldr  r0, =message
  bl  save_current_task_state
  bl  schedule

  // clear interrupt
  ldr  r0, =TIMER1_INTCLR_ASM
  mov  r1, #0x1
  str  r1, [r0]
  
  ldr  r0, =message
  bl   puts

  pop  {r0-r12, lr} //TODO: do we need this?

  bl  load_current_task_state

  //return from IRQ
  subs  pc, lr, #4 //TODO: do we need this?	



save_current_task_state:
  ldr  r4, =current_task // load current_task
  ldr  r0, [r4]         // dereference current_task, to get the task_struct
  mov  r2, sp           // save sp to iterate over it
  mov  r3, #13          // iteration count; r0-r12 is 13 registers, so while i<13
save_registers_loop:  
  ldr  r1, [r2], #4     // load saved register from stack
  str  r1, [r0], #4     // save it to the struct
  subs r3, #1           // i--
  bne  save_registers_loop
  ldr  r1, [r2]         // load saved lr from stack
  sub  r1, #4           // because lr is the old pc+4
  str  r1, [r0, #8]     // we skip sp and lr in the struct and save it
  mrs  r2, spsr         // save cpsr of the current task (i.e spsr atm)
  str  r2, [r0, #12]    // r0 still points to sp, so skip sp, lr, pc (i.e. 12) 

  // change to back SVC mode to save sp and lr
  mrs  r1, cpsr         // save cpsr to r1
  bic  r1, #0x1f        // Clear mode bits
  orr  r1, #0x13        // Select SVC mode
  msr  cpsr, r1         // Enter new mode
  stm  r0, { sp, lr }   // save sp and lr to the struct
  bic  r1, #0x1f        // Clear mode bits
  orr  r1, #0x12        // Select IRQ mode
  msr  cpsr, r1         // Enter new mode 
  bx   lr



load_current_task_state:
  ldr  r4, =current_task // load current_task
  ldr  r0, [r4]          // dereference current_task, to get the task_struct
  ldr  r5, [r0, #64]     // load cpsr from task_struct to r5 (i.e. task_struct+64)
  msr  spsr, r5          // copy r5 to spsr
  ldm  r0, {r0-r15}^     // load r0-r15 from task_struct and restore cpsr from spsr
  // [!] ^ forces the use of r14,r15 from SVC mode instead of the current IRQ mode
  // and restores cpsr form spsr (see ARM manual for LDM/STM)

  

.section .data

message:
  .string "IRQ happened!"

