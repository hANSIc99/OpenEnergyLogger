#include "error_exit.h"
#include <stdlib.h>
#include <sqlite3.h>
#include <mraa/gpio.h>

extern sqlite3 *db;

void file_error(char *err_msg){

	fprintf(stderr, "s0_scn: FILE ERROR: %s\n", err_msg);
	fflush(stderr);
	sqlite3_close(db);
    mraa_deinit();

	exit(EXIT_FAILURE);
}

void sqlite_error(char *err_msg){

	fprintf(stderr, "s0_scn: SQLITE ERROR: %s\n", err_msg);
	fflush(stderr);

	sqlite3_close(db);
    mraa_deinit();

	exit(EXIT_FAILURE);
}

void gpio_error(char *err_msg){

	fprintf(stderr, "s0_scn: GPIO ERROR: %s\n", err_msg);
	fflush(stderr);
	sqlite3_close(db);
	mraa_deinit();

	exit(EXIT_FAILURE);
}
