
#pragma once

#define SYSCFG0_BASE      ((volatile void*)0x01C14000)
#define SYSCFG1_BASE      ((volatile void*)0x01E2C000) 

#define GPIO_BASE         ((volatile void*)0x01E26000)

#define GPIO_PIN(B,O)     ((B) * 0x10 + (O))

#define GPIO_BANK(N)      (GPIO_BASE + 0x10 + (N >> 5) * 0x28)
#define GPIO_MASK(N)      (1 << (N & 0x1F))
#define GPIO_DIR(N)       *((volatile unsigned int*)(GPIO_BANK(N) + 0x00))
#define GPIO_SET(N)       *((volatile unsigned int*)(GPIO_BANK(N) + 0x08))
#define GPIO_CLR(N)       *((volatile unsigned int*)(GPIO_BANK(N) + 0x0C))

void gpio_init_pin(unsigned int pin);

void gpio_init_outpin(unsigned int pin);

void gpio_init_inpin(unsigned int pin);

void gpio_set(unsigned int pin, unsigned int value);

unsigned int gpio_get(unsigned int pin);

void spi_init_pin(unsigned int pin);
