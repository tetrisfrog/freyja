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

// nasty hacky preprocessor stringification macros
#define FRE_STRINGIFY(x) #x
#define FRE_TOSTRING(x) FRE_STRINGIFY(x)

// Verision & Revision definitions
#define FRE_VERSION 		"0.01"
#define FRE_VERSION_MOD		"WIP"
#define FRE_REV_INT			FRE_REV
#define FRE_REV_STR			FRE_TOSTRING(FRE_REV)
#define FRE_VERSION_STRING  FRE_VERSION "." FRE_REV_STR " " FRE_VERSION_MOD
#define FRE_COMPILE_TIME	__DATE__ " " __TIME__


// Max number of content loader modules
#define MAX_LOADS				64

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
 	#define FEXPORTC
 	#define FEXPORT extern
#else
 	#define FEXPORTC extern
 	#define FEXPORT extern
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
FEXPORTC FRE_GLOBAL_CONFIG global_config;

///////////////////////////////////////////////////////////
// core_control.cpp - Core Control
///////////////////////////////////////////////////////////
FEXPORTC int fre_init();
FEXPORTC void fre_shutdown();

///////////////////////////////////////////////////////////
// config_mgmt.cpp - Config Loading/Saving
///////////////////////////////////////////////////////////
FEXPORT int fre_set_val(char* keyval, void* valptr, int vtype);
FEXPORT void* fre_get_val(char* keyval, int* vtype);
FEXPORT char* read_token_string(char* cline);
FEXPORT int fre_load_config(char *cfilename);

///////////////////////////////////////////////////////////
// loadlib.cpp - Dynamic Shared Lib loading
///////////////////////////////////////////////////////////
FEXPORT int fre_lib_load(FRE_LIBDATA* libx);
FEXPORT int fre_lib_unload(FRE_LIBDATA* libx);
FEXPORT void* fre_lib_bind(FRE_LIBDATA* libx, char* libfile);
FEXPORT int fre_lib_bindall(FRE_LIBDATA* libx, FRE_LIBFUNC* bindlist);

///////////////////////////////////////////////////////////
// debug.cpp - Debug & Logging functions
///////////////////////////////////////////////////////////
FEXPORT void logthis(int llevel, char* srcname, int srcline, char* fmt, ...);
FEXPORT char* get_datestr(char* dstrx);
FEXPORT char* get_timestr(char* dstrx);
FEXPORT int log_begin();
FEXPORT int log_end();

///////////////////////////////////////////////////////////
// os_control.cpp
///////////////////////////////////////////////////////////
FEXPORT int fre_os_load_lib(FRE_LIBDATA* libx);
FEXPORT void* fre_os_bind_lib(FRE_LIBDATA* libx, char* funcname);
FEXPORT int fre_os_unload_lib(FRE_LIBDATA* libx);

///////////////////////////////////////////////////////////
// asset_mgmt.cpp
///////////////////////////////////////////////////////////
FEXPORT int fre_objdex_init();
FEXPORT int fre_obj_create(int typex, char* objname);
FEXPORT int fre_obj_destroy(int iid);
FEXPORT int fre_obj_find(char* objname);
FEXPORT FRE_OBJECT* fre_obj_ptr(int iid);

///////////////////////////////////////////////////////////
// asset_mgmt.cpp
///////////////////////////////////////////////////////////
FEXPORT int frl_register_loader(char* modname);

#endif


// Logging macros
#define zlog_debug(a...)	logthis(9,__FILE__,__LINE__,a)
#define zlog_info(a...)		logthis(2,__FILE__,__LINE__,a)
#define zlog_warn(a...)		logthis(1,__FILE__,__LINE__,a)
#define zlog_error(a...)	logthis(0,__FILE__,__LINE__,a)
#define zlog_critical(a...)	logthis(-1,__FILE__,__LINE__,a)

#include "loaders/loaders.h"
#include "render_ogl/render.h"