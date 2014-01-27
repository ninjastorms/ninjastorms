
#pragma once

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
 *   	or LED_BOTH
 *   color - the target color, LED_BLACK, LED_RED, LED_GREEN or LED_ORANGE
 */
void led_set(led_id led, led_color color);
