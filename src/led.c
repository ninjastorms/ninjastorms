/*
 * LEGO® MINDSTORMS EV3
 *
 * Copyright (C) 2010-2013 The LEGO Group
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


/*! \page UiModule Ui Button/Led Module
 *
 *  Manages button and LEDs\n
 *
 *-  \subpage UiDriver
 *-  \subpage UiModuleMemory
 *-  \subpage UiModuleResources
 *
 */

#define DA8XX_GPIO_BASE      0x01e26000




#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct gpio_controller 
{
  unsigned int dir;
  unsigned int out_data;
  unsigned int set_data;
  unsigned int clr_data;
  unsigned int in_data;
  unsigned int set_rising;
  unsigned int clr_rising;
  unsigned int set_falling;
  unsigned int clr_falling;
  unsigned int intstat;
};

typedef struct gpio_controller *GPIOC;

struct gpio_inpin
{
  int             Pin;      // GPIO pin number
  GPIOC           pGpio;    // GPIO bank base address
  unsigned int    Mask;     // GPIO pin mask
};

typedef struct gpio_inpin INPIN;

enum AllPins
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



//static    int  ModuleInit(void);
//static    void ModuleExit(void);

int       Hw = 0;

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

enum      UiButPins
{
  BUT0,     // UP
  BUT1,     // ENTER
  BUT2,     // DOWN
  BUT3,     // RIGHT
  BUT4,     // LEFT
  BUT5,     // BACK
  BUT_PINS
};


INPIN     UiLedPin[LED_PINS];

INPIN     UiButPin[BUT_PINS];


#define   LEDS                          4
#define   NO_OF_LEDS                    LEDS
#define   BUTTONS                       6
#define   NO_OF_BUTTONS                 BUTTONS

/*! \page UiModuleResources Gpios and Resources used for Module
 *
 *  Describes use of gpio and resources\n
 *
 *  \verbatim
 */

INPIN     EP2_UiLedPin[LED_PINS] =
{
  { GP6_12,  NULL, 0 },  // DIODE1
  { GP6_14,  NULL, 0 },  // DIODE2
  { GP6_13,  NULL, 0 },  // DIODE3
  { GP6_7 ,  NULL, 0 },  // DIODE0
};

INPIN     EP2_UiButPin[BUT_PINS] =
{
  { GP7_15,  NULL, 0 },  // BUT0
  { GP1_13,  NULL, 0 },  // BUT1
  { GP7_14,  NULL, 0 },  // BUT2
  { GP7_12,  NULL, 0 },  // BUT3
  { GP6_6 ,  NULL, 0 },  // BUT4
  { GP6_10,  NULL, 0 },  // BUT5
};


INPIN     FINALB_UiLedPin[LED_PINS] =
{
  { GP6_12,  NULL, 0 },  // DIODE1
  { GP2_1 ,  NULL, 0 },  // DIODE2
  { GP6_13,  NULL, 0 },  // DIODE3
  { GP6_7 ,  NULL, 0 },  // DIODE0
};

INPIN     FINALB_UiButPin[BUT_PINS] =
{
  { GP7_15,  NULL, 0 },  // BUT0
  { GP0_1 ,  NULL, 0 },  // BUT1
  { GP7_14,  NULL, 0 },  // BUT2
  { GP7_12,  NULL, 0 },  // BUT3
  { GP6_6 ,  NULL, 0 },  // BUT4
  { GP6_10,  NULL, 0 },  // BUT5
};


INPIN     FINAL_UiLedPin[LED_PINS] =
{
  { GP6_7 ,  NULL, 0 },  // DIODE0
  { GP6_13,  NULL, 0 },  // DIODE1
  { GP2_1 ,  NULL, 0 },  // DIODE2
  { GP5_7 ,  NULL, 0 },  // DIODE3
};

INPIN     FINAL_UiButPin[BUT_PINS] =
{
  { GP7_15,  NULL, 0 },  // BUT0
  { GP0_1 ,  NULL, 0 },  // BUT1
  { GP7_14,  NULL, 0 },  // BUT2
  { GP7_12,  NULL, 0 },  // BUT3
  { GP6_6 ,  NULL, 0 },  // BUT4
  { GP6_10,  NULL, 0 },  // BUT5
};

/*  \endverbatim
 *  \n
 */

enum 
{
  FINAL,
  FINALB,
  EP2
};

INPIN     *pUiLedPin[] =
{
  [FINAL]     =   FINAL_UiLedPin,       //  FINAL   platform
  [FINALB]    =   FINALB_UiLedPin,      //  FINALB  platform
  [EP2]       =   EP2_UiLedPin,         //  EP2     platform
};


INPIN     *pUiButPin[] =
{
  [FINAL]     =   FINAL_UiButPin,       //  FINAL   platform
  [FINALB]    =   FINALB_UiButPin,      //  FINALB  platform
  [EP2]       =   EP2_UiButPin,         //  EP2     platform
};


//*****************************************************************************


typedef   struct
{
  int     Pin;
  unsigned short     MuxReg;
  unsigned int     Mask;
  unsigned int     Mode;
}
MRM;


MRM       MuxRegMap[] =
{ //  Pin     MuxReg  Mask        Mode

    { GP0_1 ,      1,     0xF0FFFFFF, 0x08000000 },
    { GP0_2 ,      1,     0xFF0FFFFF, 0x00800000 },
    { GP0_3 ,      1,     0xFFF0FFFF, 0x00080000 },
    { GP0_4 ,      1,     0xFFFF0FFF, 0x00008000 },
    { GP0_5 ,      1,     0xFFFFF0FF, 0x00000800 },
    { GP0_6 ,      1,     0xFFFFFF0F, 0x00000080 },
    { GP0_7 ,      1,     0xFFFFFFF0, 0x00000008 },

    { GP0_11,      0,     0xFFF0FFFF, 0x00080000 },
    { GP0_12,      0,     0xFFFF0FFF, 0x00008000 },
    { GP0_13,      0,     0xFFFFF0FF, 0x00000800 },
    { GP0_14,      0,     0xFFFFFF0F, 0x00000080 },
    { GP0_15,      0,     0xFFFFFFF0, 0x00000008 },

    { GP1_0 ,      4,     0x0FFFFFFF, 0x80000000 },
    { GP1_8 ,      3,     0xFFFFFFF0, 0x00000004 },

    { GP1_9,       2,     0xF0FFFFFF, 0x04000000 },
    { GP1_10,      2,     0xFF0FFFFF, 0x00400000 },
    { GP1_11,      2,     0xFFF0FFFF, 0x00040000 },
    { GP1_12,      2,     0xFFFF0FFF, 0x00004000 },
    { GP1_13,      2,     0xFFFFF0FF, 0x00000400 },
    { GP1_14,      2,     0xFFFFFF0F, 0x00000040 },
    { GP1_15,      2,     0xFFFFFFF0, 0x00000008 },
    { GP2_0,       6,     0x0FFFFFFF, 0x80000000 },
    { GP2_1,       6,     0xF0FFFFFF, 0x08000000 },
    { GP2_2,       6,     0xFF0FFFFF, 0x00800000 },
    { GP2_3,       6,     0xFFF0FFFF, 0x00080000 },
    { GP2_4,       6,     0xFFFF0FFF, 0x00008000 },
    { GP2_5,       6,     0xFFFFF0FF, 0x00000800 },
    { GP2_6,       6,     0xFFFFFF0F, 0x00000080 },
    { GP2_7,       6,     0xFFFFFFF0, 0x00000008 },

    { GP2_8,       5,     0x0FFFFFFF, 0x80000000 },
    { GP2_9,       5,     0xF0FFFFFF, 0x08000000 },
    { GP2_10,      5,     0xFF0FFFFF, 0x00800000 },
    { GP2_11,      5,     0xFFF0FFFF, 0x00080000 },
    { GP2_12,      5,     0xFFFF0FFF, 0x00008000 },
    { GP2_13,      5,     0xFFFFF0FF, 0x00000800 },

    { GP3_0,       8,     0x0FFFFFFF, 0x80000000 },
    { GP3_1 ,      8,     0xF0FFFFFF, 0x08000000 },
    { GP3_2,       8,     0xFF0FFFFF, 0x00800000 },
    { GP3_3,       8,     0xFFF0FFFF, 0x00080000 },
    { GP3_4,       8,     0xFFFF0FFF, 0x00008000 },
    { GP3_5,       8,     0xFFFFF0FF, 0x00000800 },
    { GP3_6,       8,     0xFFFFFF0F, 0x00000080 },
    { GP3_7,       8,     0xFFFFFFF0, 0x00000008 },

    { GP3_8,       7,     0x0FFFFFFF, 0x80000000 },
    { GP3_9,       7,     0xF0FFFFFF, 0x08000000 },
    { GP3_10,      7,     0xFF0FFFFF, 0x00800000 },
    { GP3_11,      7,     0xFFF0FFFF, 0x00080000 },
    { GP3_12,      7,     0xFFFF0FFF, 0x00008000 },
    { GP3_13,      7,     0xFFFFF0FF, 0x00000800 },
    { GP3_14,      7,     0xFFFFFF0F, 0x00000080 },
    { GP3_15,      7,     0xFFFFFFF0, 0x00000008 },

    { GP4_1,      10,     0xF0FFFFFF, 0x08000000 },

    { GP4_8,       9,     0x0FFFFFFF, 0x80000000 },
    { GP4_9,       9,     0xF0FFFFFF, 0x08000000 },
    { GP4_10,      9,     0xFF0FFFFF, 0x00800000 },

    { GP4_12,      9,     0xFFFF0FFF, 0x00008000 },

    { GP4_14,      9,     0xFFFFFF0F, 0x00000080 },

    { GP5_0,      12,     0x0FFFFFFF, 0x80000000 },
    { GP5_1,      12,     0xF0FFFFFF, 0x08000000 },
    { GP5_2,      12,     0xFF0FFFFF, 0x00800000 },
    { GP5_3,      12,     0xFFF0FFFF, 0x00080000 },
    { GP5_4,      12,     0xFFFF0FFF, 0x00008000 },
    { GP5_5,      12,     0xFFFFF0FF, 0x00000800 },
    { GP5_6,      12,     0xFFFFFF0F, 0x00000080 },
    { GP5_7,      12,     0xFFFFFFF0, 0x00000008 },

    { GP5_8,      11,     0x0FFFFFFF, 0x80000000 },
    { GP5_9,      11,     0xF0FFFFFF, 0x08000000 },
    { GP5_10,     11,     0xFF0FFFFF, 0x00800000 },
    { GP5_11,     11,     0xFFF0FFFF, 0x00080000 },
    { GP5_12,     11,     0xFFFF0FFF, 0x00008000 },
    { GP5_13,     11,     0xFFFFF0FF, 0x00000800 },
    { GP5_14,     11,     0xFFFFFF0F, 0x00000080 },
    { GP5_15,     11,     0xFFFFFFF0, 0x00000008 },

    { GP6_0 ,     19,     0xF0FFFFFF, 0x08000000 },
    { GP6_1,      19,     0xFF0FFFFF, 0x00800000 },
    { GP6_2,      19,     0xFFF0FFFF, 0x00080000 },
    { GP6_3,      19,     0xFFFF0FFF, 0x00008000 },
    { GP6_4,      19,     0xFFFFF0FF, 0x00000800 },
    { GP6_5,      16,     0xFFFFFF0F, 0x00000080 },

    { GP6_6,      14,     0xFFFFFF0F, 0x00000080 },
    { GP6_7,      14,     0xFFFFFFF0, 0x00000008 },

    { GP6_8,      13,     0x0FFFFFFF, 0x80000000 },
    { GP6_9,      13,     0xF0FFFFFF, 0x08000000 },
    { GP6_10,     13,     0xFF0FFFFF, 0x00800000 },
    { GP6_11,     13,     0xFFF0FFFF, 0x00080000 },
    { GP6_12,     13,     0xFFFF0FFF, 0x00008000 },
    { GP6_13,     13,     0xFFFFF0FF, 0x00000800 },
    { GP6_14,     13,     0xFFFFFF0F, 0x00000080 },
    { GP6_15,     13,     0xFFFFFFF0, 0x00000008 },

    { GP7_4,      17,     0xFF0FFFFF, 0x00800000 },
    { GP7_8,      17,     0xFFFFFF0F, 0x00000080 },
    { GP7_9,      17,     0xFFFFFFF0, 0x00000008 },
    { GP7_10,     16,     0x0FFFFFFF, 0x80000000 },
    { GP7_11,     16,     0xF0FFFFFF, 0x08000000 },
    { GP7_12,     16,     0xFF0FFFFF, 0x00800000 },
    { GP7_13,     16,     0xFFF0FFFF, 0x00080000 },
    { GP7_14,     16,     0xFFFF0FFF, 0x00008000 },
    { GP7_15,     16,     0xFFFFF0FF, 0x00000800 },

    { GP8_2 ,     3 ,     0xF0FFFFFF, 0x04000000 },
    { GP8_3 ,     3 ,     0xFF0FFFFF, 0x00400000 },
    { GP8_5 ,     3 ,     0xFFFF0FFF, 0x00004000 },
    { GP8_6 ,     3 ,     0xFFFFF0FF, 0x00000400 },
    { GP8_8 ,     19,     0xFFFFFF0F, 0x00000080 },
    { GP8_9 ,     19,     0xFFFFFFF0, 0x00000008 },
    { GP8_10,     18,     0x0FFFFFFF, 0x80000000 },
    { GP8_11,     18,     0xF0FFFFFF, 0x08000000 },
    { GP8_12,     18,     0xFF0FFFFF, 0x00800000 },
    { GP8_13,     18,     0xFFF0FFFF, 0x00080000 },
    { GP8_14,     18,     0xFFFF0FFF, 0x00008000 },
    { GP8_15,     18,     0xFFFFF0FF, 0x00000800 },


    { UART0_TXD,   3,     0xFF0FFFFF, 0x00200000 },
    { UART0_RXD,   3,     0xFFF0FFFF, 0x00020000 },

    { UART1_TXD,   4,     0x0FFFFFFF, 0x20000000 },
    { UART1_RXD,   4,     0xF0FFFFFF, 0x02000000 },

    { SPI0_MOSI,   3,     0xFFFF0FFF, 0x00001000 },
    { SPI0_MISO,   3,     0xFFFFF0FF, 0x00000100 },
    { SPI0_SCL,    3,     0xFFFFFFF0, 0x00000001 },
    { SPI0_CS,     3,     0xF0FFFFFF, 0x01000000 },

    { SPI1_MOSI,   5,     0xFF0FFFFF, 0x00100000 },
    { SPI1_MISO,   5,     0xFFF0FFFF, 0x00010000 },
    { SPI1_SCL,    5,     0xFFFFF0FF, 0x00000100 },
    { SPI1_CS,     5,     0xFFFF0FFF, 0x00008000 },

    { EPWM1A,      5,     0xFFFFFFF0, 0x00000002 },
    { EPWM1B,      5,     0xFFFFFF0F, 0x00000020 },
    { APWM0,       2,     0x0FFFFFFF, 0x20000000 },
    { APWM1,       1,     0x0FFFFFFF, 0x40000000 },
    { EPWM0B,      3,     0xFFFFFF0F, 0x00000020 },

    { AXR3,        2,     0xFFF0FFFF, 0x00010000 },
    { AXR4,        2,     0xFFFF0FFF, 0x00001000 },

    {-1,           0,     0,          0}
};

#define IO_PHYS				0x01c00000
#define DA8XX_SYSCFG0_BASE	(IO_PHYS + 0x14000)


static    void  *GpioBase;

void      SetGpio(int Pin)
{
  int     Tmp = 0;
  void     *Reg;

  if (Pin >= 0)
  {
    while ((MuxRegMap[Tmp].Pin != -1) && (MuxRegMap[Tmp].Pin != Pin))
    {
      Tmp++;
    }
    if (MuxRegMap[Tmp].Pin == Pin)
    {
      Reg   =  ((void*)DA8XX_SYSCFG0_BASE) + 0x120 + (MuxRegMap[Tmp].MuxReg << 2);

      *(unsigned int*)Reg &=  MuxRegMap[Tmp].Mask;
      *(unsigned int*)Reg |=  MuxRegMap[Tmp].Mode;

    }
    else
    {
      printf("*   GP%i_%i  ********* ERROR not found *********\n",(Pin >> 4),(Pin & 0x0F));
    }
  }
}


void      InitGpio(void)
{
  int     Pin;
  memcpy(UiLedPin, pUiLedPin[Hw], sizeof(EP2_UiLedPin));
  if (memcmp((const void*)UiLedPin, (const void*)pUiLedPin[Hw], sizeof(EP2_UiLedPin)) != 0)
  {
    printf("UiLedPin tabel broken!\n");
  }

  for (Pin = 0;Pin < NO_OF_LEDS;Pin++)
  {
    if (UiLedPin[Pin].Pin >= 0)
    {
      UiLedPin[Pin].pGpio  =  (struct gpio_controller *)(GpioBase + ((UiLedPin[Pin].Pin >> 5) * 0x28) + 0x10);
      UiLedPin[Pin].Mask   =  (1 << (UiLedPin[Pin].Pin & 0x1F));

      SetGpio(UiLedPin[Pin].Pin);
    }
  }

  // memcpy(UiButPin,pUiButPin[Hw],sizeof(EP2_UiButPin));
  // if (memcmp((const void*)UiButPin,(const void*)pUiButPin[Hw],sizeof(EP2_UiButPin)) != 0)
  // {
  //   printf("UiButPin tabel broken!\n");
  // }

  // for (Pin = 0;Pin < NO_OF_BUTTONS;Pin++)
  // {
  //   if (UiButPin[Pin].Pin >= 0)
  //   {
  //     UiButPin[Pin].pGpio  =  (struct gpio_controller *)(GpioBase + ((UiButPin[Pin].Pin >> 5) * 0x28) + 0x10);
  //     UiButPin[Pin].Mask   =  (1 << (UiButPin[Pin].Pin & 0x1F));

  //     SetGpio(UiButPin[Pin].Pin);
  //   }
  // }
}



// DEVICE1 ********************************************************************

#define   BUTFloat(B)                   {\
                                          (*UiButPin[B].pGpio).dir |=  UiButPin[B].Mask;\
                                        }


#define   BUTRead(B)                    ((*UiButPin[B].pGpio).in_data & UiButPin[B].Mask)


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


#define LEDPATTERNS 10

unsigned long     LEDPATTERNDATA[NO_OF_LEDS + 1][LEDPATTERNS] =
{ //  LED_BLACK   LED_GREEN   LED_RED    LED_ORANGE           LED_GREEN_FLASH                     LED_RED_FLASH                     LED_ORANGE_FLASH                      LED_GREEN_PULSE                       LED_RED_PULSE                      LED_ORANGE_PULSE
  {  0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0b00000000000000000000000000000000, 0b00000000000000000111110000011111, 0b00000000000000000111110000011111, 0b00000000000000000000000000000000, 0b00000000000000000000000001110111, 0b00000000000000000000000001110111 }, // RR
  {  0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0b00000000000000000111110000011111, 0b00000000000000000000000000000000, 0b00000000000000000111110000011111, 0b00000000000000000000000001110111, 0b00000000000000000000000000000000, 0b00000000000000000000000001110111 }, // RG
  {  0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0b00000000000000000000000000000000, 0b00000000000000000111110000011111, 0b00000000000000000111110000011111, 0b00000000000000000000000000000000, 0b00000000000000000000000001110111, 0b00000000000000000000000001110111 }, // LR
  {  0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0b00000000000000000111110000011111, 0b00000000000000000000000000000000, 0b00000000000000000111110000011111, 0b00000000000000000000000001110111, 0b00000000000000000000000000000000, 0b00000000000000000000000001110111 }, // LG
  { 0 }
};


unsigned char     PatternBlock    = 0;          // Block pattern update
unsigned char     PatternBits     = 20;         // Pattern bits
unsigned char     PatternBit      = 0;          // Pattern bit pointer
unsigned long     ActPattern[NO_OF_LEDS];
unsigned long     TmpPattern[NO_OF_LEDS];

void blink(void)
{
  unsigned char   Tmp;
  for (Tmp = 0;Tmp < NO_OF_LEDS;Tmp++)
  {
    ActPattern[Tmp]  =  LEDPATTERNDATA[Tmp][1];
  }



  if (PatternBlock)
  {
    PatternBlock  =  0;
  }
  else
  {
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


void Device2TimerInterrupt1(void)
{
  unsigned short   Tmp;

  for(Tmp = 0;Tmp < BUT_PINS;Tmp++)
  {
    if (BUTRead(Tmp))
    { // Button active

      //(*pUi).Pressed[Tmp]  =  1;

    }
    else
    { // Button inactive

      //(*pUi).Pressed[Tmp]  =  0;

    }
  }
}


/*! \page UiDriver
 *
 *  <hr size="1"/>
 *  <b>     write </b>
 *
 *
 *  CP        C = Color, P = Pattern
 *
 *  C = 0     Off
 *  C = 1     Green
 *  C = 2     Red
 *  C = 3     Orange
 *
 *  P = 0     Off
 *  P = 1     On
 *  P = 2     50% (250mS on, 250mS off)
 *  P = 3     30% (150mS on, 50mS off, 150mS on, 650mS off)
 *
 *- 0pct      Set all LED = pct ["0".."100"]
 *- 1pct      Set LED 1 = pct
 *- 2pct      Set LED 2 = pct
 *- 3pct      Set LED 3 = pct
 *- 4pct      Set LED 4 = pct
 */
/*! \brief    Device1Write
 *
 *
 *
 */
/*static ssize_t Device1Write(struct file *File,const char *Buffer,size_t Count,loff_t *Data)
{
  char    Buf[2];
  unsigned char   No;
  unsigned char   Tmp;
  int     Lng     = 0;

  if (Count >= 2)
  {
    Lng     =  Count;
    copy_from_user(Buf,Buffer,2);
    No      =  Buf[0] - '0';
    if ((No >= 0) && (No < LEDPATTERNS))
    {
      PatternBlock  =  1;

      PatternBits   =  20;
      PatternBit    =  0;

      for (Tmp = 0;Tmp < NO_OF_LEDS;Tmp++)
      {
        ActPattern[Tmp]  =  LEDPATTERNDATA[Tmp][No];
      }

      PatternBlock  =  0;
    }
  }

  return (Lng);
}*/


/*static ssize_t Device1Read(struct file *File,char *Buffer,size_t Count,loff_t *Offset)
{
  int     Lng = 0;

  Lng  =  snprintf(Buffer,Count,"V%c.%c0",HwId[0],HwId[1]);

  return (Lng);
}*/


#define     SHM_LENGTH    (sizeof(UiDefault))
#define     NPAGES        ((SHM_LENGTH + PAGE_SIZE - 1) / PAGE_SIZE)
//static void *kmalloc_ptr;

/*static int Device1Mmap(struct file *filp, struct vm_area_struct *vma)
{
   int ret;

   ret = remap_pfn_range(vma,vma->vm_start,virt_to_phys((void*)((unsigned long)pUi)) >> PAGE_SHIFT,vma->vm_end-vma->vm_start,PAGE_SHARED);

   if (ret != 0)
   {
     ret  =  -EAGAIN;
   }

   return (ret);
}*/


/*static    const struct file_operations Device1Entries =
{
  .owner        = THIS_MODULE,
  .read         = Device1Read,
  .write        = Device1Write,
  .mmap         = Device1Mmap
};*/


/*static    struct miscdevice Device1 =
{
  MISC_DYNAMIC_MINOR,
  DEVICE1_NAME,
  &Device1Entries
};*/

int Device1Init(void)
{
  //int     Result = -1;
  int     Tmp;
  //int     i;
  //unsigned short   *pTmp;

  //Result  =  misc_register(&Device1);
  //if (Result)
  //{
  //  printk("  %s device register failed\n",DEVICE1_NAME);
  //}
  //else
  //{
    // allocate kernel shared memory for ui button states (pUi)
    //if ((kmalloc_ptr = kmalloc((NPAGES + 2) * PAGE_SIZE, GFP_KERNEL)) != NULL)
    //{
      //pTmp = (unsigned short*)((((unsigned long)kmalloc_ptr) + PAGE_SIZE - 1) & PAGE_MASK);
      //for (i = 0; i < NPAGES * PAGE_SIZE; i += PAGE_SIZE)
      //{
      //  SetPageReserved(virt_to_page(((unsigned long)pTmp) + i));
      //}
      //pUi      =  (UI*)pTmp;
      //memset(pUi,0,sizeof(UI));

      for (Tmp = 0;Tmp < NO_OF_LEDS;Tmp++)
      {
        DIODEInit(Tmp);
      }
      /*for (Tmp = 0;Tmp < NO_OF_BUTTONS;Tmp++)
      {
        BUTFloat(Tmp);
      }

      // setup ui update timer interrupt
      Device2Time  =  ktime_set(0,10000000);
      hrtimer_init(&Device2Timer,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
      Device2Timer.function  =  Device2TimerInterrupt1;
      hrtimer_start(&Device2Timer,Device2Time,HRTIMER_MODE_REL);

      // setup ui update timer interrupt
      Device1Time  =  ktime_set(0,50000000);
      hrtimer_init(&Device1Timer,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
      Device1Timer.function  =  Device1TimerInterrupt1;
      hrtimer_start(&Device1Timer,Device1Time,HRTIMER_MODE_REL);
*/
#ifdef DEBUG
      printk("  %s device register succes\n",DEVICE1_NAME);
#endif
    //}
  //}

  return 0;
}


void Device1Exit(void)
{
  int     Tmp;
  //int     i;
  //unsigned short   *pTmp;

  //hrtimer_cancel(&Device1Timer);
  //hrtimer_cancel(&Device2Timer);

  for (Tmp = 0;Tmp < NO_OF_LEDS;Tmp++)
  {
    DIODEOff(Tmp);
  }

  // free shared memory
  //pTmp   =  (unsigned short*)pUi;
  //pUi    =  &UiDefault;

  /*for (i = 0; i < NPAGES * PAGE_SIZE; i+= PAGE_SIZE)
  {
    ClearPageReserved(virt_to_page(((unsigned long)pTmp) + i));
#ifdef DEBUG
    printk("  %s memory page %d unmapped\n",DEVICE1_NAME,i);
#endif
  }*/
  //kfree(kmalloc_ptr);

  //misc_deregister(&Device1);
#ifdef DEBUG
  printk("  %s device unregistered\n",DEVICE1_NAME);
#endif
}


// MODULE *********************************************************************


#ifndef PCASM
//module_param (HwId, charp, 0);
#endif

int LEDInit(void)
{
  GpioBase  =  (void*)DA8XX_GPIO_BASE;
  InitGpio();

  Device1Init();

  return (0);
}


/*
static void ModuleExit(void)
{
#ifdef DEBUG
  printk("%s exit started\n",MODULE_NAME);
#endif

  Device1Exit();
  iounmap(GpioBase);

}*/
