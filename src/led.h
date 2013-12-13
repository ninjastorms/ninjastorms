#ifndef LED_H
#define LED_H

#include "util.h"

#define NO_OF_LEDS 4


#define   DIODEInit(D)                  {\
                                          (*UiLedPin[D].pGpio).clr_data  =  UiLedPin[D].Mask;\
                                          (*UiLedPin[D].pGpio).dir      &= ~UiLedPin[D].Mask;\
                                        }

#define   DIODEOn(D)                    {\
                                          (*UiLedPin[D].pGpio).set_data  =  UiLedPin[D].Mask;\
                                        }

#define   DIODEOff(D)                   {\
                                          (*UiLedPin[D].pGpio).clr_data  =  UiLedPin[D].Mask;\
                                        }

enum      UiLedPins
{
  DIODE0,
  DIODE1,
  DIODE2,
  DIODE3,
  DIODE4,
  DIODE5,
  LED_PINS
};


void blink(void);


#endif