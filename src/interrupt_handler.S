#include <memory.h>

.section .text
.globl puts
.globl irq_handler
.type irq_handler STT_FUNC

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


.section .data

message:
  .string "IRQ happened!"

