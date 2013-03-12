/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 * Asset Management - Loading, unloading, tracking/accounting, memory mgmt
 */

#include "freyja.h"

FRE_OBJECT** objdex;
int objdex_sz;

int fre_objdex_init() {
	objdex = NULL;
	objdex_sz = 0;
	zlog_debug("fre_objdex_init(): Objdex initialized.\n");
	return 0;
}

int fre_objdex_purge() {
	if(objdex) {
		for(int i = 0; i < objdex_sz; i++) {
			if(objdex[i]) free(objdex[i]);
		}
		free(objdex);
		objdex = (FRE_OBJECT**)NULL;
	} else {
		zlog_info("fre_objdex_purge: Nothing to purge. Objdex is uninitialized.\n");
		return 1;
	}
	objdex_sz = 0;
	zlog_info("fre_objdex_purge(): All objects and objdex memory free'd!\n");
	return 0;
}

int fre_objdex_sz() {
	return objdex_sz;
}

int fre_obj_create(int typex, char* objname) {
	int newid = objdex_sz;

	objdex_sz++;
	if((objdex = (FRE_OBJECT**)realloc(objdex, sizeof(FRE_OBJECT*) * objdex_sz)) == NULL) {
		zlog_critical("Failed to reallocate memory for objdex!\n");
		return -1;
	}
	if((objdex[newid] = (FRE_OBJECT*)malloc(sizeof(FRE_OBJECT))) == NULL) {
		zlog_critical("Failed to allocate memory for new object!\n");
		return -1;
	}

	memset(objdex[newid],0,sizeof(FRE_OBJECT));

	objdex[newid]->obj_type = typex;
	if(objname) strcpy(objdex[newid]->objname,objname);

	return newid;
}

int fre_obj_destroy(int iid) {

	if(iid >= objdex_sz) {
		zlog_error("fre_obj_destroy: Invalid object ID referenced!\n");
		return -1;
	}

	if(objdex[iid]) {
		free(objdex[iid]);
	}

	if(objdex_sz > 1 && (iid != (objdex_sz - 1))) {
		objdex[iid] = objdex[objdex_sz - 1];
	}

	objdex_sz--;

	if((objdex = (FRE_OBJECT**)realloc(objdex, sizeof(FRE_OBJECT*) * objdex_sz)) == NULL) {
		zlog_critical("Failed to reallocate memory for objdex!\n");
		return -1;
	}

	return objdex_sz;
}

int fre_obj_find(char* objname) {
	int found = -1;

	for(int i = 0; i < objdex_sz; i++) {
		if(!strcmp(objname,objdex[i]->objname)) {
			found = i;
			break;
		}
	}

	return found;
}

FRE_OBJECT* fre_obj_ptr(int iid) {
	if(iid < objdex_sz) {
		return objdex[iid];
	}
	return NULL;
}