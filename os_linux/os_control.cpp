/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 * OS Control - Linux
 *
 */


#include "freyja.h"
#include "os_linux/platform_linux.h"
#include <dlfcn.h>


 int fre_os_load_lib(FRE_LIBDATA* libx) {

 	zlog_debug("fre_os_load_lib: Loading shared lib [%s] from file [%s]...\n",libx->libname,libx->filename);

 	if((libx->libvector = dlopen(libx->filename,RTLD_LAZY)) ==  NULL) {
 		zlog_critical("CRITICAL: Error loading [%s] shared library \"%s\" !\n",libx->libname,libx->filename);
 		zlog_critical("          dlerror() reports: %s\n",dlerror());
 		libx->status = FRE_STATUS_FAIL;
 		return FRE_STATUS_FAIL;
 	}

 	libx->status = FRE_STATUS_OK;
 	zlog_debug("fre_os_load_lib: Loaded [%s] successfully. Vector = 0x%08X\n",libx->libname,libx->libvector);

 	return FRE_STATUS_OK;
 }

void* fre_os_bind_lib(FRE_LIBDATA* libx, char* funcname) {
	char* errorptr;
	void* funcvec;

	funcvec = dlsym(libx->libvector,funcname);
	if((errorptr = dlerror()) != NULL) {
		zlog_error("fre_os_bind_lib: dlsym() failed to bind \"%a\" from [%s].\n",funcname, libx->libname);
		zlog_error("fre_os_bind_lib: %s\n",errorptr);
		return NULL;
	}

	return funcvec;
}

 int fre_os_unload_lib(FRE_LIBDATA* libx) {

 	zlog_debug("fre_os_unload_lib: Unloading shared lib [%s]...\n",libx->libname);

 	if(libx->libvector || libx->status == FRE_STATUS_OK) {
 		dlclose(libx->libvector);
 		libx->status = FRE_STATUS_NG;
 		zlog_debug("fre_os_unload_lib: Unloaded successfully!\n");
 	} else {
 		libx->status = FRE_STATUS_NG;
 		zlog_debug("fre_os_unload_lib: Unload failed.\n");
 	}

 	return FRE_STATUS_NG;
 }