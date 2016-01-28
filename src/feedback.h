
#pragma once

#include <libp/led.h>

/* wait for a given number of ticks (1024 * 1024 ~ 1s)
 *
 * params:
 *   ticks - the number of ticks to wait
 */
void feedback_wait(unsigned long ticks);

void blink(led_id led, led_color color);

void flash_LR(led_color color);
void flash_RL(led_color color);