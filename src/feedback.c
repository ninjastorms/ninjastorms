#include "feedback.h"

void feedback_wait (unsigned long ticks) {
  volatile unsigned int timer = 0;
  while (timer < ticks) ++timer;
}

void blink(led_id led, led_color color) {
  unsigned int i;
  for (i = 0; i < 6; ++i)
    {
      led_set(led, (i & 1 ? LED_BLACK : color));
      feedback_wait(512 * 1024);
    }
}

void flash_LR(led_color color) {
  led_set(LED_LEFT, color);
  feedback_wait(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  led_set(LED_RIGHT, color);
  feedback_wait(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  feedback_wait(512 * 1024);
}

void flash_RL(led_color color) {
  led_set(LED_RIGHT, color);
  feedback_wait(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  led_set(LED_LEFT, color);
  feedback_wait(64 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  feedback_wait(512 * 1024);
}
