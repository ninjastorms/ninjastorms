
#pragma once

#define SYSCFG_BASE       ((volatile void*)0x01C14000)

#define GPIO_BASE         ((volatile void*)0x01E26000)

#define GPIO_PIN(B,O)     ((B) * 0x10 + (O))

#define SPI0_OFFSET       (GPIO_PIN(16, 16))
#define SPI0_MOSI         (SPI0_OFFSET + 0)
#define SPI0_MISO         (SPIO_OFFSET + 1)
#define SPI0_SCL          (SPIO_OFFSET + 2)
#define SPI0_CS           (SPIO_OFFSET + 3)

void gpio_init_pin(unsigned int pin);

void gpio_init_outpin(unsigned int pin);

void gpio_init_inpin(unsigned int pin);

void gpio_set(unsigned int pin, unsigned int value);

unsigned int gpio_get(unsigned int pin);

void spi_init_pin(unsigned int pin);
