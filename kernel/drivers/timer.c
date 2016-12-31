
/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#include "timer.h"

#include "kernel/memory.h"

void
timer_start (unsigned int period)
{
#if BOARD_VERSATILEPB
  *TIMER1_CTRL &= ~(1 << 7);   // disable timer
  *TIMER1_CTRL |= 1 << 6;      // set periodic-mode
  *TIMER1_INTCLR = (char)0x1;  // clear interrupts
  *TIMER1_CTRL |= 1 << 5;      // set IntEnable
  *TIMER1_CTRL |= 1 << 1;      // set 32-bit mode
  *TIMER1_CTRL &= ~(1 << 0);   // set Wrapping-Mode
  *TIMER1_LOAD  = period;      // set timer period
  *TIMER1_CTRL |= 1 << 7;      // start timer
#endif

#if BOARD_EV3
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
}

void
timer_stop(void)
{
#if BOARD_VERSATILEPB
  *TIMER1_CTRL &= ~(1 << 7);        // disable timer
  *TIMER1_INTCLR = (char)0x1;       // clear interrupts
#endif

#if BOARD_EV3
  *TIMER0_TCR  &= ~ENAMODE34;          // disable timer
  *TIMER0_INTCTLSTAT |= PRDINTSTAT34;  // clear interrupts
#endif
}
