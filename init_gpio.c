#include "init_gpio.h"
#include "error_exit.h"
#include <stdlib.h>
#include <mraa/gpio.h>
#include <mraa/types.h>

#define GPIO_DI0 12
#define GPIO_DI2 10
#define GPIO_LED 13


void init_gpio(mraa_gpio_context *di0, mraa_gpio_context *di2, mraa_gpio_context *led){

	mraa_result_t status = MRAA_SUCCESS;
	char err_msg[100];

	mraa_init();

	/*
	 *  GPIO SETUP
	 */

	*di0 = mraa_gpio_init(GPIO_DI0);
	if (*di0 == NULL) {
		sprintf(err_msg, "%s() - %s %d\n", __func__, "failed to initialize GPIO", GPIO_DI0);
		gpio_error(err_msg);
	}

	*di2 = mraa_gpio_init(GPIO_DI2);
	if (*di2 == NULL) {
		sprintf(err_msg, "%s() - %s %d\n", __func__, "failed to initialize GPIO", GPIO_DI2);
		gpio_error(err_msg);
	}


	*led = mraa_gpio_init(GPIO_LED);
	if (*led == NULL) {
		sprintf(err_msg, "%s() - %s %d\n", __func__, "failed to initialize GPIO", GPIO_LED);
		gpio_error(err_msg);
	}



	status = mraa_gpio_dir(*di0, MRAA_GPIO_IN);
	if (status != MRAA_SUCCESS) {
		sprintf(err_msg, "%s() - %s %d\n", __func__, "input initialization failed for GPIO", GPIO_DI0);
		gpio_error(err_msg);
	}


	status = mraa_gpio_dir(*di2, MRAA_GPIO_IN);
	if (status != MRAA_SUCCESS) {
		sprintf(err_msg, "%s() - %s %d\n", __func__, "input initialization failed for GPIO", GPIO_DI2);
		gpio_error(err_msg);
	}


	status = mraa_gpio_dir(*led, MRAA_GPIO_OUT);
	if (status != MRAA_SUCCESS) {
		sprintf(err_msg, "%s() - %s\n", __func__, "failed to initialize DO");
		gpio_error(err_msg);
	}
}
