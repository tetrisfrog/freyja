/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 * Content Loaders - Header
 */

#ifndef __LOADERS_H
#define __LOADERS_H

// Module/Loader Types

#define FRL_TYPE_IMAGE			1
#define FRL_TYPE_OBJECT			2
#define FRL_TYPE_GEO			3
#define FRL_TYPE_BRUSH			4
#define FRL_TYPE_SCRIPT			5
#define FRL_TYPE_SPECIAL		6


//#define FRL_DSPEC __cdecl
#define FRL_DSPEC

typedef int (FRL_DSPEC * FRL_LOADER_INIT)(FRE_RENDER_INIT_DATA* rdata);
typedef int (FRL_DSPEC * FRL_LOADER_QUERY)(char* outstr, int maxsz);
typedef int (FRL_DSPEC * FRL_FUNC_FROM_FILE)(FILE* src_file, void* dest_mem, int d_sz, int params);
typedef int (FRL_DSPEC * FRL_FUNC_FROM_MEM)(void* src_mem, void* dest_mem, int s_sz, int d_sz, int params);
typedef int (FRL_DSPEC * FRL_FUNC_TO_FILE)(FILE* dest_file, void* src_mem, int s_sz, int params);
typedef int (FRL_DSPEC * FRL_FUNC_TO_MEM)(void* dest_mem, void* src_mem, int s_sz, int d_sz, int params);

typedef struct {
	long height, width, planes, depth;
	int  type;
	char *data_ptr;
} FRL_IMG_DATA;

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


#endif /* __LOADERS_H */