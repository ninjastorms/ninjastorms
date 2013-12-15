
#include <stdio.h>
#include <stddef.h>

#define GPIO_BASE ((void*volatile)0x01E26000)
#define SYSCFG_BASE ((void*volatile)0x01C14000)
#define NO_OF_LEDS 4

#define   DIODEInit(D) \
  {\
    *((unsigned int*volatile)(UiLedPin[D].pGpio + 0xC))  =  UiLedPin[D].Mask;\
    *((unsigned int*volatile)(UiLedPin[D].pGpio      )) &= ~UiLedPin[D].Mask;\
  }

#define   DIODEOn(D) \
  {\
    *((unsigned int*volatile)(UiLedPin[D].pGpio + 0x8))  =  UiLedPin[D].Mask;\
  }

#define   DIODEOff(D) \
  {\
    *((unsigned int*volatile)(UiLedPin[D].pGpio + 0xC))  =  UiLedPin[D].Mask;\
  }

#define GP2_1   0x21
#define GP5_7   0x57
#define GP6_7   0x67
#define GP6_13  0x6D

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
  { GP6_7 ,  GPIO_BASE + 0x88, 0 },  // DIODE0
  { GP6_13,  GPIO_BASE + 0x88, 0 },  // DIODE1
  { GP2_1 ,  GPIO_BASE + 0x38, 0 },  // DIODE2
  { GP5_7 ,  GPIO_BASE + 0x60, 0 },  // DIODE3
};

void
gpio_set (int Pin)
{
  int     Tmp = 0;
  void    *Reg;

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

void
gpio_init (void)
{
  int i;
  for (i = 0; i < NO_OF_LEDS; ++i)
    {
      UiLedPin[i].Mask   =  (1 << (UiLedPin[i].Pin & 0x1F));

      gpio_set(UiLedPin[i].Pin);
      DIODEInit(i);
    }
}

void
led_test (void)
{
  DIODEOn(0); // left green
  DIODEOff(1); // left red
  DIODEOn(2); // nothing
  DIODEOff(3); // nothing
}
