
#include <stdio.h>

#include "gpio.h"
#include "gpio_pins.h"

#define SYSCFG_BASE       ((void*volatile)0x01C14000)
#define SYSCFG_KICK(N)    (*((unsigned int*volatile)(SYSCFG_BASE + 0x38 + (N) * 0x4)))
#define SYSCFG_PINMUX(N)  (*((unsigned int*volatile)(SYSCFG_BASE + 0x120 + (N) * 0x4)))

#define KICK0_UNLOCK      0x83E70B13
#define KICK1_UNLOCK      0x95A4F1E0
#define KICK0_LOCK        0x0
#define KICK1_LOCK        0x0

#define SYSCFG_UNLOCK     { SYSCFG_KICK(0) = KICK0_UNLOCK; SYSCFG_KICK(1) = KICK1_UNLOCK; }
#define SYSCFG_LOCK       { SYSCFG_KICK(0) = KICK0_LOCK; SYSCFG_KICK(1) = KICK1_LOCK; }

#define GPIO_BASE         ((void*volatile)0x01E26000)
#define GPIO_BANK(N)      (GPIO_BASE + 0x10 + (N >> 5) * 0x28)
#define GPIO_MASK(N)      (1 << (N & 0x1F))
#define GPIO_DIR(N)       *((unsigned int*)(GPIO_BANK(N) + 0x00))
#define GPIO_SET(N)       *((unsigned int*)(GPIO_BANK(N) + 0x08))
#define GPIO_CLR(N)       *((unsigned int*)(GPIO_BANK(N) + 0x0C))

void
gpio_init_pin (unsigned int pin)
{
  SYSCFG_UNLOCK;

  // setup pin multiplexing
  pin_info pi = pins[pin];
  
  if (__builtin_expect(pi.muxreg_mask == 0, 0))
    printf("gpio: can not initialize pin %x - need init information in pin_info\n", pin);

  SYSCFG_PINMUX(pi.muxreg) &= pi.muxreg_mask;
  SYSCFG_PINMUX(pi.muxreg) |= pi.muxreg_mode;

  // clear pin data and set direction
  GPIO_CLR(pin)  =  GPIO_MASK(pin);
  GPIO_DIR(pin) &= ~GPIO_MASK(pin);

  SYSCFG_LOCK;
}

void
gpio_set (unsigned int pin, unsigned int value)
{
  *((unsigned int*)(GPIO_BANK(pin) + 8 + (!value) * 4)) = GPIO_MASK(pin);
}
