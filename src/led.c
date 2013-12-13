#include "led.h"

INPIN     UiLedPin[LED_PINS];

ULONG     ActPattern[NO_OF_LEDS] = {
    // LED_RED
    0xFFFFFFFF, 
    0x00000000,
    0xFFFFFFFF,
    0x00000000
  };

ULONG     TmpPattern[NO_OF_LEDS];
UBYTE     PatternBits     = 20;         // Pattern bits
UBYTE     PatternBit      = 0;          // Pattern bit pointer


void 
blink(void){
  
  UBYTE Tmp;
  
  for (Tmp = 0;Tmp < NO_OF_LEDS;Tmp++)
  {
    DIODEInit(Tmp);
  }

  for(int i = 0; i < 100; i++){
    for (Tmp = 0;Tmp < NO_OF_LEDS;Tmp++)
    {
      if (PatternBit == 0)
      {
        TmpPattern[Tmp]  =  ActPattern[Tmp];
      }
      if ((TmpPattern[Tmp] & 0x00000001))
      {
        DIODEOn(Tmp);
      }
      else
      {
        DIODEOff(Tmp);
      }
      TmpPattern[Tmp] >>=  1;
    }

    if (++PatternBit >= PatternBits)
    {
      PatternBit  =  0;
    }
  }
}
