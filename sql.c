#include "sql.h"
#include "error_exit.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static const char *sql_meter_0 = "CREATE TABLE IF NOT EXISTS meter_0 "
			"(timestamp INTEGER PRIMARY KEY NOT NULL, value UNSIGNED BIG INT);";

static const char *sql_meter_1 = "CREATE TABLE IF NOT EXISTS meter_1 "
			"(timestamp INTEGER PRIMARY KEY NOT NULL, value UNSIGNED BIG INT);";



static const char *sql_insert_0 = "INSERT INTO meter_0 VALUES (?, ?);";
static const char *sql_insert_1 = "INSERT INTO meter_1 VALUES (?, ?);";


void create_tables(sqlite3 *db){

	int rc;
	char err_msg[100];
	sqlite3_stmt *db_ret;

	rc = sqlite3_prepare_v2(db, sql_meter_1, -1, &db_ret, 0);

    if (rc != SQLITE_OK) {
		sprintf(err_msg, "%s() - %s %s\n", __func__, "prepare failed:", sqlite3_errmsg(db));
		sqlite_error(err_msg);
    }

    rc = sqlite3_step(db_ret);

    if (rc != SQLITE_DONE) {
		sprintf(err_msg, "%s() - %s%d - %s\n", __func__, "step failed, rc = ", rc, sqlite3_errmsg(db));
		sqlite_error(err_msg);
    }

    sqlite3_reset(db_ret);

    rc = sqlite3_prepare_v2(db, sql_meter_0, -1, &db_ret, 0);

    if (rc != SQLITE_OK) {
		sprintf(err_msg, "%s() - %s %s\n", __func__, "prepare failed:", sqlite3_errmsg(db));
		sqlite_error(err_msg);
    }

    rc = sqlite3_step(db_ret);

    if (rc != SQLITE_DONE) {
		sprintf(err_msg, "%s() - %s%d - %s\n", __func__, "step failed, rc = ", rc, sqlite3_errmsg(db));
		sqlite_error(err_msg);
    }

    sqlite3_reset(db_ret);
    sqlite3_finalize(db_ret);

}

void insert_meter(sqlite3 *db, const char *table, time_t ts, unsigned long long int val, int meter){

	int rc;
	char err_msg[100];
	const char *query;
	sqlite3_stmt *db_ret;

	query = meter ? sql_insert_1 : sql_insert_0;


	rc = sqlite3_prepare_v2(db, query, -1, &db_ret, 0);

    if (rc != SQLITE_OK) {
		sprintf(err_msg, "%s() - %s%d - %s\n", __func__, "prepare failed for table meter_", meter, sqlite3_errmsg(db));
		sqlite_error(err_msg);
    }

    sqlite3_bind_int64(db_ret, 1, ts);
    sqlite3_bind_int64(db_ret, 2, val);

    /* fprintf(stdout, "meter: %d table: %s ts: %lu  value: %llu\n", meter, table, ts, val); */

    rc = sqlite3_step(db_ret);

    if (rc != SQLITE_DONE) {
    	sprintf(err_msg, "%s() - %s%d - %s\n", __func__, "insert failed for table meter_", meter, sqlite3_errmsg(db));
		sqlite_error(err_msg);
    }

    sqlite3_reset(db_ret);
    sqlite3_finalize(db_ret);

}

void open_db(sqlite3 **db, const char *filename){

	char err_msg[100];

	int rc = sqlite3_open(filename, db);

    if(rc != SQLITE_OK) {
		sprintf(err_msg, "%s() - %s %s\n", __func__, "cannot open database", sqlite3_errmsg(*db));
		sqlite_error(err_msg);
    }
}
