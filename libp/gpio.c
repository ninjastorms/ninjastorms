
#include <stddef.h>

#define GPIO_BASE ((void*)0x01E26000)
#define SYSCFG_BASE ((void*)0x01C14000)
#define NO_OF_LEDS 4

#define   DIODEInit(D) \
  {\
    *((unsigned int*)(UiLedPin[D].pGpio + 0xC))  =  UiLedPin[D].Mask;\
    *((unsigned int*)(UiLedPin[D].pGpio      )) &= ~UiLedPin[D].Mask;\
  }

#define   DIODEOn(D) \
  {\
    *((unsigned int*)(UiLedPin[D].pGpio + 0x8))  =  UiLedPin[D].Mask;\
  }

#define   DIODEOff(D) \
  {\
    *((unsigned int*)(UiLedPin[D].pGpio + 0xC))  =  UiLedPin[D].Mask;\
  }

enum
{
  GP0_0,GP0_1,GP0_2,GP0_3,GP0_4,GP0_5,GP0_6,GP0_7,GP0_8,GP0_9,GP0_10,GP0_11,GP0_12,GP0_13,GP0_14,GP0_15,
  GP1_0,GP1_1,GP1_2,GP1_3,GP1_4,GP1_5,GP1_6,GP1_7,GP1_8,GP1_9,GP1_10,GP1_11,GP1_12,GP1_13,GP1_14,GP1_15,
  GP2_0,GP2_1,GP2_2,GP2_3,GP2_4,GP2_5,GP2_6,GP2_7,GP2_8,GP2_9,GP2_10,GP2_11,GP2_12,GP2_13,GP2_14,GP2_15,
  GP3_0,GP3_1,GP3_2,GP3_3,GP3_4,GP3_5,GP3_6,GP3_7,GP3_8,GP3_9,GP3_10,GP3_11,GP3_12,GP3_13,GP3_14,GP3_15,
  GP4_0,GP4_1,GP4_2,GP4_3,GP4_4,GP4_5,GP4_6,GP4_7,GP4_8,GP4_9,GP4_10,GP4_11,GP4_12,GP4_13,GP4_14,GP4_15,
  GP5_0,GP5_1,GP5_2,GP5_3,GP5_4,GP5_5,GP5_6,GP5_7,GP5_8,GP5_9,GP5_10,GP5_11,GP5_12,GP5_13,GP5_14,GP5_15,
  GP6_0,GP6_1,GP6_2,GP6_3,GP6_4,GP6_5,GP6_6,GP6_7,GP6_8,GP6_9,GP6_10,GP6_11,GP6_12,GP6_13,GP6_14,GP6_15,
  GP7_0,GP7_1,GP7_2,GP7_3,GP7_4,GP7_5,GP7_6,GP7_7,GP7_8,GP7_9,GP7_10,GP7_11,GP7_12,GP7_13,GP7_14,GP7_15,
  GP8_0,GP8_1,GP8_2,GP8_3,GP8_4,GP8_5,GP8_6,GP8_7,GP8_8,GP8_9,GP8_10,GP8_11,GP8_12,GP8_13,GP8_14,GP8_15,
  NO_OF_GPIOS,
  UART0_TXD,UART0_RXD,UART1_TXD,UART1_RXD,
  SPI0_MOSI,SPI0_MISO,SPI0_SCL,SPI0_CS,
  SPI1_MOSI,SPI1_MISO,SPI1_SCL,SPI1_CS,
  EPWM1A,EPWM1B,APWM0,APWM1,EPWM0B,AXR3,AXR4
};

typedef   struct
{
  int               Pin;
  unsigned short    MuxReg;
  unsigned int      Mask;
  unsigned int      Mode;
}
MRM;

MRM       MuxRegMap[] =
{ //  Pin     MuxReg  Mask        Mode

    { GP2_1,       6,     0xF0FFFFFF, 0x08000000 },
    { GP5_7,      12,     0xFFFFFFF0, 0x00000008 },
    { GP6_7,      14,     0xFFFFFFF0, 0x00000008 },
    { GP6_13,     13,     0xFFFFF0FF, 0x00000800 },
    { -1,          0,     0,          0 }
};

typedef struct
{
  int               Pin;      // GPIO pin number
  void*             pGpio;    // GPIO bank base address
  unsigned int      Mask;     // GPIO pin mask
}
INPIN;

INPIN UiLedPin[] =
{
  { GP6_7 ,  NULL, 0 },  // DIODE0
  { GP6_13,  NULL, 0 },  // DIODE1
  { GP2_1 ,  NULL, 0 },  // DIODE2
  { GP5_7 ,  NULL, 0 },  // DIODE3
};

void
gpio_set (int Pin)
{
  int     Tmp = 0;
  void    *Reg;

  if (Pin >= 0)
  {
    while ((MuxRegMap[Tmp].Pin != -1) && (MuxRegMap[Tmp].Pin != Pin))
    {
      Tmp++;
    }
    if (MuxRegMap[Tmp].Pin == Pin)
    {
      Reg   =  SYSCFG_BASE + 0x120 + (MuxRegMap[Tmp].MuxReg << 2);

      *(unsigned int*)Reg &=  MuxRegMap[Tmp].Mask;
      *(unsigned int*)Reg |=  MuxRegMap[Tmp].Mode;
    }
  }
}

void
gpio_init (void)
{
  int Pin;
  for (Pin = 0;Pin < NO_OF_LEDS;Pin++)
  {
    if (UiLedPin[Pin].Pin >= 0)
    {
      UiLedPin[Pin].pGpio  =  (struct gpio_controller *)(GPIO_BASE + ((UiLedPin[Pin].Pin >> 5) * 0x28) + 0x10);
      UiLedPin[Pin].Mask   =  (1 << (UiLedPin[Pin].Pin & 0x1F));

      gpio_set(UiLedPin[Pin].Pin);
    }
  }

  int Tmp;
  for (Tmp = 0;Tmp < NO_OF_LEDS;Tmp++)
    {
      DIODEInit(Tmp);
    }
}

void
led_test (void)
{
  DIODEOn(0);
  DIODEOn(1)
  DIODEOn(2);
  DIODEOn(3);
}
