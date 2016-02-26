
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

#include "gpio.h"

#include "kernel/drivers/pininfo.h"

#include <stdio.h>

#define SYSCFG_KICK(N)    (*((volatile unsigned int*)(SYSCFG0_BASE + 0x38 + (N) * 0x4)))
#define SYSCFG_PINMUX(N)  (*((volatile unsigned int*)(SYSCFG0_BASE + 0x120 + (N) * 0x4)))

#define KICK0_UNLOCK      0x83E70B13
#define KICK1_UNLOCK      0x95A4F1E0
#define KICK0_LOCK        0x0
#define KICK1_LOCK        0x0

#define SYSCFG_UNLOCK     { SYSCFG_KICK(0) = KICK0_UNLOCK; SYSCFG_KICK(1) = KICK1_UNLOCK; }
#define SYSCFG_LOCK       { SYSCFG_KICK(0) = KICK0_LOCK; SYSCFG_KICK(1) = KICK1_LOCK; }

#define GPIO_BANK(N)      (GPIO_BASE + 0x10 + (N >> 5) * 0x28)
#define GPIO_MASK(N)      (1 << (N & 0x1F))
#define GPIO_DIR(N)       *((volatile unsigned int*)(GPIO_BANK(N) + 0x00))
#define GPIO_SET(N)       *((volatile unsigned int*)(GPIO_BANK(N) + 0x08))
#define GPIO_CLR(N)       *((volatile unsigned int*)(GPIO_BANK(N) + 0x0C))

void
gpio_init_pin (unsigned int pin)
{
  SYSCFG_UNLOCK;

  // setup pin multiplexing
  pin_info pi = pininfo[pin];

  if (__builtin_expect(pin >= pininfo_size || pi.muxreg_mask == 0, 0))
    printf("gpio: can not initialize pin %x - need init information in pin_info\n", pin);

  SYSCFG_PINMUX(pi.muxreg) &= pi.muxreg_mask;
  SYSCFG_PINMUX(pi.muxreg) |= pi.muxreg_mode;

  SYSCFG_LOCK;
}

void
gpio_init_outpin (unsigned int pin)
{
  SYSCFG_UNLOCK;

  gpio_init_pin(pin);

  // clear pin data and set direction
  GPIO_CLR(pin)  =  GPIO_MASK(pin);
  GPIO_DIR(pin) &= ~GPIO_MASK(pin);

  SYSCFG_LOCK;
}

void
gpio_init_inpin (unsigned int pin)
{
  SYSCFG_UNLOCK;

  gpio_init_pin(pin);

  // set direction
  GPIO_DIR(pin) |=  GPIO_MASK(pin);

  SYSCFG_LOCK;
}

void
gpio_set (unsigned int pin, unsigned int value)
{
  gpio_init_outpin(pin);
  *((volatile unsigned int*)(GPIO_BANK(pin) + 8 + (!value) * 4)) = GPIO_MASK(pin);
}

unsigned int
gpio_get (unsigned int pin)
{
  gpio_init_inpin(pin);
  volatile unsigned int *Reg = (GPIO_BANK(pin) + 0x10);
  return (((*Reg) & GPIO_MASK(pin)) != 0);
}
