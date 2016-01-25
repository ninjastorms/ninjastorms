// ## Memory Layout

// Stacks
#define IRQ_STACK_ADDRESS 0x4000000
#define SVC_STACK_ADDRESS 0x3ff8000

#define TASK_A_STACK_ADDRESS 0x2000000
#define TASK_B_STACK_ADDRESS 0x3000000


// ## Hardware Memory Mappings

#ifdef QEMU

// Timer Adresses
#define TIMER1_BASE 0x101E2000
#define TIMER1_INTCLR_ASM 0x101E200C
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
#define TIMER0_INTCTLSTAT_ASM 0x101E200C
#define TIMER0_TIM12      (volatile unsigned int*)(TIMER0_BASE+0x10)
#define TIMER0_TIM34      (volatile unsigned int*)(TIMER0_BASE+0x14)
#define TIMER0_PRD12      (volatile unsigned int*)(TIMER0_BASE+0x18)
#define TIMER0_PRD34      (volatile unsigned int*)(TIMER0_BASE+0x1C)
#define TIMER0_TCR        (volatile unsigned int*)(TIMER0_BASE+0x20)
#define TIMER0_TGCR       (volatile unsigned int*)(TIMER0_BASE+0x24)
#define TIMER0_INTCTLSTAT (volatile unsigned int*)(TIMER0_BASE+0x44)

// TCR bits
#define ENAMODE34        (0b11 << 22)
#define ENAMODE34_CONTIN (0b10 << 22)
#define ENAMODE12        (0b11 << 6)
#define ENAMODE12_CONTIN (0b10 << 6)
#define CLKSRC12 (1 << 8)

// TGRC bits
#define PSC34             (0b1111 << 8)
#define PSC34_VALUE       (0b1111 << 8)
#define TIMMODE           (0b11 << 2)
#define TIMMODE_UNCHAINED (0b01 << 2)
#define PLUSEN         (1 << 4)
#define TIM34RS_REMOVE (1 << 1)
#define TIM12RS_REMOVE (1 << 0)

// INTCTLSTAT bits
#define PRDINTSTAT34_ASM #0x20000
#define PRDINTSTAT34 (1 << 17)
#define PRDINTEN34   (1 << 16)
#define PRDINTSTAT12 (1 <<  1)
#define PRDINTEN12   (1 <<  0)

// ARM interrupt controller (AINTC)
#define AINTC_BASE 0xFFFEE000
#define AINTC_GER   (volatile unsigned int*)(AINTC_BASE+0x0010)
#define AINTC_SECR1 (volatile unsigned int*)(AINTC_BASE+0x0280)
#define AINTC_SECR2 (volatile unsigned int*)(AINTC_BASE+0x0284)
#define AINTC_SECR3 (volatile unsigned int*)(AINTC_BASE+0x0288)
#define AINTC_SECR4 (volatile unsigned int*)(AINTC_BASE+0x038C)
#define AINTC_ESR1  (volatile unsigned int*)(AINTC_BASE+0x0300)
#define AINTC_ESR2  (volatile unsigned int*)(AINTC_BASE+0x0304)
#define AINTC_ESR3  (volatile unsigned int*)(AINTC_BASE+0x0308)
#define AINTC_ESR4  (volatile unsigned int*)(AINTC_BASE+0x030C)
#define AINTC_CMR0  (volatile unsigned int*)(AINTC_BASE+0x0400)
#define AINTC_CMR5  (volatile unsigned int*)(AINTC_BASE+0x0414)
#define AINTC_HIER  (volatile unsigned int*)(AINTC_BASE+0x1500)

// AINTEC bits
#define GER_ENABLE 1
#define T64P0_TINT34 (1 << 22)
#define HIER_IRQ (1 << 1)


#endif