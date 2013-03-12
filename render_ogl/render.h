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
 	#define ZREXPORT
#else
 	#define ZREXPORT extern
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

 	#include "render_ogl/glext/glext.h"

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

 	ZREXPORT FRZ_OGLX_CONTEXT condex[FRZ_MAX_CONTEXT];
 	ZREXPORT int condex_sz;
	ZREXPORT void (*logthis)(int llevel, char* srcname, int srcline, char* fmt, ...);
	ZREXPORT void (*fre_shutdown)();

	// OGL Extension func pointers
	ZREXPORT PFNGLMULTITEXCOORD1FPROC fglMultiTexCoord1f;
	ZREXPORT PFNGLMULTITEXCOORD2FPROC fglMultiTexCoord2f;
	ZREXPORT PFNGLMULTITEXCOORD3FPROC fglMultiTexCoord3f;
	ZREXPORT PFNGLMULTITEXCOORD4FPROC fglMultiTexCoord4f;
	ZREXPORT PFNGLACTIVETEXTUREPROC fglActiveTexture;
	ZREXPORT PFNGLCLIENTACTIVETEXTUREPROC fglClientActiveTexture;

	extern "C" {

		int frz_init(FRE_RENDER_INIT_DATA* rdata);
		int frz_gfx_init(FRZ_OGLX_CONTEXT* rtx);
		int frz_gfx_check_ext(char* extname);
		int frz_gfx_kill(FRZ_OGLX_CONTEXT* rtx);
		int frz_gfx_update(FRZ_OGLX_CONTEXT* rtx);
		void* frz_context_ptr(int cdex);
		int frz_context_create();
		int frz_gfx_bind_glext();

	}

#else

	ZEXPORTC int (*frz_init)(FRE_RENDER_INIT_DATA* rdata);
	ZEXPORTC int (*frz_gfx_init)(void* cptr);
	ZEXPORTC int (*frz_gfx_kill)(void* cptr);
	ZEXPORTC int (*frz_gfx_update)(void* cptr);
	ZEXPORTC void* (*frz_context_ptr)(int cdex);
	ZEXPORTC int (*frz_context_create)();

#endif