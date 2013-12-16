
#pragma once

#define GPIO_PIN(B,O) ((B) * 0x10 + (O))

void gpio_init_pin(unsigned int pin);

void gpio_set(unsigned int pin, unsigned int value);
