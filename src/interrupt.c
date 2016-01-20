#include "interrupt.h"
#include "interrupt_handler.h"

#include <memory.h>

void init_interrupt_handling(void) {
  //build interrupt vector table
  *(unsigned int*) 0x00 = 0xe59ff014;	//TODO: reset
  *(unsigned int*) 0x04 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x20 undefined instruction
  *(unsigned int*) 0x08 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x24 software interrupt
  *(unsigned int*) 0x0c = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x28 prefetch abort
  *(unsigned int*) 0x10 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x2c data abort
  *(unsigned int*) 0x14 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x30 reserved
  *(unsigned int*) 0x18 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x34 IRQ
  *(unsigned int*) 0x1c = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x38 FIQ

  *(unsigned int*) 0x1c = 0x1000000; //TODO
  *(unsigned int*) 0x20 = 0x1000000; //TODO
  //ATTENTION: don't use software interrupts in supervisor mode
  *(unsigned int*) 0x24 = 0x1000000; //(unsigned int) &swi_handler;
  *(unsigned int*) 0x28 = 0x1000000; //TODO
  *(unsigned int*) 0x2c = 0x1000000; //TODO
  *(unsigned int*) 0x30 = 0x1000000; //TODO
  *(unsigned int*) 0x34 = (unsigned int) &irq_handler;
  *(unsigned int*) 0x38 = 0x1000000; //TODO

  //setting up IRQ mode stack
  asm(
    "mrs  r0, cpsr\n"
    "bic  r0, #0x1f\n" // Clear mode bits
    "orr  r0, #0x12\n" // Select IRQ mode
    "msr  cpsr, r0\n"  // Enter IRQ mode
    "mov  sp, %0\n"    // set stack pointer
    "bic  r0, #0x1f\n" // Clear mode bits
    "orr  r0, #0x13\n" // Select SVC mode
    "msr  cpsr, r0\n"  // Enter SVC mode
    : : "r" (IRQ_STACK_ADDRESS)
  );

  // Enable IRQs
  asm(
    "mrs  r1, cpsr\n"
    "bic  r1, r1, #0x80\n"
    "msr  cpsr_c, r1\n"
  );

  // Setting up primary interrupt controller
  //*PIC_INTENABLE |= TIMER1_INTBIT;	// unmask interrupt bit for timer1
  return;
}

