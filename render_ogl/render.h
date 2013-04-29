/*
 *
 * Freyja Engine - OGL Renderer
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 04 Mar 2013
 *
 * OGL Renderer - Main Header
 *
 */

#define FRZ_MAX_CONTEXT 16

#ifdef FRE_RENDER_CORE
 	#define FREXPORT
#else
 	#define FREXPORT extern
#endif

#ifdef FRE_RENDER_LIB

 	// Include platform-specific OpenGL and Windowing system functions & definitions
 	#if FRE_PLATFORM == FRE_PLATFORM_WIN32
 		#include "render_ogl/os_win/ogl_win.h"
 	#elif FRE_PLATFORM == FRE_PLATFORM_LINUX
 		#include "render_ogl/os_linux/ogl_linux.h"
 	#elif FRE_PLATFORM == FRE_PLATFORM_MAC
 		#include "render_ogl/os_mac/ogl_mac.h"
 	#endif

	#define FRZX_OPTION		0
 	#define FRZX_REQD		1
 	#define FRZX_LOADED		2
 	#define FRZX_NG			4
 	#define FRZX_ARB		8

	typedef struct {
		char extname[64];
		char funcname[128];
		void** funcvec;
		int reqs;
	} FRZ_OGL_EXT;

	FREXPORT void (*logthis)(int llevel, char* srcname, int srcline, char* fmt, ...);
	FREXPORT void (*fre_shutdown)();

	// OGL Extension func pointers
	FREXPORT PFNGLMULTITEXCOORD1FPROC fglMultiTexCoord1f;
	FREXPORT PFNGLMULTITEXCOORD2FPROC fglMultiTexCoord2f;
	FREXPORT PFNGLMULTITEXCOORD3FPROC fglMultiTexCoord3f;
	FREXPORT PFNGLMULTITEXCOORD4FPROC fglMultiTexCoord4f;
	FREXPORT PFNGLACTIVETEXTUREPROC fglActiveTexture;
	FREXPORT PFNGLCLIENTACTIVETEXTUREPROC fglClientActiveTexture;

	extern "C" {

		int frz_init(FRE_RENDER_INIT_DATA* rdata);
		int frz_gfx_init(int ctx);
		int frz_gfx_check_ext(char* extname);
		int frz_gfx_kill(int ctx);
		int frz_gfx_update(int ctx);

		void frz_context_init();
		void* frz_context_ptr(int cdex);
		int frz_context_create(FRE_RENDER_MODE* rm_setup);
		void frz_context_destroy(int ctx);
		int frz_set_fullscreen(int ctx, int fsmode);

		int frz_gfx_bind_glext();

		int frz_subsys_init_display(int ctx);
		char* frz_subsys_enum_oglext(int ctx);
		int frz_gfx_render();
	}

#else

	FEXPORTC int (*frz_init)(FRE_RENDER_INIT_DATA* rdata);
	FEXPORTC int (*frz_gfx_init)(int ctx);
	FEXPORTC int (*frz_gfx_kill)(int ctx);
	FEXPORTC int (*frz_gfx_update)(int ctx);
	FEXPORTC void* (*frz_context_ptr)(int cdex);
	FEXPORTC int (*frz_context_create)(FRE_RENDER_MODE* rm_setup);
	FEXPORTC int (*frz_set_fullscreen)(int ctx, int fsmode);

#endif