#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <time.h>
#include "libpafe.h"

const static char* host = "localhost";
const static char* user = "pafe";
const static char* pass = "pafe";
const static char* db = "pafe";

static time_t playedtime = 0;

static void playsound(void);
static void _writedb(char* idm, char* pmm);
static void _log(char* log);

#ifdef	DEBUG
#undef	DEBUG
#endif
#define DEBUG 4

void formatid(char *buf, uint8 *p) {
	sprintf(buf, "%02X%02X%02X%02X%02X%02X%02X%02X",
		*p++, *p++, *p++, *p++, *p++, *p++, *p++, *p++);
}

int main(void) {
	char _idm[20], _pmm[20];
	while(1) {
_log("BEGIN PASORI OPEN\n");
		pasori *p = pasori_open();
_log("END OPEN OK\n");
		if (!p) {
			sleep(1);
			printf("pasoriopen err\n");
			continue;
		}

		// S320は無くても動作可 S370はpasori_init必要
		pasori_init(p);

_log("BEGIN FELICA_POLLING\n");
		felica *f = felica_polling(p, FELICA_POLLING_ANY, 0, 0);
_log("END FELICA_POLLING\n");

		if (f) {
			char idm[20], pmm[20];
			formatid(idm, f->IDm);
			formatid(pmm, f->PMm);
			if (strncmp(_idm, idm, 16) || strncmp(_pmm, pmm, 16)) {
				_writedb(idm, pmm);
				free(f);
				strcpy(_idm, idm);
				strcpy(_pmm, pmm);
				printf("%s:%s\n", idm, pmm);
			}
			playsound();
		}
_log("BEGIN PASORI CLOSE\n");
		pasori_close(p);
_log("END PASORI CLOSE\n");
	}
	return 0;
}

void getname(char* idm, char* pmm, char* name) {
	
	MYSQL *mysql = mysql_init(NULL);
	if (!mysql) {
		printf("mysql init error\n");
		exit(-1);
	}
	
	if (!mysql_real_connect(mysql, host, user, pass, db, 0, NULL, 0)) {
		//printf(stderr, "Failed to connect to database %s\n", (char*)mysql_error(mysql));
		printf("Failed to connect to database %s\n", (char*)mysql_error(mysql));
	}
	
	char query[128];
	sprintf(query, "select name from user where idm = '%s' and pmm = '%s'", idm, pmm);
	int r = mysql_query(mysql, query);
	if (r) {
		printf("error: %s\n", (char*)mysql_error(mysql));
	} else {
		MYSQL_ROW record;
		MYSQL_RES *result = mysql_store_result(mysql);
		if (result) {
			record = mysql_fetch_row(result);
			strcpy(name, record[0]);
		}
	}
	mysql_close(mysql);
	
}

static void _writedb(char* idm, char* pmm) {
	char date[20] = "\n";

	//char name[64] = "\n";
	//getname(idm, pmm, name);

	MYSQL *mysql = mysql_init(NULL);
	if (!mysql) {
		printf("mysql init error\n");
		exit(-1);
	}

	time_t timer;
	struct tm *t;
	time(&timer);
	t = localtime(&timer);
	sprintf(date, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	if (!mysql_real_connect(mysql, host, user, pass, db, 0, NULL, 0)) {
		//printf(stderr, "Failed to connect to database %s\n", (char*)mysql_error(mysql));
		printf("Failed to connect to database %s\n", (char*)mysql_error(mysql));
	}

	char query[128];
	sprintf(query,  "insert into data (idm, pmm, create_t) values"
			" ('%s', '%s', '%s')", idm, pmm, date);
	int r = mysql_query(mysql, query);
	if (r) {
		printf("error: %s\n", (char*)mysql_error(mysql));
	}
		
	mysql_close(mysql);
}

static void playsound(void) {
	time_t timer;
	time(&timer);

//@@@ (version2)10分から45分の間であれば、音を変える
//@@@ version1では、集計時に判別

	if (playedtime + 1 < timer) {
		//@@@ (varsion1)わぉーんに変える
		//const char* cmd = "aplay /usr/share/scratch/Media/Sounds/Animal/Dog1.wav";
		const char* cmd = "aplay /usr/share/scratch/Media/Sounds/Animal/Dog2.wav";
		system(cmd);
		playedtime = timer;
	}
}

static void _log(char* message) {
#if	DEBUG
	//printf(message);
#endif
}
