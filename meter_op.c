#include <stdio.h>
#include <stdlib.h>
#include "meter_op.h"
#include "error_exit.h"

extern volatile unsigned long long int e_meter_0;
extern volatile unsigned long long int e_meter_1;


unsigned long long int read_meter_file(const char* filename){

	FILE *fp;
	int ch;
	char err_msg[50];
	char buffer[21];
	unsigned int buflen = sizeof(buffer);
	char *end = buffer + buflen -1;
	char *dst = buffer;

	fp = fopen(filename, "a+");
	 if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to open", filename);
		file_error(err_msg);
	}

		while((ch = getc(fp)) != EOF && ch != '\n' && dst < end){
			*dst++ = ch;
			/* fprintf(stdout, "char: %d\n", ch); */
		}
		fclose(fp);

		*dst = '\0';

		/* EOF = -1 */
		/* first condition: empty file */

	/* chars_read = ((ch == EOF && dst == buffer) ? EOF : dst - buffer); */

	fprintf(stdout, "s0_scn: saved value %s: %s\n", filename, buffer);
	fflush(stdout);
	return strtoll(buffer, (char**)NULL, 10);
}


int read_tarif_file(const char* filename){

	FILE *fp;
	char err_msg[50];
	int ch;
	char buffer[21];
	unsigned int buflen = sizeof(buffer);
	char *end = buffer + buflen -1;
	char *dst = buffer;

	fp = fopen(filename, "a+");
	if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to read", filename);
		file_error(err_msg);
	}
	   	while((ch = getc(fp)) != EOF && ch != '\n' && dst < end){
	   		*dst++ = ch;
	   		/* fprintf(stdout, "char: %d\n", ch); */
	   	}
	   	fclose(fp);
	   	*dst = '\0';

	   	return atoi(buffer);
}

void write_metering(const char* filename, long long unsigned int meter_reading){

	FILE *fp;
	char err_msg[50];

	fp = fopen(filename, "w+");
	if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to write to", filename);
		file_error(err_msg);
	}
	fprintf(fp, "%llu\n", meter_reading);
	fclose(fp);

}

void preset_meter(const char* filename, int meter){

	FILE *fp;
	int ch;
	char buffer[21];
	char err_msg[50];
	unsigned int buflen = sizeof(buffer);
	char *end = buffer + buflen -1;
	char *dst = buffer;


	fp = fopen(filename, "r+");
	if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to open", filename);
		file_error(err_msg);
	}
	while((ch = getc(fp)) != EOF && ch != '\n' && dst < end){
		*dst++ = ch;
	}

	fclose(fp);
	*dst = '\0';

	if(buffer[0] != '\0'){

		if(meter){
			e_meter_1 = strtoll(buffer, (char**)NULL, 10);
			fprintf(stdout, "New value for meter_1 found %llu\n", e_meter_1);
		} else {
			e_meter_0 = strtoll(buffer, (char**)NULL, 10);
			fprintf(stdout, "New value for meter_0 found %llu\n", e_meter_0);
		}
		fflush(stdout);

		/* clear file */
		fp = fopen(filename, "w");
		if(!fp){
			sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to clear file", filename);
			file_error(err_msg);
		}
		fclose(fp);
	}
}

void write_tarif(const char* filename, unsigned int tarif){

	FILE *fp;
	char err_msg[50];

    fp = fopen(filename, "w+");
    if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to open file", filename);
		file_error(err_msg);
    }
	fprintf(fp, "%u\n", tarif);
	fclose(fp);
}
