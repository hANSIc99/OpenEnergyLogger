#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <mraa/gpio.h>
#include <mraa/types.h>
#include <time.h>

#include "user_button.h"
#include "init_gpio.h"
#include "ramdisc.h"
#include "tarif_struct.h"
#include "isr.h"
#include "meter_op.h"
#include "sql.h"
#include "error_exit.h"

#define GPIO_DI0 12
#define GPIO_DI2 10
#define GPIO_LED 13

#define TRUE  1
#define FALSE 0

#define METER_0 0
#define METER_1 1

const char file_meter_0[] = "/home/root/e_meter_0.txt";
const char file_meter_1[] = "/home/root/e_meter_1.txt";
const char file_tarif_state[] = "/dev/shm/tarif_state.txt";
const char file_meter_0_cnt[] = "/dev/shm/meter_0_cnt.txt";
const char file_meter_1_cnt[] = "/dev/shm/meter_1_cnt.txt";
const char file_database[] = "/home/root/e_meter.db";
const char tab_meter_0[] = "meter_0";
const char tab_meter_1[] = "meter_1";


/*
 * USER LED = Tarifanzeige
 * Button = Tarifumschalter
 *
 *  */


tarif_switch tarif_flag;
sqlite3 *db;

/* tarif 0 and 1 and tarif-flag*/
volatile unsigned long long int e_meter_0;
volatile unsigned long long int e_meter_1;



int main(void) {

	fprintf(stdout, "IoT2020 e-meter by Stephan Avenwedde\n");
	fflush(stdout);
	tarif_flag.f_tarif = 0;

	mraa_gpio_context gpio_di0, gpio_di2, gpio_led;

	unsigned long long int last_meter_0;
	unsigned long long int last_meter_1;

	int last_minute = 0;
	int tarif_last_state = 0;
	int b_switch_tarif = FALSE;

	time_t time_raw;
	struct tm *time_info;

	/*
	 * START INIT PROCESS
	 */

	/* INIT DATABASE */

	open_db(&db, file_database);

	/* INIT TABLES */

	create_tables(db);

	/* INIT GPIO */

	init_gpio(&gpio_di0, &gpio_di2, &gpio_led);

	/* INIT RAMDISC (CREATE EMPTY FILES) */

	init_ramdisc();

	/* SET TARIF STATE FILE */

	write_tarif(file_tarif_state, tarif_flag.f_tarif);

	/* SETUP ISR */

	install_isr(&gpio_di0, &isr_signal_tarif_0);
	install_isr(&gpio_di2, &isr_signal_tarif_switch);

	/* INIT METER READINGS */

    e_meter_0 = read_meter_file(file_meter_0);
    e_meter_1 = read_meter_file(file_meter_1);

    last_meter_0 = e_meter_0;
    last_meter_1 = e_meter_1;


    /*
     *  START REGULAR OPERATION (INFINITE LOOP)
     */

    while(1){

    	sleep(2);

    	time(&time_raw);
    	time_info = localtime(&time_raw);

    	/*
    	 * CHECK FOR OTHER INDICATORS FOR TARIF CHANGE
    	 * (when no interrupt was detected)
    	 */

    	if(tarif_flag.b_isr_trig){
    		b_switch_tarif = TRUE;
    	}

    	/* CHECK IF THE USER BUTTON IS PRESSED */

    	if(!get_user_button()){
    		b_switch_tarif = TRUE;
    	}

    	/* CHECK FOR CHANGES IN THE TARIF FILE */

   	   	if(tarif_last_state != read_tarif_file(file_tarif_state)){
   	   		b_switch_tarif = TRUE;
   	   	}

    	/*
    	 * EXECUTE TARIF SWITCH
    	 */

    	if (b_switch_tarif){
    		tarif_flag.f_tarif++;
    		tarif_last_state = tarif_flag.f_tarif;
    		b_switch_tarif = FALSE;

    		fprintf(stdout, "Switch to tarif %u\n", tarif_flag.f_tarif);
    		fflush(stdout);
    		/* switched to tarif 1 */
    		if(tarif_flag.f_tarif){

    	        deinstall_isr(&gpio_di0);
    	        install_isr(&gpio_di0, &isr_signal_tarif_1);
    	        set_led(&gpio_led, TRUE);

    		} else {

    			deinstall_isr(&gpio_di0);
    		    install_isr(&gpio_di0, &isr_signal_tarif_0);
    		    set_led(&gpio_led, FALSE);

    		}

		    /* write new state to tmpfs (dev/shm) */
    		write_tarif(file_tarif_state, tarif_flag.f_tarif);
    	}

    	/*
    	 * WRITE DO HARD DRIVE EVERY MINUTE
    	 */
    	if(last_minute != time_info->tm_min ){

    		last_minute = time_info->tm_min;

    		/*
    		 * DATABSE: INSERT IMPULSES PER MINUTE COUNT
    		 */

        	insert_meter(db, tab_meter_0, time_raw, (e_meter_0 - last_meter_0), METER_0);
        	insert_meter(db, tab_meter_1, time_raw, (e_meter_1 - last_meter_1), METER_1);

        	/*
        	 * CHECK FOR USER INPUT ON BOTH METERS
        	 * IF INPUT FOUND -> WRITE TO VALUE e_meter_x
        	 */

       	   	preset_meter(file_meter_0_cnt, METER_0);
       	   	preset_meter(file_meter_1_cnt, METER_1);

       	   	/*
       	   	 * WRITE ABSOLUTE METERING VALUE
       	   	 */
        	write_metering(file_meter_0, e_meter_0);
        	write_metering(file_meter_1, e_meter_1);

    		fprintf(stdout, "s0_scn: e: e_meter_0: %llu (+%llu)  -  e_meter_1: %llu (+%llu)\n",
    				e_meter_0, (e_meter_0 - last_meter_0),
					e_meter_1, (e_meter_1 - last_meter_1));
    		fflush(stdout);

    		last_meter_0 = e_meter_0;
    		last_meter_1 = e_meter_1;
    	}

    }

	return 0;
}
