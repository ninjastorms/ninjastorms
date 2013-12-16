
#pragma once

#define SYSCFG_BASE       ((void*volatile)0x01C14000)

#define GPIO_BASE         ((void*volatile)0x01E26000)

#define GPIO_PIN(B,O)     ((B) * 0x10 + (O))

void gpio_init_outpin(unsigned int pin);

void gpio_init_inpin(unsigned int pin);

void gpio_set(unsigned int pin, unsigned int value);

unsigned int gpio_get(unsigned int pin);
