#ifndef __sql_h
#define __sql_h

#include <sqlite3.h>
#include <time.h>

int write_sql(sqlite3 *db, int* tarif);
void insert_meter(sqlite3 *db, const char *table, time_t ts, unsigned long long int val, int meter);
void create_tables(sqlite3 *db);
void open_db(sqlite3 **db, const char *filename);

#endif
