// ## Memory Layout

// Stacks
#define IRQ_STACK_ADDRESS 0x4000000
#define SVC_STACK_ADDRESS 0x3ff8000

#define TASK_A_STACK_ADDRESS 0x2000000
#define TASK_B_STACK_ADDRESS 0x3000000


// ## Hardware Memory Mappings

//TODO: out back
#define TIMER1_INTCLR_ASM 0x101E200C

#ifdef QEMU

// Timer Adresses
#define TIMER1_BASE 0x101E2000
#define TIMER1_LOAD   (volatile unsigned int*)(TIMER1_BASE+0x0)
#define TIMER1_VALUE  (volatile unsigned int*)(TIMER1_BASE+0x4)
#define TIMER1_CTRL   (volatile char*)(TIMER1_BASE+0x08)
#define TIMER1_INTCLR (volatile char*)(TIMER1_BASE+0x0C)
#define TIMER1_RIS    (volatile char*)(TIMER1_BASE+0x10)
#define TIMER1_MIS    (volatile char*)(TIMER1_BASE+0x14)



// Primary Interrupt Controller (PL190)
#define PIC_BASE 0x10140000
#define PIC_INTENABLE    (volatile char*)(PIC_BASE+0x10)
#define PIC_SOFTINTCLEAR (volatile unsigned int*)(PIC_BASE+0x14)
#define PIC_SOFTINT      (volatile unsigned int*)(PIC_BASE+0x18)
#define PIC_DEFVECTADDR  (volatile unsigned int*)(PIC_BASE+0x34)
#define TIMER1_INTBIT (1 << 4)

#endif

#ifndef QEMU

// Timer Adresses
#define TIMER0_BASE 0x01c20000
#define TIMER0_TIM12      (volatile unsigned int*)(TIMER0_BASE+0x10)
#define TIMER0_TIM34      (volatile unsigned int*)(TIMER0_BASE+0x14)
#define TIMER0_PRD12      (volatile unsigned int*)(TIMER0_BASE+0x18)
#define TIMER0_PRD34      (volatile unsigned int*)(TIMER0_BASE+0x1C)
#define TIMER0_TCR        (volatile unsigned int*)(TIMER0_BASE+0x20)
#define TIMER0_TGCR       (volatile unsigned int*)(TIMER0_BASE+0x24)
#define TIMER0_INTCTLSTAT (volatile unsigned int*)(TIMER0_BASE+0x44)

// TCR bits
#define ENAMODE34 (0b11 << 22)
#define ENAMODE34_CONTIN (0b10 << 22)
#define ENAMODE12 (0b11 << 6)
#define ENAMODE12_CONTIN (0b10 << 6)
#define CLKSRC12 (1 << 8)

// TGRC bits
#define PSC34 (0b1111 << 8)
#define PSC34_VALUE (0b1111 << 8)
#define PLUSEN (1 << 4)
#define TIMMODE (0b11 << 2)
#define TIMMODE_UNCHAINED (0b01 << 2)
#define TIM34RS_REMOVE (1 << 1)
#define TIM12RS_REMOVE (1 << 0)

// INTCTLSTAT bits
#define PRDINTSTAT34 (1 << 17)
#define PRDINTEN34 (1 << 16)
#define PRDINTSTAT12 (1 << 1)
#define PRDINTEN12 (1 << 0)

#endif