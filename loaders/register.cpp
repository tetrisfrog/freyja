/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 22 Mar 2013
 *
 * Content Loaders - Loader Registration & Handling
 */


#include "freyja.h"

FRL_LOADER_DEF loaddex[MAX_LOADS];
int loaddex_sz = 0;

/*
typedef struct {
	char funcname[128];
	void** funcvec;
} FRE_LIBFUNC;

typedef struct {
	char filename[256];
	char libname[32];
	void* libvector;
	int status;
} FRE_LIBDATA;

typedef struct {
	int id;
	char modname[128];
	int  modtype;
	FRE_LIBDATA libdata;
	FRL_LOADER_INIT    loader_init;
	FRL_LOADER_QUERY   loader_query_types;
	FRL_FUNC_FROM_FILE decode_from_file;
	FRL_FUNC_FROM_MEM  decode_from_mem;
	FRL_FUNC_TO_FILE   encode_to_file;
	FRL_FUNC_TO_MEM    encode_to_mem;
} FRL_LOADER_DEF;

typedef struct {
	int version_id;
	void (*logthis_ptr)(int llevel, char* srcname, int srcline, char* fmt, ...);
	void (*fre_shutdown_ptr)();
} FRE_RENDER_INIT_DATA;
*/

int frl_register_loader(char* modname) {

	FRL_LOADER_DEF* modlib = &loaddex[loaddex_sz];

	// erase block of mem in case we had a previous botched attempt
 	memset(modlib,0,sizeof(FRL_LOADER_DEF));

 	zlog_info("frl_register_loader: Registering content loader \"%s\"...\n", modname);

 	// Set initial params
 	sprintf(modlib->libdata.filename,"loaders/%s/%s.so",modname,modname);
 	strcpy(modlib->libdata.libname,modname);
 	modlib->libdata.status = FRE_STATUS_FAIL;
 	modlib->id = loaddex_sz;

 	// Load shared lib
 	fre_lib_load(&modlib->libdata);
 	if(modlib->libdata.status == FRE_STATUS_FAIL) {
 		zlog_error("Failed to bind to content loader!\n");
 		return FRE_STATUS_FAIL;
 	}

 	// Bind to functions...

 	// loader_init() - *REQUIRED*
 	zlog_debug("frl_register_loader: Binding to loader_init()...\n");
 	if((modlib->loader_init = (FRL_LOADER_INIT)fre_lib_bind(&modlib->libdata, "loader_init")) == NULL) {
 		zlog_error("Failed to bind to loader_init() function! Aborting loader registration.\n");
 		return FRE_STATUS_FAIL;
 	}

 	// loader_query_types() - *REQUIRED*
 	zlog_debug("frl_register_loader: Binding to loader_query_types()...\n");
 	if((modlib->loader_query_types = (FRL_LOADER_QUERY)fre_lib_bind(&modlib->libdata, "loader_query_types")) == NULL) {
 		zlog_error("Failed to bind to loader_query_types() function! Aborting loader registration.\n");
 		return FRE_STATUS_FAIL;
 	}

 	// decode_from_file() - OPTIONAL
 	zlog_debug("frl_register_loader: Binding to decode_from_file()...\n");
 	if((modlib->decode_from_file = (FRL_FUNC_FROM_FILE)fre_lib_bind(&modlib->libdata, "decode_from_file")) == NULL) {
 		// FAILED	
 	} 

 	// decode_from_mem() - OPTIONAL
 	zlog_debug("frl_register_loader: Binding to decode_from_mem()...\n");
 	if((modlib->decode_from_mem = (FRL_FUNC_FROM_MEM)fre_lib_bind(&modlib->libdata, "decode_from_mem")) == NULL) {
 		// FAILED
 	} 

 	// encode_to_file() - OPTIONAL
 	zlog_debug("frl_register_loader: Binding to encode_to_file()...\n");
 	if((modlib->encode_to_file = (FRL_FUNC_TO_FILE)fre_lib_bind(&modlib->libdata, "encode_to_file")) == NULL) {
 		// FAILED
 	} 

  	// encode_to_mem() - OPTIONAL
 	zlog_debug("frl_register_loader: Binding to encode_to_mem()...\n");
 	if((modlib->encode_to_mem = (FRL_FUNC_TO_MEM)fre_lib_bind(&modlib->libdata, "encode_to_mem")) == NULL) {
 		// FAILED
 	} 

 	// Now initialize the loader...
 	FRE_RENDER_INIT_DATA rconfig;

 	rconfig.version_id = BUILD_DCODE;
	rconfig.logthis_ptr = logthis;
	rconfig.fre_shutdown_ptr = fre_shutdown;

	// Run loader_init() and store module type
 	modlib->modtype = modlib->loader_init(&rconfig);

 	loaddex_sz++;
}

