
/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#include "demo_led.h"

#include "kernel/drivers/led.h"

static void
spin (unsigned int ticks)
{
  volatile unsigned int timer = 0;
  while (timer < ticks)
    ++timer;
}

static void
blink (led_id led, led_color color)
{
  unsigned int i;
  for (i = 0; i < 6; ++i)
    {
      led_set(led, (i & 1 ? LED_BLACK : color));
      spin(512 * 1024);
    }
}

static void
flash_LR (led_color color)
{
  led_set(LED_LEFT, color);
  spin(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  led_set(LED_RIGHT, color);
  spin(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  spin(512 * 1024);
}

static void
flash_RL (led_color color)
{
  led_set(LED_RIGHT, color);
  spin(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  led_set(LED_LEFT, color);
  spin(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  spin(512 * 1024);
}

void
demo_led (void)
{
  led_set(LED_LEFT, LED_GREEN);
  spin(512 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  spin(512 * 1024);
  led_set(LED_RIGHT, LED_GREEN);
  spin(512 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  spin(512 * 1024);
  blink(LED_BOTH, LED_GREEN);
  spin(1024 * 1024);
  flash_RL(LED_RED);
  flash_LR(LED_RED);
  spin(512 * 1024);
  flash_RL(LED_GREEN);
  flash_RL(LED_RED);
  flash_RL(LED_ORANGE);
  blink(LED_BOTH, LED_RED);
  led_set(LED_LEFT, LED_GREEN);
  spin(1024 * 1024);
  led_set(LED_RIGHT, LED_RED);
  spin(1024 * 1024);
  blink(LED_LEFT, LED_GREEN);
  blink(LED_RIGHT, LED_RED);
  led_set(LED_BOTH, LED_ORANGE);
  spin(1024 * 1024);
  blink(LED_BOTH, LED_ORANGE);
  spin(2 * 1024 * 1024);
}
