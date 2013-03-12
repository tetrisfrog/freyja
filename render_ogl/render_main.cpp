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
	
	condex_sz = 0;

	// link to Freyja Core functions
	logthis = rdata->logthis_ptr;
	fre_shutdown = rdata->fre_shutdown_ptr;

	zlog_info("** Freyja Engine - OpenGL Rendering Library\n");
	zlog_info("** Built " __DATE__ " " __TIME__ "\n");

	return 123;
}


int frz_context_create() {
	int newcon;
 
	newcon = condex_sz;
	condex_sz++;

	memset(&condex[newcon],0,sizeof(FRZ_OGLX_CONTEXT));

	// set default attribs
	condex[newcon].att[0] = GLX_RGBA;
	condex[newcon].att[1] = GLX_DEPTH_SIZE;
	condex[newcon].att[2] = 24;
	condex[newcon].att[3] = GLX_DOUBLEBUFFER;
	condex[newcon].att[4] = None;

	// default/fallback res
	condex[newcon].sz_x = 640;
	condex[newcon].sz_y = 480;

	return newcon;
}

void* frz_context_ptr(int cdex) {
	return &condex[cdex];
}