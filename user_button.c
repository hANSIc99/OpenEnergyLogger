#include <stdlib.h>
#include <stdio.h>
#include "user_button.h"

#define FILENAME "/sys/class/gpio/gpio63/value"

int get_user_button(void){
	FILE *fp;
    fp = fopen(FILENAME, "r");
    if(!fp){
        return EXIT_FAILURE;
    }

    char buffer[2];

    buffer[0] = getc(fp);
    buffer[1] = '\0';

    fclose(fp);


	return atoi(buffer);
}
