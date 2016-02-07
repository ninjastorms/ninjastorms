#include <memory.h>

#include "timer.h"

void start_timer(unsigned int period) {
#ifdef QEMU
  *TIMER1_CTRL &= ~(1 << 7);   // disable timer
  *TIMER1_CTRL |= 1 << 6;      // set periodic-mode
  *TIMER1_INTCLR = (char)0x1;  // clear interrupts
  *TIMER1_CTRL |= 1 << 5;      // set IntEnable
  *TIMER1_CTRL |= 1 << 1;      // set 32-bit mode
  *TIMER1_CTRL &= ~(1 << 0);   // set Wrapping-Mode
  *TIMER1_LOAD  = period;      // set timer period
  *TIMER1_CTRL |= 1 << 7;      // start timer
#endif

#ifndef QEMU
  *TIMER0_TCR  &= ~ENAMODE34;          // disable timer
  *TIMER0_TGCR &= ~TIM34RS_REMOVE;     // reset timer
  *TIMER0_TGCR &= ~TIMMODE;            // reset mode bits
  *TIMER0_TGCR |= TIMMODE_UNCHAINED;   // set dual 32 bit unchained mode
  *TIMER0_TGCR |= TIM34RS_REMOVE;      // remove timer from reset
  *TIMER0_PRD34 = period;              // set timer period
  *TIMER0_TGCR &= ~PSC34;              // reset prescaler
  *TIMER0_TGCR |= PSC34_VALUE;         // set prescaler
  *TIMER0_INTCTLSTAT |= PRDINTSTAT34;  // clear interrupts
  *TIMER0_INTCTLSTAT |= PRDINTEN34;    // enable interrupts
  *TIMER0_TCR  |= ENAMODE34_CONTIN;    // set continuously-mode, start timer
#endif

  return;
}

void stop_timer(void) {
#ifdef QEMU
  *TIMER1_CTRL &= ~(1 << 7);        // disable timer
  *TIMER1_INTCLR = (char)0x1;       // clear interrupts
#endif
#ifndef QEMU
  *TIMER0_TCR  &= ~ENAMODE34;          // disable timer
  *TIMER0_INTCTLSTAT |= PRDINTSTAT34;  // clear interrupts
#endif
}