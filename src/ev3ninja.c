
#include <stdio.h>

#include "feedback.h"

#include <libp/led.h>
#include <libp/button.h>

#include <libp/gpio.h>
#include <libp/spi.h>


/* TODO:: get __attribute__((constructor)), __attribute__((destructor)) working and remove manual init/fini code */
extern void bss_init(void);
extern void led_init(void);
extern void button_init(void);

static void
ev3ninja_init (void)
{
  bss_init();
  led_init();
  button_init();
}

int 
ev3ninja_main (void)
{
  ev3ninja_init();

  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");

  feedback_flash_green();

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

  spi_init();

  puts("  device initialized (pt1)");

  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);
  spi_update(0x400F);

  puts("  device initialized (pt2)");
  
  unsigned char Input[16] = { 6,8,10,12,5,7,9,11,1,0,13,14,2,15,3,4 };

  unsigned int hit = 0;
  unsigned int total = 0;

  unsigned int output_type = 0;

  unsigned int i = 5;
  while (1)
    {
      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        {
          hit = 0;
          total = 0;
        }

      if (button_get_state(BUTTON_LEFT) == BUTTON_DOWN)
        {
          i = (i + 15) % 16;
          while (button_get_state(BUTTON_LEFT) == BUTTON_DOWN);
        }

      if (button_get_state(BUTTON_RIGHT) == BUTTON_DOWN)
        {
          i = (i + 1) % 16;
          while (button_get_state(BUTTON_RIGHT) == BUTTON_DOWN);
        }

      if (button_get_state(BUTTON_TOP) == BUTTON_DOWN)
        {
          output_type = (output_type + 1) % 2;
          while (button_get_state(BUTTON_TOP) == BUTTON_DOWN);
        }

      if (button_get_state(BUTTON_BOTTOM) == BUTTON_DOWN)
        {
          output_type = (output_type + 1) % 2;
          while (button_get_state(BUTTON_BOTTOM) == BUTTON_DOWN);
        }

      unsigned short Data = (spi_update((0x1840 | ((Input[i] & 0x000F) << 7)))) & 0x0FFF;

      switch (output_type)
      {
      case 0:
        printf("%i(%i) - %i                   \r", i, Input[i], Data);
        break;
      case 1:
        if (Data > 2000)
          ++hit;
        ++total;
        printf("%i(%i) - %i / %i              \r", i, Input[i], hit, total);
        break;
      }
    }

  printf("\n");

  puts("All done. ev3ninja out!");
  
  feedback_flash_red();

  return 0;
}

/* bss init */
extern unsigned long __bss_start, _end;

void
__attribute__((constructor)) // <- does not work yet
bss_init (void)
{
  /* Zero out BSS */
  unsigned char * cp = (unsigned char *) &__bss_start;
  while (cp < (unsigned char *)&_end)
    *cp++ = 0;
}

