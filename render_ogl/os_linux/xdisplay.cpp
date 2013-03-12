/*
 *
 * Freyja Engine - OGL Renderer
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 10 Mar 2013
 *
 * Linux - X11 display routines & platform-specific display implementation
 *
 */

#include "freyja.h"

FRZ_OGLX_CONTEXT condex[FRZ_MAX_CONTEXT];
int condex_sz;

void frz_context_init() {
	condex_sz = 0;
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

void frz_context_destroy(int ctx) {
	// FIXME ///////////////////////////////
}

void* frz_context_ptr(int cdex) {
	return &condex[cdex];
}

int frz_subsys_init_display(int ctx) {

 	FRZ_OGLX_CONTEXT* rtx = &condex[ctx];

 	if((rtx->dpy = XOpenDisplay(NULL)) == NULL) {
 		// Failed to open display
 		return 1;
 	}

 	rtx->rootwin = DefaultRootWindow(rtx->dpy);

 	if((rtx->vi = glXChooseVisual(rtx->dpy, 0, rtx->att)) == NULL) {
 		// Failed to choose visual mode
 		return 2;
 	}

 	rtx->cmap = XCreateColormap(rtx->dpy, rtx->rootwin, rtx->vi->visual, AllocNone);

 	rtx->swa.colormap = rtx->cmap;
 	rtx->swa.event_mask = ExposureMask | KeyPressMask;

 	rtx->win = XCreateWindow(rtx->dpy, rtx->rootwin, 0, 0, rtx->sz_x, rtx->sz_y, 0, rtx->vi->depth, InputOutput, rtx->vi->visual, CWColormap | CWEventMask, &rtx->swa);

	XMapWindow(rtx->dpy, rtx->win);
	XStoreName(rtx->dpy, rtx->win, "Freyja Test");
	 
	rtx->glc = glXCreateContext(rtx->dpy, rtx->vi, NULL, GL_TRUE);
	glXMakeCurrent(rtx->dpy, rtx->win, rtx->glc);

 	return 0;
 }

char* frz_subsys_enum_oglext(int ctx) {
 	FRZ_OGLX_CONTEXT* rtx = &condex[ctx];
	return (char*)glXQueryExtensionsString(rtx->dpy,0);
 }

 int frz_gfx_update(int ctx) {
 	FRZ_OGLX_CONTEXT* rtx = &condex[ctx];

	XNextEvent(rtx->dpy, &rtx->xev);

	if(rtx->xev.type == Expose) {
		XGetWindowAttributes(rtx->dpy, rtx->win, &rtx->gwa);
		glViewport(0, 0, rtx->gwa.width, rtx->gwa.height);
		frz_gfx_render(); 
		glXSwapBuffers(rtx->dpy, rtx->win);
		return 1;
	} else if(rtx->xev.type == KeyPress) {
		frz_gfx_kill(ctx);
		return 2;
	}	

	return 0;
}


 int frz_gfx_kill(int ctx) {
 	FRZ_OGLX_CONTEXT* rtx = &condex[ctx];

	glXMakeCurrent(rtx->dpy, None, NULL);
	glXDestroyContext(rtx->dpy, rtx->glc);
	XDestroyWindow(rtx->dpy, rtx->win);
	XCloseDisplay(rtx->dpy);

	return 0;
 }