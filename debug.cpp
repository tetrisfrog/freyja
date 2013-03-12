/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 * Debug & Logging Functions
 *
 */

#include "freyja.h"

#define LOGSTAT_NG 0
#define LOGSTAT_OK 1

char monthx[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
char dayx[][4]   = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};

FILE *log_hand = NULL;
int log_status = LOGSTAT_NG;

 void logthis(int llevel, char* srcname, int srcline, char* fmt, ...) {
	char texbuf[4096];
	char logbuf[4096];
	char trace_buf[256];
	char ts_buf[128];

	va_list ap;
	va_start(ap, fmt);
	vsprintf(texbuf, fmt, ap);
	va_end(ap);

	if(global_config.loglevel >= llevel) {
		// Prefix loglevel, source file, and line number if enabled
		if(global_config.trace_enable) sprintf(trace_buf,"<%i>[%s, Line %4i] ",llevel,srcname,srcline);
		else trace_buf[0] = 0;

		// Generate timestamp
		if(global_config.log_tstamp) sprintf(ts_buf,"");
		else ts_buf[0] = 0;

		// Print the message
		if(global_config.log_echo) printf("%s%s%s",ts_buf,trace_buf,texbuf);

		// And log to file...
		if(global_config.logwrite) {
			// logfile write
		}
	}

}

char* get_datestr(char* dstrx) {
	time_t ltime;
	struct tm* ltx;
	static char tstime[64];
	char* outptr;

	if(!dstrx) outptr = tstime;
	else outptr = dstrx;

	// get time and set 'tm' struct
	ltime = time(NULL);
	ltx = localtime(&ltime);

	sprintf(tstime,"%s, %i %s %i",dayx[ltx->tm_wday], ltx->tm_mday, monthx[ltx->tm_mon], ltx->tm_year + 1900);

	return outptr;
}

char* get_timestr(char* dstrx) {
	time_t ltime;
	struct tm* ltx;
	static char tstime[64];
	char* outptr;

	if(!dstrx) outptr = tstime;
	else outptr = dstrx;

	// get time and set 'tm' struct
	ltime = time(NULL);
	ltx = localtime(&ltime);

	sprintf(tstime,"%i:%i:%i",ltx->tm_hour,ltx->tm_min,ltx->tm_sec);

	return outptr;
}


int log_begin() {
	char log_fmode[3];

	// setup fopen() mode string
	if(global_config.log_append) strcpy(log_fmode,"w+");
	else strcpy(log_fmode,"w");

	if((log_hand = fopen(global_config.logname,log_fmode)) == NULL) {
		zlog_error("Failed to open log file - \"%s\"\n",global_config.logname);
		return LOGSTAT_NG;
	}

	log_status = LOGSTAT_OK;

	zlog_info("Log file opened successfully on %s @ %s\n",get_datestr(NULL),get_timestr(NULL));

	return LOGSTAT_OK;
}

int log_end() {

	if(log_hand && log_status == LOGSTAT_OK) {
		zlog_info("Log file closed on %s @ %s\n",get_datestr(NULL),get_timestr(NULL));
		fclose(log_hand);
	}

	log_status = LOGSTAT_NG;

	return LOGSTAT_NG;
}