/*
 *
 * Freyja Engine - OGL Renderer
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 04 Mar 2013
 *
 * Renderer Main Control
 *
 */

#define FRE_RENDER_CORE

#include "freyja.h"

int frz_init(FRE_RENDER_INIT_DATA* rdata) {
	
	frz_context_init();

	// link to Freyja Core functions
	logthis = rdata->logthis_ptr;
	fre_shutdown = rdata->fre_shutdown_ptr;

	zlog_info("** Freyja Engine - OpenGL Rendering Library\n");
	zlog_info("** Built " __DATE__ " " __TIME__ "\n");

	return 123;
}
