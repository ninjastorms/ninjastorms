
#include "adc.h"

#include "gpio.h"
#include "spi.h"


unsigned short
adc_get (unsigned short channel)
{
  return (spi_update((0x1840 | ((channel & 0x000F) << 7)))) & 0x0FFF;
}


/* initialize the state of the adc
 */
static void
__attribute__((constructor (1002))) // <- does not work yet
adc_init (void)
{
  // init adc power pins
  gpio_init_pin(GPIO_PIN(6, 14)); // 5VONIGEN
  gpio_init_pin(GPIO_PIN(0, 6));  // ADCBATEN

  // enable battery power on adc
  GPIO_SET(GPIO_PIN(0, 6))  =  GPIO_MASK(GPIO_PIN(0, 6));
  GPIO_DIR(GPIO_PIN(0, 6)) &= ~GPIO_MASK(GPIO_PIN(0, 6));

  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
}


