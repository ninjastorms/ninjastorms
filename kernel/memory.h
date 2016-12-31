
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

#pragma once

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

// ## Memory Layout

// Stacks
#define STACK_SIZE 0x10000

#if BOARD_VERSATILEPB
#  define IRQ_STACK_ADDRESS 0x4000000
#endif

#if BOARD_EV3
#  define IRQ_STACK_ADDRESS 0xC5000000
#endif

#define SVC_STACK_ADDRESS (IRQ_STACK_ADDRESS - STACK_SIZE)
#define TASK_STACK_BASE_ADDRESS (SVC_STACK_ADDRESS - STACK_SIZE)

// ## Hardware Memory Mappings

#if BOARD_VERSATILEPB

// Timer Adresses
#  define TIMER1_BASE 0x101E2000
#  define TIMER1_INTCLR_ASM 0x101E200C
#  define TIMER1_LOAD   (volatile unsigned int*)(TIMER1_BASE+0x0)
#  define TIMER1_VALUE  (volatile unsigned int*)(TIMER1_BASE+0x4)
#  define TIMER1_CTRL   (volatile char*)(TIMER1_BASE+0x08)
#  define TIMER1_INTCLR (volatile char*)(TIMER1_BASE+0x0C)
#  define TIMER1_RIS    (volatile char*)(TIMER1_BASE+0x10)
#  define TIMER1_MIS    (volatile char*)(TIMER1_BASE+0x14)



// Primary Interrupt Controller (PL190)
#  define PIC_BASE 0x10140000
#  define PIC_INTENABLE    (volatile char*)(PIC_BASE+0x10)
#  define PIC_SOFTINTCLEAR (volatile unsigned int*)(PIC_BASE+0x14)
#  define PIC_SOFTINT      (volatile unsigned int*)(PIC_BASE+0x18)
#  define PIC_DEFVECTADDR  (volatile unsigned int*)(PIC_BASE+0x34)
#  define TIMER1_INTBIT (1 << 4)

#endif

#if BOARD_EV3

// Timer Adresses
#  define TIMER0_BASE 0x01C20000
#  define TIMER0_INTCTLSTAT_ASM 0x01C20044
#  define TIMER0_TIM12      (volatile unsigned int*)(TIMER0_BASE+0x10)
#  define TIMER0_TIM34      (volatile unsigned int*)(TIMER0_BASE+0x14)
#  define TIMER0_PRD12      (volatile unsigned int*)(TIMER0_BASE+0x18)
#  define TIMER0_PRD34      (volatile unsigned int*)(TIMER0_BASE+0x1C)
#  define TIMER0_TCR        (volatile unsigned int*)(TIMER0_BASE+0x20)
#  define TIMER0_TGCR       (volatile unsigned int*)(TIMER0_BASE+0x24)
#  define TIMER0_INTCTLSTAT (volatile unsigned int*)(TIMER0_BASE+0x44)

// TCR bits
#  define ENAMODE34        (0b11 << 22)
#  define ENAMODE34_CONTIN (0b10 << 22)
#  define ENAMODE12        (0b11 << 6)
#  define ENAMODE12_CONTIN (0b10 << 6)
#  define CLKSRC12 (1 << 8)

// TGRC bits
#  define PSC34             (0b1111 << 8)
#  define PSC34_VALUE       (0b1111 << 8)
#  define TIMMODE           (0b11 << 2)
#  define TIMMODE_UNCHAINED (0b01 << 2)
#  define PLUSEN         (1 << 4)
#  define TIM34RS_REMOVE (1 << 1)
#  define TIM12RS_REMOVE (1 << 0)

// INTCTLSTAT bits
#  define CLEARTIMER34_ASM #0x30000
#  define PRDINTSTAT34 (1 << 17)
#  define PRDINTEN34   (1 << 16)
#  define PRDINTSTAT12 (1 <<  1)
#  define PRDINTEN12   (1 <<  0)

// ARM interrupt controller (AINTC)
#  define AINTC_BASE      0xFFFEE000
#  define AINTC_SECR1_ASM 0xFFFEE280
#  define AINTC_GER    (volatile unsigned int*)(AINTC_BASE+0x0010)
#  define AINTC_SECR1  (volatile unsigned int*)(AINTC_BASE+0x0280)
#  define AINTC_SECR2  (volatile unsigned int*)(AINTC_BASE+0x0284)
#  define AINTC_SECR3  (volatile unsigned int*)(AINTC_BASE+0x0288)
#  define AINTC_SECR4  (volatile unsigned int*)(AINTC_BASE+0x038C)
#  define AINTC_ESR1   (volatile unsigned int*)(AINTC_BASE+0x0300)
#  define AINTC_ESR2   (volatile unsigned int*)(AINTC_BASE+0x0304)
#  define AINTC_ESR3   (volatile unsigned int*)(AINTC_BASE+0x0308)
#  define AINTC_ESR4   (volatile unsigned int*)(AINTC_BASE+0x030C)
#  define AINTC_CMR0   (volatile unsigned int*)(AINTC_BASE+0x0400)
#  define AINTC_CMR5   (volatile unsigned int*)(AINTC_BASE+0x0414)
#  define AINTC_HIPIR2 (volatile unsigned int*)(AINTC_BASE+0x0904)
#  define AINTC_HIER   (volatile unsigned int*)(AINTC_BASE+0x1500)

// AINTEC bits
#  define GER_ENABLE 1
#  define T64P0_TINT34 (1 << 22)
#  define T64P0_TINT34_ASM #0x400000
#  define HIER_IRQ (1 << 1)

#endif
