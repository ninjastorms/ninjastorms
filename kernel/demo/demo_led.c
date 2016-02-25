#include "demo_led.h"
#include "kernel/feedback.h"

void lightshow(void) {
  led_set(LED_LEFT, LED_GREEN);
  feedback_wait(512 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  feedback_wait(512 * 1024);
  led_set(LED_RIGHT, LED_GREEN);
  feedback_wait(512 * 1024);
  led_set(LED_BOTH, LED_BLACK);
  feedback_wait(512 * 1024);
  blink(LED_BOTH, LED_GREEN);
  feedback_wait(1024 * 1024);
  flash_RL(LED_RED);
  flash_LR(LED_RED);
  feedback_wait(512 * 1024);
  flash_RL(LED_GREEN);
  flash_RL(LED_RED);
  flash_RL(LED_ORANGE);
  blink(LED_BOTH, LED_RED);
  led_set(LED_LEFT, LED_GREEN);
  feedback_wait(1024 * 1024);
  led_set(LED_RIGHT, LED_RED);
  feedback_wait(1024 * 1024);
  blink(LED_LEFT, LED_GREEN);
  blink(LED_RIGHT, LED_RED);
  led_set(LED_BOTH, LED_ORANGE);
  feedback_wait(1024 * 1024);
  blink(LED_BOTH, LED_ORANGE);
  feedback_wait(2 * 1024 * 1024);
}
