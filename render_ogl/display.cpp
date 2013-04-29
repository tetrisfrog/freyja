/*
 *
 * Freyja Engine - OGL Renderer
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 04 Mar 2013
 *
 * Display Setup & Initialization
 *
 */

#include "freyja.h"

// GLEXT Extension function mapping
FRZ_OGL_EXT ogl_extmap[] = {
	{"GL_ARB_multitexture",					"glMultiTexCoord1f",		(void**)&fglMultiTexCoord1f,		FRZX_REQD | FRZX_ARB },
	{"GL_ARB_multitexture",					"glMultiTexCoord2f",		(void**)&fglMultiTexCoord2f,		FRZX_REQD | FRZX_ARB },
	{"GL_ARB_multitexture",					"glMultiTexCoord3f",		(void**)&fglMultiTexCoord3f,		FRZX_REQD | FRZX_ARB },
	{"GL_ARB_multitexture",					"glMultiTexCoord4f",		(void**)&fglMultiTexCoord4f,		FRZX_REQD | FRZX_ARB },
	{"GL_ARB_multitexture",					"glActiveTexture",			(void**)&fglActiveTexture,			FRZX_REQD | FRZX_ARB },
	{"GL_ARB_multitexture",					"glClientActiveTexture",	(void**)&fglClientActiveTexture,	FRZX_REQD | FRZX_ARB },
	{"GL_ARB_texture_non_power_of_two",		NULL,						NULL,								FRZX_OPTION | FRZX_ARB },
	{"GL_ARB_texture_compression",			NULL,						NULL,								FRZX_OPTION },
	{"GL_EXT_texture_compression_s3tc",		NULL,						NULL,								FRZX_OPTION },
	{"GLX_AMD_gpu_association",				NULL,						NULL,								FRZX_OPTION },
	{NULL, NULL, NULL}
};

char ogl_ext_list[512][64];
int  ogl_ext_sz;

int tex_units;

int frz_gfx_init(int ctx) {

 	int rval;

 	if(rval = frz_subsys_init_display(ctx)) {
 		zlog_error("frz_gfx_init: Failed to initialize graphics subsystem! frz_subsys_init_display() failed with %i rval.\n",rval);
 		return 1;
 	}

	// Renderer info
	zlog_info("*** Rendering subsystem information **\n"
		      "    Renderer       : %s\n"
		      "    Vendor         : %s\n"
		      "    OpenGL Version : %s\n"
		 ,glGetString(GL_RENDERER),glGetString(GL_VENDOR),glGetString(GL_VERSION)
		);

	// OGL Extension caps
	char* ext_ptr;
	int ext_strlen;
	if((ext_ptr = strdup((char*)glGetString(GL_EXTENSIONS))) == NULL) {
		zlog_error("frz_gfx_init: Failed to retrieve GL_EXTENSIONS descriptor!\n");
		return 1;
	}

	ext_strlen = strlen(ext_ptr);
	ogl_ext_sz = 0;
	int cz = 0;

	// Explode into array
	for(int z = 0; z < ext_strlen; z++) {
		if(ext_ptr[z] == ' ' || ext_ptr[z] == '\n') {
			ogl_ext_list[ogl_ext_sz][cz] = 0;
			cz = 0;
			ogl_ext_sz++;
		} else {
			ogl_ext_list[ogl_ext_sz][cz] = ext_ptr[z];
			cz++;
		}
	}

	// GLX Extension caps
	// frz_subsys_enum_oglext()
	char* glx_ext_ptr;
	int glx_ext_strlen;
	if((glx_ext_ptr = strdup(frz_subsys_enum_oglext(ctx))) == NULL) {
		zlog_error("frz_gfx_init: glXQueryExtensionsString() failed!\n");
		return 1;
	}

	glx_ext_strlen = strlen(glx_ext_ptr);
	cz = 0;

	// Explode into array - continue where OGL extensions left off
	for(int z = 0; z < glx_ext_strlen; z++) {
		if(glx_ext_ptr[z] == ' ' || glx_ext_ptr[z] == '\n') {
			ogl_ext_list[ogl_ext_sz][cz] = 0;
			cz = 0;
			ogl_ext_sz++;
		} else {
			ogl_ext_list[ogl_ext_sz][cz] = glx_ext_ptr[z];
			cz++;
		}
	}

	// Echo array to log
	zlog_info("**  Extensions  : %i enumerated.\n",ogl_ext_sz);
	for(int z = 0; z < ogl_ext_sz; z++) {
		zlog_info("%s\n",ogl_ext_list[z]);
	}


	// Check for required extensions and bind GLEXT functions
	frz_gfx_bind_glext();

	// Check number of texture units
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&tex_units);
	zlog_info("**  Texture Units : %i\n",tex_units);

 	glEnable(GL_DEPTH_TEST);

	return 0;
}

int frz_gfx_bind_glext() {
	char lastext[64];
	char arbfunc[64];
	int  lastext_found = 0;
	int  load_ok = 0;
	int  loaded_good = 0;
	int  loaded_fail = 0;
	lastext[0] = 0;

	// iterate through ogl_extmap and map functions while checking for availability
	for(int i = 0; i < 1024; i++) {
		if(!ogl_extmap[i].extname[0]) break;

		if(!strcmp(lastext, ogl_extmap[i].extname) && lastext_found) {
			load_ok = 1;
		} else {
			lastext_found = frz_gfx_check_ext(ogl_extmap[i].extname);
			strcpy(lastext, ogl_extmap[i].extname);

			if(!lastext_found) {
				load_ok = 0;
				ogl_extmap[i].reqs |= FRZX_NG;
				if(ogl_extmap[i].reqs & FRZX_REQD) {
					zlog_error("frz_gfx_bind_glext: [%s] Required extension not available!\n",ogl_extmap[i].extname);
				} else {
					zlog_error("frz_gfx_bind_glext: [%s] Extension not available (optional)\n",ogl_extmap[i].extname);
				}
			} else {
				load_ok = 1;
			}
		}

		// First attempt
		if(load_ok && ogl_extmap[i].funcname[0]) {
			// map vector to appropriate func pointer
			if(((*ogl_extmap[i].funcvec) = (void*)OGLX_GetProcAddress((GLubyte*)ogl_extmap[i].funcname)) == NULL) {
				zlog_error("frz_gfx_bind_glext: OGLX_GetProcAddress() failed to retrieve function vector for %s()!\n",ogl_extmap[i].funcname);
				load_ok = 2;
			}
		}

		// Second attempt, ARB functions
		if(load_ok == 2 && ogl_extmap[i].reqs & FRZX_ARB) {
			// Try appending "ARB" to function name for older OGL implementations
			sprintf(arbfunc,"%sARB",ogl_extmap[i].funcname);
			zlog_error("frz_gfx_bind_glext: Attempting to bind to %s() instead...\n",arbfunc);
			if(((*ogl_extmap[i].funcvec) = (void*)OGLX_GetProcAddress((GLubyte*)arbfunc)) == NULL) {
				zlog_error("frz_gfx_bind_glext: OGLX_GetProcAddress() failed to retrieve function vector for %s()!\n",arbfunc);
				load_ok = 0;
			} else {
				// If that works, then modify funcname with ARB suffix
				strcpy(ogl_extmap[i].funcname,arbfunc);
				load_ok = 1;
			}	
		}

		if(load_ok == 1 && !ogl_extmap[i].funcname[0]) {
			ogl_extmap[i].reqs |= FRZX_LOADED;
			zlog_debug("frz_gfx_bind_glext: [%s] Extension supported!\n",ogl_extmap[i].extname);
		} else if(load_ok == 1) {
			ogl_extmap[i].reqs |= FRZX_LOADED;
			zlog_debug("frz_gfx_bind_glext: [%s] %s() hooked func vec OK!\n",ogl_extmap[i].extname,ogl_extmap[i].funcname);
			loaded_good++;
		} else {
			ogl_extmap[i].reqs |= FRZX_NG;
			loaded_fail++;
		}

	}

	zlog_debug("frz_gfx_bind_glext: Bound to %i functions successfully! (%i failed)\n",loaded_good,loaded_fail);

	return loaded_good;
}

int frz_gfx_check_ext(char* extname) {
	int found = -1;
	for(int i = 0; i < ogl_ext_sz; i++) {
		if(!strcmp(extname,ogl_ext_list[i])) {
			found = i;
			break;
		}
	}

	if(found != -1) return 1;
	else return 0;
}

int frz_gfx_render() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1., 1., -1., 1., 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

	glBegin(GL_QUADS);
	glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
	glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
	glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
	glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
	glEnd();

	return 0;
}
