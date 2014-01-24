
#include "spi.h"
#include "pininfo.h"

/* SPI Register numbers */
#define   SPIGCR0     (*((volatile unsigned int*)(SPI_BASE + 0x00)))
#define   SPIGCR1     (*((volatile unsigned int*)(SPI_BASE + 0x04)))
#define   SPIINT0     (*((volatile unsigned int*)(SPI_BASE + 0x08)))
#define   SPILVL      (*((volatile unsigned int*)(SPI_BASE + 0x0C)))
#define   SPIFLG      (*((volatile unsigned int*)(SPI_BASE + 0x10)))
#define   SPIPC0      (*((volatile unsigned int*)(SPI_BASE + 0x14)))
#define   SPIPC1      (*((volatile unsigned int*)(SPI_BASE + 0x18)))
#define   SPIPC2      (*((volatile unsigned int*)(SPI_BASE + 0x1C)))
#define   SPIPC3      (*((volatile unsigned int*)(SPI_BASE + 0x20)))
#define   SPIPC4      (*((volatile unsigned int*)(SPI_BASE + 0x24)))
#define   SPIPC5      (*((volatile unsigned int*)(SPI_BASE + 0x28)))

#define   SPIDAT0     (*((volatile unsigned int*)(SPI_BASE + 0x38)))
#define   SPIDAT1     (*((volatile unsigned int*)(SPI_BASE + 0x3C)))
#define   SPIBUF      (*((volatile unsigned int*)(SPI_BASE + 0x40)))
#define   SPIEMU      (*((volatile unsigned int*)(SPI_BASE + 0x44)))
#define   SPIDELAY    (*((volatile unsigned int*)(SPI_BASE + 0x48)))
#define   SPIDEF      (*((volatile unsigned int*)(SPI_BASE + 0x4C)))
#define   SPIFMT0     (*((volatile unsigned int*)(SPI_BASE + 0x50)))
#define   SPIFMT1     (*((volatile unsigned int*)(SPI_BASE + 0x54)))
#define   SPIFMT2     (*((volatile unsigned int*)(SPI_BASE + 0x58)))
#define   SPIFMT3     (*((volatile unsigned int*)(SPI_BASE + 0x5C)))
#define   INTVEC0     (*((volatile unsigned int*)(SPI_BASE + 0x60)))
#define   INTVEC1     (*((volatile unsigned int*)(SPI_BASE + 0x64)))


unsigned int save_GCR0;
unsigned int save_GCR1;
unsigned int save_PC0;
unsigned int save_DAT1;
unsigned int save_FMT0;
unsigned int save_DELAY;
unsigned int save_DEF;

void
spi_save (void)
{
  save_GCR0  = SPIGCR0;
  save_GCR1  = SPIGCR1;
  save_PC0   = SPIPC0;
  save_DAT1  = SPIDAT1;
  save_FMT0  = SPIFMT0;
  save_DELAY = SPIDELAY;
  save_DEF   = SPIDEF;
}

void
spi_restore (void)
{
  
}
