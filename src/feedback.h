
#pragma once

/* wait for a given number of ticks (1024 * 1024 ~ 1s)
 *
 * params:
 *   ticks - the number of ticks to wait
 */
void feedback_wait(unsigned long ticks);

/* flash the leds green, this is done at the very end of the startup process
 */
void feedback_flash_green(void);

/* flash the leds red, this is done at the start of the shutdown process
 */
void feedback_flash_red(void);
