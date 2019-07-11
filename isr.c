#include "isr.h"
#include "tarif_struct.h"
#include "error_exit.h"
#include <stdio.h>
#include <stdlib.h>
#include <mraa/gpio.h>

extern volatile unsigned long long int e_meter_0;
extern volatile unsigned long long int e_meter_1;

extern tarif_switch tarif_flag;

void isr_signal_tarif_0(void* args){
	e_meter_0++;

	//fprintf(stdout, "signal detected: e_meter_0: %llu\n", e_meter_0);
	/* fprintf(stdout, "sizeof: e_meter: %u\n", sizeof(e_meter)); = 8 */
}

void isr_signal_tarif_1(void* args){
	e_meter_1++;

	//fprintf(stdout, "signal detected: e_meter_1: %llu\n", e_meter_1);
	/* fprintf(stdout, "sizeof: e_meter: %u\n", sizeof(e_meter)); = 8 */
}

void isr_signal_tarif_switch(void* args){
	tarif_flag.b_isr_trig = 1;
}

void install_isr(mraa_gpio_context *gpio, void (*isr)(void*)){

	char err_msg[100];
	mraa_result_t status = MRAA_SUCCESS;

	status = mraa_gpio_isr(*gpio, MRAA_GPIO_EDGE_FALLING,isr, NULL);

	if (status != MRAA_SUCCESS) {
		sprintf(err_msg, "%s() - %s\n", __func__, "failed to install ISR");
		gpio_error(err_msg);
	}
}

void deinstall_isr(mraa_gpio_context *gpio){

	char err_msg[100];
	mraa_result_t status = MRAA_SUCCESS;

	status = mraa_gpio_isr_exit(*gpio);

	if (status != MRAA_SUCCESS) {
		sprintf(err_msg, "%s() - %s\n", __func__, "failed to deinstall ISR");
		gpio_error(err_msg);
	}
}

void set_led(mraa_gpio_context *gpio, int state){

	char err_msg[100];
	mraa_result_t status = MRAA_SUCCESS;

	status = mraa_gpio_write(*gpio, state);

	if (status != MRAA_SUCCESS) {
		sprintf(err_msg, "%s() - %s\n", __func__, "failed to write DO");
		gpio_error(err_msg);
	}
}

