
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

int 
ev3ninja_main (void)
{
  puts("This is EV3 NinjaStorms");
  puts("  shuriken ready");
  feedback_flash_green();

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

  puts("All done. ev3ninja out!");
  feedback_flash_red();

  return 0;
}

