
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

#include "sensor.h"

#include "kernel/drivers/gpio.h"
#include "kernel/drivers/adc.h"

struct sensor_port_info
{
  unsigned int pin1;      // I_ONB           - 9V enable (high)
  unsigned int pin2;      // LEGDETB         - Digital input pulled up
  unsigned int pin5;      // DIGIB0          - Digital input/output
  unsigned int pin6;      // DIGIB1          - Digital input/output
  unsigned int buffer;    // Buffer disable
  unsigned char adc1;  // adc channel 1
  unsigned char adc2;  // adc channel 2
};
typedef struct sensor_port_info sensor_port_info;

static sensor_port_info ports[] =
{
  { GPIO_PIN(8, 10), GPIO_PIN(2,  2), GPIO_PIN(0,  2), GPIO_PIN(0, 15), GPIO_PIN(8, 11), 0x6, 0x5 },
  { GPIO_PIN(8, 12), GPIO_PIN(8, 15), GPIO_PIN(0, 14), GPIO_PIN(0, 13), GPIO_PIN(8, 14), 0x8, 0x7 },
  { GPIO_PIN(8,  9), GPIO_PIN(7, 11), GPIO_PIN(0, 12), GPIO_PIN(1, 14), GPIO_PIN(7,  9), 0xA, 0x9 },
  { GPIO_PIN(6,  4), GPIO_PIN(7,  8), GPIO_PIN(0,  1), GPIO_PIN(1, 15), GPIO_PIN(7, 10), 0xC, 0xB },
};

sensor_touch_state
sensor_touch_get_state (sensor_port_id port)
{
  unsigned short Data1 = adc_get(ports[port].adc1);
  unsigned short Data2 = adc_get(ports[port].adc2);

  return ((Data2 > 3000) || (Data1 > 500 && Data1 < 1000) ? SENSOR_TOUCH_DOWN : SENSOR_TOUCH_UP);
}

unsigned short
sensor_light_get (sensor_port_id port)
{
  // turn the light on
  gpio_set(ports[port].pin5, 1);

  return adc_get(ports[port].adc1);
}

/* initialize the gpio pins necessary for sensor functions
 * this is done automatically on startup
 */
static void
__attribute__((constructor))
sensor_init (void)
{
  unsigned int i;
  for (i = 0; i < sizeof(ports) / sizeof(ports[0]); ++i)
    {
      gpio_init_inpin(ports[i].pin1);
      gpio_init_inpin(ports[i].pin2);
      gpio_init_outpin(ports[i].pin5);
      gpio_init_pin(ports[i].pin6);
      gpio_init_pin(ports[i].buffer);
    }

  // disable pull-dpwn
  *((volatile unsigned int*)(SYSCFG1_BASE + 0x0C)) &= ~0xFFFFFFFF;
}
