/*
 *
 * Freyja Engine - OGL Renderer
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 04 Mar 2013
 *
 * OGL Renderer - Linux OS-specific header
 *
 */

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include "render_ogl/glext/glext.h"

// Define function name for extension func pointer resolution
#define OGLX_GetProcAddress glXGetProcAddress

typedef struct {
	int id;
	Display *dpy;
	Window rootwin;
	GLint att[8];
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	Window win;
	GLXContext glc;
	XWindowAttributes gwa;
	XEvent xev;
	int sz_x;
	int sz_y;
	int fullscreen;
} FRZ_OGLX_CONTEXT;
