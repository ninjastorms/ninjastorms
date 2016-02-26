
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

#pragma once

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

enum led_id
{
  LED_LEFT   = 0x01,
  LED_RIGHT  = 0x02,
  LED_BOTH   = LED_LEFT | LED_RIGHT
};
typedef enum led_id led_id;

enum led_color
{
  LED_BLACK  = 0x00,
  LED_RED    = 0x01,
  LED_GREEN  = 0x02,
  LED_ORANGE = LED_RED | LED_GREEN
};
typedef enum led_color led_color;


/* set the state of the given LED(s)
 *
 * params:
 *   led - an identifier for the target LEDs, can be LED_LEFT, LED_RIGHT
 *   or LED_BOTH
 *   color - the target color, LED_BLACK, LED_RED, LED_GREEN or LED_ORANGE
 */
void led_set (led_id led, led_color color);
