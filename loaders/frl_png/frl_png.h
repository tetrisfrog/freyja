/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 22 Mar 2013
 *
 * Content Loaders - Core - PNG Loader - Header
 *
 */


#include <png.h>

// Loader definition
#define LOADER_MODULE_TYPE			FRL_TYPE_IMAGE
#define LOADER_MODULE_NAME			"Freyja Core - PNG"
#define LOADER_MODULE_VENDOR		"J.Hipps/Neo-Retro Group"
#define LOADER_MODULE_VER_MAJOR		1
#define LOADER_MODULE_VER_MINOR		0
#define LOADER_MODULE_VER_STR		FRE_TOSTRING(LOADER_MODULE_VER_MAJOR) "." FRE_TOSTRING(LOADER_MODULE_VER_MINOR)
#define LOADER_MODULE_COMPILED		__DATE__ " " __TIME__

// Extern defines
#ifdef ZLOADER_MAIN
 	#define FLEXTERN
#else
 	#define FLEXTERN extern
#endif

// Core callbacks
FLEXTERN void (*logthis)(int llevel, char* srcname, int srcline, char* fmt, ...);
FLEXTERN void (*fre_shutdown)();

// Exports
extern "C" {
	int loader_init(FRE_RENDER_INIT_DATA* rdata);
	int loader_query_types(char* outstr, int maxsz);
	int decode_from_file(FILE* src_file, void* dest_mem, int d_sz, int params);
	int decode_from_mem(void* src_mem, void* dest_mem, int s_sz, int d_sz, int params);
	int encode_to_file(FILE* dest_file, void* src_mem, int s_sz, int params);
	int encode_to_mem(void* dest_mem, void* src_mem, int s_sz, int d_sz, int params);
}