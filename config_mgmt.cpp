/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 * Config Management - Load/Save Config structures
 *
 */

 #include "freyja.h"

int fre_set_val(char* keyval, void* valptr, int vtype) {

}

void* fre_get_val(char* keyval, int* vtype) {

}

char* read_token_string(char* cline) {
	if(!cline) return NULL;
	int llen = strlen(cline);

}

 int fre_load_config(char *cfilename) {
 	FILE *cfhand = NULL;
 	char linebuf[256];

 	if((cfhand = fopen(cfilename,"rb")) == NULL) {
 		zlog_error("Failed to load config file \"%s\"\n",cfilename);
 		return -1;
 	}

 	while(!feof(cfhand)) {
 		if(fgets(linebuf,256,cfhand) == NULL) break;
 		read_token_string(linebuf);
 	}

 	fclose(cfhand);
 	return 0;
 }