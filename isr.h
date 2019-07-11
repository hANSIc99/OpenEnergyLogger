#ifndef __isr_h
#define __isr_h
#include <mraa/gpio.h>

void isr_signal_tarif_0(void* args);

void isr_signal_tarif_1(void* args);

void isr_signal_tarif_switch(void* args);

void install_isr(mraa_gpio_context *gpio, void (*isr)(void*));

void deinstall_isr(mraa_gpio_context *gpio);

void set_led(mraa_gpio_context *gpio, int state);

#endif
