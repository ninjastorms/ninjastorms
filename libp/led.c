
#include "led.h"
#include "gpio.h"

/* this struct holds the information on the led pins
 * 
 * each LED has two pins
 *   - pin1, for RED on/off
 *   - pin2, for GREEN on/off
 */
struct led_info
{
  unsigned int pin1;
  unsigned int pin2;
};
typedef struct led_info led_info;

led_info leds[] = 
{
  { GPIO_PIN(6, 13), GPIO_PIN(6,  7) }, // LED_LEFT
  { GPIO_PIN(6, 12), GPIO_PIN(6, 14) }  // LED_RIGHT
};


void 
led_set (led_id led, led_color color)
{
  if (led & LED_LEFT)
    {
      gpio_set(leds[0].pin1, color & 1);
      gpio_set(leds[0].pin2, (color & 2) >> 1);
    }
  if (led & LED_RIGHT)
    {
      gpio_set(leds[1].pin1, color & 1);
      gpio_set(leds[1].pin2, (color & 2) >> 1);
    }
}


/* initialize the gpio pins necessary for led functions
 * this is done automatically on startup
 */
void
__attribute((constructor)) // <- does not work yet
led_init (void)
{
  gpio_init_outpin(leds[0].pin1);
  gpio_init_outpin(leds[0].pin2);
  gpio_init_outpin(leds[1].pin1);
  gpio_init_outpin(leds[1].pin2);
}


