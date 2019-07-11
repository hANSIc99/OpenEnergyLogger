#ifndef __meter_op_h
#define __meter_op_h

unsigned long long int read_meter_file(const char* filename);
void preset_meter(const char* filename, int meter);
void write_metering(const char* filename, long long unsigned int meter_reading);
int read_tarif_file(const char* filename);

void write_tarif(const char* filename, unsigned int tarif);

#endif
