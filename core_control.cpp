/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 02 Mar 2013
 *
 * Core Control
 *
 */

#define FRE_CORE_CONTROL

#include "freyja.h"

// Globals
FRE_LIBDATA lib_render;

// Functions for automatic dynamic linker
FRE_LIBFUNC render_funcs[] = {
	{"frz_init",			(void**)&frz_init },
	{"frz_gfx_init",		(void**)&frz_gfx_init },
	{"frz_gfx_kill",		(void**)&frz_gfx_kill },
	{"frz_gfx_update",		(void**)&frz_gfx_update },
	{"frz_context_create",	(void**)&frz_context_create },
	{"frz_context_ptr",		(void**)&frz_context_ptr },
	{NULL,					NULL}
};


/*
 * Freyja Initialize
 *
 * Must be called in order to initialize and start up the Freyja Engine
 * and use any other functions
 *
 * int fre_init()
 *
 */
 int fre_init() {

 	// Global Config
	global_config.loglevel = 9;
	global_config.trace_enable = 1;
	global_config.log_echo = 1;
	global_config.logwrite = 0;
 	global_config.platform = FRE_PLATFORM;

 	zlog_info("** Freyja Engine\n");
 	zlog_info("** Version %s - %s - Build time %s\n",FRE_VERSION_STRING,FRE_PLATFORM_STRING,FRE_COMPILE_TIME);

 	// Asset/Object Management
 	fre_objdex_init();
 	zlog_info("Asset management initialized.\n");

 	// Rendering lib
 	strcpy(lib_render.filename,"render_ogl/render_ogl.so");
 	strcpy(lib_render.libname,"render_ogl");
 	
 	// Bind to renderer lib
 	int func_binds = 0;
 	fre_lib_load(&lib_render);
 	func_binds = fre_lib_bindall(&lib_render, render_funcs);
 	zlog_debug("Bound to %i functions!\n",func_binds);

 	return 0;
 }

 void fre_shutdown() {
 	fre_lib_unload(&lib_render);
 }



 int main() {

 	int init_rval;
 	int context;
	FRE_RENDER_MODE rendset;
	FRE_RENDER_INIT_DATA render_config;

	render_config.logthis_ptr = logthis;
	render_config.fre_shutdown_ptr = fre_shutdown;

	fre_init();

 	init_rval = frz_init(&render_config);
 	zlog_debug("init_rval = %i\n",init_rval);

 	context = frz_context_create();
 	frz_gfx_init(frz_context_ptr(context));

	zlog_debug("Entering test loop...\n");

	while(frz_gfx_update(frz_context_ptr(context)) != 2);

	zlog_debug("Shutting down...\n");

	fre_shutdown();

	return 0; 	
 }