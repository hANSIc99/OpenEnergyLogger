#include "ramdisc.h"
#include "error_exit.h"

#include <stdlib.h>
#include <stdio.h>

	extern const char file_tarif_state[];
	extern const char file_meter_0_cnt[];
	extern const char file_meter_1_cnt[];

void init_ramdisc(void){

	FILE *fp;
	char err_msg[100];

	/*
	 * CREATE FILE ON RAMDISC FOR TARIF STATE SHARING
	 */
    fp = fopen(file_tarif_state, "w");
    if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to create", file_tarif_state);
		file_error(err_msg);
    }
    fclose(fp);

	/*
	 * CREATE FILE ON RAMDISC FOR TARIF 0 METERING VALUE
	 */
    fp = fopen(file_meter_0_cnt, "w");
    if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to create", file_meter_0_cnt);
		file_error(err_msg);
    }
    fclose(fp);

	/*
	 * CREATE FILE ON RAMDISC FOR TARIF 1 METERING VALUE
	 */
    fp = fopen(file_meter_1_cnt, "w");
    if(!fp){
		sprintf(err_msg, "%s() - %s %s\n", __func__, "failed to create", file_meter_1_cnt);
		file_error(err_msg);
    }
    fclose(fp);

}

