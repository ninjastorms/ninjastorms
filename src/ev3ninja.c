
#include <stdio.h>

#include "feedback.h"

#include <libp/led.h>
#include <libp/button.h>
#include <libp/sensor.h>
#include <libp/motor.h>

#include <libp/gpio.h>
#include <libp/spi.h>

#define P (1.4)
#define I (0.0002)
#define D (-110)

// Versatile stuff
#define TIMER1_BASE 0x101E2000
#define TIMER1_INTBIT 1 << 4

#define TIMER1_LOAD (volatile unsigned int*)(TIMER1_BASE+0x0)
#define TIMER1_VALUE (volatile unsigned int*)(TIMER1_BASE+0x4)
#define TIMER1_CTRL (volatile char*)(TIMER1_BASE+0x8)
#define TIMER1_INTCLR (volatile char*)(TIMER1_BASE+0xC)
#define TIMER1_RIS (volatile char*)(TIMER1_BASE+0x10)
#define TIMER1_MIS (volatile char*)(TIMER1_BASE+0x14)

// Primary Interrupt Controller (PL190)
#define PIC_BASE 0x10140000
#define PIC_INTENABLE (volatile char*)(PIC_BASE+0x10)
#define PIC_SOFTINTCLEAR (volatile unsigned int*)(PIC_BASE+0x14)
#define PIC_SOFTINT (volatile unsigned int*)(PIC_BASE+0x18)
#define PIC_DEFVECTADDR (volatile unsigned int*)(PIC_BASE+0x34)
#define TIMER1_INTBIT 1 << 4

#define NOP_SLIDE 0x1003c0

void run_segway(void) {
  unsigned int active = 0;

  unsigned int button_center_last_state = BUTTON_UP;

  int t = 0;

  int d1 = 0;
  int d2 = 0;

  int prev_p = 0;
  int p = 0;
  int d = 0;
  int i = 0;

  while (1)
    {
      if (button_get_state(BUTTON_BACK) == BUTTON_DOWN)
        {
          d1 = sensor_light_get(SENSOR_PORT_2);
          d2 = sensor_light_get(SENSOR_PORT_4);          
          i = 0;
        }

      if (button_get_state(BUTTON_CENTER) == BUTTON_DOWN)
        {
          d1 = sensor_light_get(SENSOR_PORT_2);
          d2 = sensor_light_get(SENSOR_PORT_4);          
          i = 0;

          if (button_center_last_state == BUTTON_UP)
            active ^= 1;
          button_center_last_state = BUTTON_DOWN;
        }
      else
        button_center_last_state = BUTTON_UP;

      int l1 = sensor_light_get(SENSOR_PORT_2) - d1;
      int l2 = sensor_light_get(SENSOR_PORT_4) - d2;

      int diff = (l1) - (l2);

      prev_p = p;
      p = diff;
      d = p - prev_p;
      i = i + p;

      int speed = (P * p + D * d + I * i);
      if (speed < -5) 
        speed -= 20;
      else if (speed > 5)
        speed += 20;

      if (speed > 100)
        speed = 100;
      if (speed < -100)
        speed = -100;

      //printf("%i, %i - %i %i %i - %i\n", l1, l2, p, i, d, speed);

      if (!active)
        {
          motor_set_state(MOTOR_PORT_A, MOTOR_OFF);
          motor_set_state(MOTOR_PORT_D, MOTOR_OFF);
          continue;
        }

        t += speed; 
        if (t > 100)
          {
            t -= 100;
            motor_set_state(MOTOR_PORT_A, MOTOR_BACKWARD);
            motor_set_state(MOTOR_PORT_D, MOTOR_FORWARD);
          }
        else if (t < -100)
          {
            t += 100;
            motor_set_state(MOTOR_PORT_A, MOTOR_FORWARD);
            motor_set_state(MOTOR_PORT_D, MOTOR_BACKWARD);
          }
        else
          {
            motor_set_state(MOTOR_PORT_A, MOTOR_OFF);
            motor_set_state(MOTOR_PORT_D, MOTOR_OFF);
          }
    }
  return;
}

#ifdef QEMU
#define UART_THR (volatile char*)(0x101f1000)
#endif

#ifndef QEMU
#define UART_THR (volatile char*)(0x01D0C000)
#define UART_LSR (volatile char*)(0x01D0C014)
#endif

void irq_handler(void) {

  asm("push {r0, r1, r2, r3, r4, r5}");

  *UART_THR = '#';
  *TIMER1_INTCLR = (char)0x1; // clear timer interrupt

  asm("pop {r0, r1, r2, r3, r4, r5}");
  asm("SUBS pc,lr,#4"); //return from IRQ
}

void swi_handler(void) {
  asm("push {r2, r3}");
  *UART_THR = 'S';
  asm("pop {r2, r3}");
  asm("movs pc, lr");
}

void init_interrupt_handling(void) {
  //build interrupt vector table
  *(unsigned int*) 0x00 = 0xe59ff014;	//TODO: reset
  *(unsigned int*) 0x04 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x20 undefined instruction
  *(unsigned int*) 0x08 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x24 software interrupt
  *(unsigned int*) 0x0c = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x28 prefetch abort
  *(unsigned int*) 0x10 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x2c data abort
  *(unsigned int*) 0x14 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x30 reserved
  *(unsigned int*) 0x18 = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x34 IRQ
  *(unsigned int*) 0x1c = 0xe59ff014;	//ldr pc, [pc, #20] ; 0x38 FIQ

  *(unsigned int*) 0x1c = 0x1000000; //TODO
  *(unsigned int*) 0x20 = 0x1000000; //TODO
  //ATTENTION: don't use software interrupts in supervisor mode
  *(unsigned int*) 0x24 = (unsigned int) &swi_handler;
  *(unsigned int*) 0x28 = 0x1000000; //TODO
  *(unsigned int*) 0x2c = 0x1000000; //TODO
  *(unsigned int*) 0x30 = 0x1000000; //TODO
  *(unsigned int*) 0x34 = (unsigned int) &irq_handler;
  *(unsigned int*) 0x38 = 0x1000000; //TODO

  // Enable IRQs
  asm(
    "MRS r1,cpsr\n\t"
    "BIC r1,r1, #0x80\n\t"
    "MSR cpsr_c, r1\n\t"
  );

  // Setting up primary interrupt controller
  *PIC_INTENABLE |= TIMER1_INTBIT;	// unmask interrupt bit for timer1
  return;
}

void init_timer(void) {
  *TIMER1_CTRL &= ~(1 << 7); // unset TimerEn(abled)

  *TIMER1_CTRL |= 1 << 6;    // set periodic-mode
  *TIMER1_CTRL |= 1 << 5;    // set IntEnable
  *TIMER1_CTRL |= 1 << 1;    // set 32-bit mode
  //*TIMER1_CTRL |= 1 << 0;    // set OneShot-Mode
  *TIMER1_CTRL &= ~(1 << 0);    // set Wrapping-Mode

  *TIMER1_LOAD = 0x5000;     // load
  *TIMER1_CTRL |= 1 << 7;    // set TimerEn(abled)
  return;
}

int
ev3ninja_main (void)
{
  init_interrupt_handling();
  
  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");
  
  feedback_flash_green();

  init_timer();
  while(1) {
    unsigned int i = 0xfffffff;
    while(i != 0) {
      i--;
    }
    printf("Timer: %x\n", *TIMER1_VALUE);
  }
  //run_segway();

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  return 0;
}

