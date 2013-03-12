/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 * Library Loader - loadlib
 *
 */


#include "freyja.h"

/*
 * Load Shared Library
 *
 * Loads a shared library into memory and prepares it for
 * binding of functions.
 *
 * int fre_lib_load()
 *
 */
 int fre_lib_load(FRE_LIBDATA* libx) {

 	zlog_debug("fre_lib_load: Loading platform-specific lib loader...\n");
 	fre_os_load_lib(libx);
 	return 0;
 }

/*
 * Unload Shared Library
 *
 * Unloads a shared library from memory. All bound functions will no longer work.
 *
 * int fre_lib_unload()
 *
 */
 int fre_lib_unload(FRE_LIBDATA* libx) {

 	zlog_debug("fre_lib_unload: Loading platform-specific lib unloader...\n");
 	fre_os_unload_lib(libx);
 	return 0;
 }

 /*
 * Bind Library Functions
 *
 * Binds shared lib functions to proper function pointers.
 *
 * int fre_lib_bind()
 *
 */
 void* fre_lib_bind(FRE_LIBDATA* libx, char* libfile) {
 	void* fvec;

	zlog_debug("fre_lib_bind: Binding [%s] from lib [%s].\n",libfile,libx->libname);
 	if((fvec = fre_os_bind_lib(libx, libfile)) == NULL) {
 		zlog_critical("CRITICAL: fre_lib_bind: Failed to bind!\n");
 		return NULL;
 	}

	return fvec;
 }

 /*
 * Bind All Functions for Library
 *
 * Binds to all enumerated library functions.
 *
 * int fre_lib_bindall()
 *
 */
 int fre_lib_bindall(FRE_LIBDATA* libx, FRE_LIBFUNC* bindlist) {
 	void* fvec;
 	int bmax = 1024; // max functions
 	int fload = 0;

	zlog_debug("fre_lib_bindall: Binding all functions for [%s]...\n",libx->libname);

	for(int i = 0; i < bmax; i++) {
		if(bindlist[i].funcname[0] == 0 || bindlist[i].funcvec == NULL) break;
		if((fvec = fre_lib_bind(libx,bindlist[i].funcname)) == NULL) {
			zlog_error("fre_lib_bindall: Failed to bind to \"%s\"!\n",bindlist[i].funcname);
			return 0;
		}
		(*bindlist[i].funcvec) = fvec;
		fload++;
	}

	return fload;
 }
 