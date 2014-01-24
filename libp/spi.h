
#pragma once

#define SPI0_OFFSET       (GPIO_PIN(16, 16) + 5)
#define SPI0_MOSI         (SPI0_OFFSET + 0)
#define SPI0_MISO         (SPI0_OFFSET + 1)
#define SPI0_SCL          (SPI0_OFFSET + 2)
#define SPI0_CS           (SPI0_OFFSET + 3)

#define SPI_BASE          ((volatile void*)0x01C41000)

void spi_save(void);

void spi_restore(void);
