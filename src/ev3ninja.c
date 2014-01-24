
#include <stdio.h>

#include "startup.h"
#include "util.h"

#include <libp/led.h>
#include <libp/button.h>

#include <libp/gpio.h>
#include <libp/spi.h>

void rotate_lights(void);
void signal_ready(void);
void wait(void);

int 
ev3ninja_main (__unused int argc, __unused char *argv[])
{
  startup();

  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");

  signal_ready();

  // init input port2 gpio pins
  gpio_init_pin(GPIO_PIN(8, 12)); // Pin 1  - I_ONB           - 9V enable (high)
  gpio_init_pin(GPIO_PIN(8, 15)); // Pin 2  - LEGDETB         - Digital input pulled up
  gpio_init_pin(GPIO_PIN(0, 14)); // Pin 5  - DIGIB0          - Digital input/output
  gpio_init_pin(GPIO_PIN(0, 13)); // Pin 6  - DIGIB1          - Digital input/output
  gpio_init_pin(GPIO_PIN(8, 14)); // Buffer disable

  // init adc spi pins
  gpio_init_pin(SPI0_MOSI); // ADCMOSI
  gpio_init_pin(SPI0_MISO); // ADCMISO
  gpio_init_pin(SPI0_SCL);  // ADCCLK
  gpio_init_pin(SPI0_CS);   // ADCCS

  // init adc power pins
  gpio_init_pin(GPIO_PIN(6, 14)); // 5VONIGEN
  gpio_init_pin(GPIO_PIN(0, 6));  // ADCBATEN

  // disable pull-dpwn
  *((volatile unsigned int*)(SYSCFG1_BASE + 0x0C)) &= ~0xFFFFFFFF;

  // enable battery power on adc
  GPIO_SET(GPIO_PIN(0, 6))  =  GPIO_MASK(GPIO_PIN(0, 6));
  GPIO_DIR(GPIO_PIN(0, 6)) &= ~GPIO_MASK(GPIO_PIN(0, 6));

  spi_save();

  while (1)
    {
      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        rotate_lights();
      wait();
    }

  puts("All done. ev3ninja out!");

  return 0;
}

void 
wait (void)
{
  volatile unsigned int timer = 0;
  while (timer < 1024 * 512) ++timer;
}

void 
rotate_lights (void)
{
  unsigned int state = 0;
  while (1)
    {
      led_set(LED_LEFT, state + 2);
      led_set(LED_RIGHT, state);

      wait();

      state = (state + 1) % 4;

      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        {
          led_set(LED_LEFT, LED_BLACK);
          led_set(LED_RIGHT, LED_BLACK);
          return;
        }
    }
}

void
signal_ready (void)
{
  led_set(LED_LEFT, LED_GREEN);
  led_set(LED_RIGHT, LED_GREEN);

  wait();

  led_set(LED_LEFT, LED_BLACK);
  led_set(LED_RIGHT, LED_BLACK);

  wait();

  led_set(LED_LEFT, LED_GREEN);
  led_set(LED_RIGHT, LED_GREEN);
  
  wait();

  led_set(LED_LEFT, LED_BLACK);
  led_set(LED_RIGHT, LED_BLACK);

  wait();

  led_set(LED_LEFT, LED_GREEN);
  led_set(LED_RIGHT, LED_GREEN);

  wait();
 
  led_set(LED_LEFT, LED_BLACK);
  led_set(LED_RIGHT, LED_BLACK);
}
