#include <memory.h>

.section .text
.globl puts
.globl irq_handler
.type irq_handler STT_FUNC

.global current_task

irq_handler:
  // for less latency save only registers that will change
  push  {r0-r12, lr}

  //ldr  r0, =message
  bl  schedule

  // clear interrupt
  ldr  r0, =TIMER1_INTCLR_ASM
  mov  r1, #0x1
  str  r1, [r0]

  pop  {r0-r12, lr}

  //return from IRQ
  subs  pc, lr, #4	

schedule:
  ldr  r0, task_a_addr  // load task_a's address (TODO: dynamic!)
  mov  r2, sp           // save sp to iterate over it
  mov  r3, #13          // iteration count; r0-r12 is 13 registers, so while i<13
save_registers_loop:  
  ldr  r1, [r2], #4     // load saved register from stack
  str  r1, [r0], #4     // save it to the struct
  subs r3, #1           // i--
  bne  save_registers_loop
  ldr  r1, [r2]         // load saved lr from stack
  sub  r1, #4           // because lr is the old pc+4
  str  r1, [r0+8]       // we skip sp and lr in the struct and save it
  bx   lr
  //TODO: save cpsr
  //TODO: change to saved mode
  //TODO: save sp and lr

task_a_addr: 
  .word 0x111310

.section .data

message:
  .string "IRQ happened!"

