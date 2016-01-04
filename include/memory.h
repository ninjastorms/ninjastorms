// ## Memory Layout

// Stacks
#define IRQ_STACK_ADDRESS 0x4000000
#define SVC_STACK_ADDRESS 0x3ff8000

#define TASK_A_STACK_ADDRESS 0x2000000
#define TASK_B_STACK_ADDRESS 0x3000000


// ## Hardware Memory Mappings

// Timer Adresses
#define TIMER1_BASE 0x101E2000
#define TIMER1_LOAD (volatile unsigned int*)(TIMER1_BASE+0x0)
#define TIMER1_VALUE (volatile unsigned int*)(TIMER1_BASE+0x4)
#define TIMER1_CTRL (volatile char*)(TIMER1_BASE+0x8)
#define TIMER1_INTCLR (volatile char*)(TIMER1_BASE+0xC)
#define TIMER1_INTCLR_ASM 0x101E200C
#define TIMER1_RIS (volatile char*)(TIMER1_BASE+0x10)
#define TIMER1_MIS (volatile char*)(TIMER1_BASE+0x14)

// Primary Interrupt Controller (PL190)
#define PIC_BASE 0x10140000
#define PIC_INTENABLE (volatile char*)(PIC_BASE+0x10)
#define PIC_SOFTINTCLEAR (volatile unsigned int*)(PIC_BASE+0x14)
#define PIC_SOFTINT (volatile unsigned int*)(PIC_BASE+0x18)
#define PIC_DEFVECTADDR (volatile unsigned int*)(PIC_BASE+0x34)
#define TIMER1_INTBIT (1 << 4)
