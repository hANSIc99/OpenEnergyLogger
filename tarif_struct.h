#ifndef __tarif_struct_h
#define __tarif_struct_h

typedef struct {
	volatile unsigned int f_tarif 	 : 1;
	volatile unsigned int b_isr_trig : 1;
	volatile unsigned int         	 : 6;
} tarif_switch;

#endif
