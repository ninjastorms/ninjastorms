
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

#include "adc.h"

#include "kernel/drivers/gpio.h"
#include "kernel/drivers/spi.h"

unsigned short
adc_get (unsigned short channel)
{
  return (spi_update((0x1840 | ((channel & 0x000F) << 7)))) & 0x0FFF;
}

/* initialize the state of the adc
 */
static void
__attribute__((constructor (1002)))
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


