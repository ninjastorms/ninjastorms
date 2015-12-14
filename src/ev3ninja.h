
#pragma once

int ev3ninja_main (void);
void irq_handler(void) __attribute__((interrupt ("IRQ")));

