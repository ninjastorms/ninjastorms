
#include "led.h"
#include "gpio.h"

struct led_info
{
  unsigned int pin1;
  unsigned int pin2;
};

typedef struct led_info led_info;

led_info leds[] = 
{
  { GPIO_PIN(6, 13), GPIO_PIN(6,  7) },
  { GPIO_PIN(6, 12), GPIO_PIN(6, 14) }
};


void
led_init (void)
{
  gpio_init_outpin(leds[0].pin1);
  gpio_init_outpin(leds[0].pin2);
  gpio_init_outpin(leds[1].pin1);
  gpio_init_outpin(leds[1].pin2);
}

void 
led_set (led_id led, led_color color)
{
  gpio_set(leds[led].pin1, color & 1);
  gpio_set(leds[led].pin2, (color & 2) >> 1);
}
