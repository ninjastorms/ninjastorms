
#include "spi.h"
#include "pininfo.h"


#define   SPI0_CLOCK 150000000UL
#define   ADC_TIME           8UL // µS

#define   ADC_CLOCK  ((1000000UL * 16UL) / ADC_TIME)
#define   CNVSPD     ((SPI0_CLOCK / ADC_CLOCK) - 1)



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

#define SPITxFULL     (SPIBUF & 0x20000000)
#define SPIRxEMPTY    (SPIBUF & 0x80000000)

unsigned int save_GCR0  = 0;
unsigned int save_GCR1  = 0;
unsigned int save_PC0   = 0;
unsigned int save_DAT1  = 0;
unsigned int save_FMT0  = 0;
unsigned int save_DELAY = 0;
unsigned int save_DEF   = 0;

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
  SPIGCR0  = save_GCR0;
  SPIGCR1  = save_GCR1;
  SPIPC0   = save_PC0;
  SPIDAT1  = save_DAT1;
  SPIFMT0  = save_FMT0;
  SPIDELAY = save_DELAY;
  SPIDEF   = save_DEF;
}


#include <stdio.h>

void
spi_init (void)
{
  SPIGCR0  = 0x00000001; // enable
  SPIGCR1  = 0x00000003; // Master enable
  SPIPC0   = 0x00000E08;
  SPIDAT1  = 0x00000000; // Format 0 is selected
  SPIFMT0  = 0x00010010 | ((CNVSPD << 8) & 0xFF00);
  SPIDELAY = 0x0A0A0A0A; // Delays = 10
  SPIINT0  = 0x00000000; // Interrupts disabled
  SPIDEF   = 0x00000008;
  SPIGCR1  = 0x01000003; // Enable bit
}

unsigned short
spi_update (unsigned short data)
{
  while (SPITxFULL);

  SPIDAT0 = (unsigned long)data;

  while (SPIRxEMPTY);

  return ((unsigned short)(SPIBUF & 0x0000FFFF));
}
