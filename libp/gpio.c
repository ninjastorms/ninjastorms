
#include <stdio.h>
#include <stddef.h>

#define GPIO_BASE ((void*volatile)0x01E26000)
#define SYSCFG_BASE ((void*volatile)0x01C14000)
#define NO_OF_LEDS 4

// pin numbers
#define GP2_1   0x21
#define GP5_7   0x57
#define GP6_7   0x67
#define GP6_13  0x6D

struct led_info
{
  int               pin;            // GPIO pin number
  void*             gpio;           // GPIO bank base address
  unsigned int      gpio_mask;      // GPIO pin mask
  unsigned short    muxreg;         // SYSCFG pinmux register
  unsigned int      muxreg_mask;    // SYSCFG pinmux mask
  unsigned int      muxreg_mode;    // SYSCFG pinmux mode
};

typedef struct led_info led_info;

led_info leds[] =
{ // id      register base     1 << (id & 0x1F)   MuxReg    MuxRegMask    MuxRegMode

  { GP6_7 ,  GPIO_BASE + 0x88, 0x00000080,        14,       0xFFFFFFF0,   0x00000008 },
  { GP6_13,  GPIO_BASE + 0x88, 0x00002000,        13,       0xFFFFF0FF,   0x00000800 },
  { GP2_1 ,  GPIO_BASE + 0x38, 0x00000002,         6,       0xF0FFFFFF,   0x08000000 },
  { GP5_7 ,  GPIO_BASE + 0x60, 0x00800000,        12,       0xFFFFFFF0,   0x00000008 },
};

static void
gpio_init (int diode)
{
  volatile void *Reg = SYSCFG_BASE + 0x120 + (leds[diode].muxreg * 4); // pinmux register

  *(unsigned int*)Reg &=  leds[diode].muxreg_mask;
  *(unsigned int*)Reg |=  leds[diode].muxreg_mode;

  *((unsigned int*volatile)(leds[diode].gpio + 0xC))  =  leds[diode].gpio_mask;
  *((unsigned int*volatile)(leds[diode].gpio      )) &= ~leds[diode].gpio_mask;
}

static void
diode_set (int diode, int state)
{
  *((unsigned int*)(leds[diode].gpio + 0x08 + (!state) * 0x04)) = leds[diode].gpio_mask;
}

void
led_test (void)
{
  gpio_init(0);
  gpio_init(1);
  gpio_init(2);
  gpio_init(3);

  diode_set(0, 1); // left green
  diode_set(1, 0);
  diode_set(2, 1); // right green (?)
  diode_set(3, 0);
}
