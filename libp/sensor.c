
#include "sensor.h"

#include "gpio.h"

void
__attribute__((constructor)) // <- does not work yet
sensor_init (void)
{
  gpio_init_pin(GPIO_PIN(8, 12)); // Pin 1  - I_ONB           - 9V enable (high)
  gpio_init_pin(GPIO_PIN(8, 15)); // Pin 2  - LEGDETB         - Digital input pulled up
  gpio_init_pin(GPIO_PIN(0, 14)); // Pin 5  - DIGIB0          - Digital input/output
  gpio_init_pin(GPIO_PIN(0, 13)); // Pin 6  - DIGIB1          - Digital input/output
  gpio_init_pin(GPIO_PIN(8, 14)); // Buffer disable

  // disable pull-dpwn
  *((volatile unsigned int*)(SYSCFG1_BASE + 0x0C)) &= ~0xFFFFFFFF;
}
