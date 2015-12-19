
.section .text
.globl puts
.globl irq_handler
.type irq_handler STT_FUNC

irq_handler:
  //for less latency save only registers that will change
  push {r0-r12, lr}

  ldr r0, =message
  bl puts

  //clear interrupt
  ldr r0, =TIMER1_INTCLR
  mov r1, #0x1
  str r1, [r0]

  pop {r0-r12, lr}
  subs pc, lr, #4	//return from IRQ

.section .data

TIMER1_INTCLR = 0x101E200C
message:
  .string "IRQ happened!"

