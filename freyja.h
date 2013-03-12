/*
 *
 * Freyja Engine - Main Header
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <malloc.h>

#define FRE_VERSION_STRING "0.01 WIP"
#define FRE_COMPILE_TIME	__DATE__ " " __TIME__

// Constant defines
#define FRE_PLATFORM_LINUX		1
#define FRE_PLATFORM_WIN32		2
#define FRE_PLATFORM_MAC		3

#define FRE_STATUS_NG			0
#define FRE_STATUS_OK			1
#define FRE_STATUS_FAIL			2

#define FRE_OBJ_TEXTURE			1
#define FRE_OBJ_AUDIO			2
#define FRE_OBJ_MODEL			3

// Target platform detection
#ifdef _WIN32
 	#define FRE_PLATFORM FRE_PLATFORM_WIN32
 	#define FRE_PLATFORM_STRING "win32"
#endif

#ifdef __APPLE__
 	#define FRE_PLATFORM FRE_PLATFORM_MAC
 	#define FRE_PLATFORM_STRING "mac"
#endif

#ifdef __linux__
 	#define FRE_PLATFORM FRE_PLATFORM_LINUX
 	#define FRE_PLATFORM_STRING "linux"
#endif

// Superglobal exports
#ifdef FRE_CORE_CONTROL
 	#define ZEXPORTC
 	#define ZEXPORT extern
#else
 	#define ZEXPORTC extern
 	#define ZEXPORT extern
#endif

// Typedefs

typedef struct {
	int loglevel;
	int trace_enable;
	int log_echo;
	int logwrite;
	int log_tstamp;
	int log_append;
	char logname[256];
	int platform;
} FRE_GLOBAL_CONFIG; 

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
	int version_id;
	void (*logthis_ptr)(int llevel, char* srcname, int srcline, char* fmt, ...);
	void (*fre_shutdown_ptr)();
} FRE_RENDER_INIT_DATA;

typedef struct {
	int mode_ctl;
	int screen_x;
	int screen_y;
	int fullscreen;
} FRE_RENDER_MODE;

typedef struct {
	int obj_type;
	char objname[128];
	int data_sz;
	void* objdata;
} FRE_OBJECT;

#ifdef FREYJA_CORE
// Superglobal definitions
ZEXPORTC FRE_GLOBAL_CONFIG global_config;

///////////////////////////////////////////////////////////
// core_control.cpp - Core Control
///////////////////////////////////////////////////////////
ZEXPORTC int fre_init();

///////////////////////////////////////////////////////////
// config_mgmt.cpp - Config Loading/Saving
///////////////////////////////////////////////////////////
ZEXPORT int fre_set_val(char* keyval, void* valptr, int vtype);
ZEXPORT void* fre_get_val(char* keyval, int* vtype);
ZEXPORT char* read_token_string(char* cline);
ZEXPORT int fre_load_config(char *cfilename);

///////////////////////////////////////////////////////////
// loadlib.cpp - Dynamic Shared Lib loading
///////////////////////////////////////////////////////////
ZEXPORT int fre_lib_load(FRE_LIBDATA* libx);
ZEXPORT int fre_lib_unload(FRE_LIBDATA* libx);
ZEXPORT void* fre_lib_bind(FRE_LIBDATA* libx, char* libfile);
ZEXPORT int fre_lib_bindall(FRE_LIBDATA* libx, FRE_LIBFUNC* bindlist);

///////////////////////////////////////////////////////////
// debug.cpp - Debug & Logging functions
///////////////////////////////////////////////////////////
ZEXPORT void logthis(int llevel, char* srcname, int srcline, char* fmt, ...);
ZEXPORT char* get_datestr(char* dstrx);
ZEXPORT char* get_timestr(char* dstrx);
ZEXPORT int log_begin();
ZEXPORT int log_end();

///////////////////////////////////////////////////////////
// os_control.cpp
///////////////////////////////////////////////////////////
ZEXPORT int fre_os_load_lib(FRE_LIBDATA* libx);
ZEXPORT void* fre_os_bind_lib(FRE_LIBDATA* libx, char* funcname);
ZEXPORT int fre_os_unload_lib(FRE_LIBDATA* libx);

///////////////////////////////////////////////////////////
// asset_mgmt.cpp
///////////////////////////////////////////////////////////
ZEXPORT int fre_objdex_init();
ZEXPORT int fre_obj_create(int typex, char* objname);
ZEXPORT int fre_obj_destroy(int iid);
ZEXPORT int fre_obj_find(char* objname);
ZEXPORT FRE_OBJECT* fre_obj_ptr(int iid);

#endif


// Logging macros
#define zlog_debug(a...)	logthis(9,__FILE__,__LINE__,a)
#define zlog_info(a...)		logthis(2,__FILE__,__LINE__,a)
#define zlog_warn(a...)		logthis(1,__FILE__,__LINE__,a)
#define zlog_error(a...)	logthis(0,__FILE__,__LINE__,a)
#define zlog_critical(a...)	logthis(-1,__FILE__,__LINE__,a)

#include "loaders/loaders.h"
#include "render_ogl/render.h"