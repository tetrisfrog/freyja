/*
 *
 * Freyja Engine
 * J. Hipps - http://jhipps.org/
 * Copyright (c) 2013 Jacob Hipps/Neo-Retro Group
 *
 * Started - 22 Mar 2013
 *
 * Content Loaders - Core - PNG Loader
 *
 */

#define ZLOADER_MAIN

#include "freyja.h"
#include "frl_png.h"

FRL_LOADER_TYPES frl_png_types[2] = { "PNG", NULL };

// Local declarations
void png_rowcall(png_structp png_ptr, png_uint_32 row, int pass);


int loader_init(FRE_RENDER_INIT_DATA* rdata) {

	// link to Freyja Core functions
	logthis = rdata->logthis_ptr;
	fre_shutdown = rdata->fre_shutdown_ptr;

	zlog_info("** %s - v%i.%i\n",LOADER_MODULE_NAME,LOADER_MODULE_VER_MAJOR,LOADER_MODULE_VER_MINOR);
	zlog_info("** %s\n",LOADER_MODULE_VENDOR);
	zlog_info("** Built %s\n",LOADER_MODULE_COMPILED);

	return LOADER_MODULE_TYPE;
}

int loader_query_types(char* outstr, int maxsz) {
	int tot_types = 0;
	char *ttype;

	for(int i = 0; i < 256; i++) {
		ttype = frl_png_types[i].loader;
		if(!ttype) {
			tot_types = i;
			break;
		}
	}

	if(outstr) {
		if(maxsz >= sizeof(FRL_LOADER_TYPES) * tot_types) {
			memcpy(outstr,frl_png_types,tot_types);
		} else {
			zlog_warn("loader_query_types: outstr buffer size is insufficient! Nothing copied.\n");
		}
	}

	zlog_debug("loader_query_types: outstr pointer is NULL. Returning only tot_types.\n");

	return tot_types;
}

int decode_from_file(FILE* infile, void* dest_mem, int d_sz, int params) {

	FRL_IMG_DATA* img_info = (FRL_IMG_DATA*)dest_mem;

	img_info->type = FRL_TYPE_IMAGE;
	img_info->type = FRL_IMG_RGBA;
	int bpx = 3;

	// check the header to make sure we've got a PNG

	png_byte header[8];
	fread(header,1,8,infile);

	if(png_sig_cmp(header, 0, 8)) {
		zlog_error("Not a PNG file!\n");
	}

	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_info;

	if(!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL/*(png_voidp)user_error_ptr*/, NULL/*user_error_fn*/, NULL/*user_warning_fn*/))) {
		zlog_error("PNG initilaization error!\n");
		return 1;
	}
	if(!(info_ptr = png_create_info_struct(png_ptr))) {      
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);      
		zlog_error("PNG initilaization error!\n");
		return 1;
	}
	if(!(end_info = png_create_info_struct(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		zlog_error("PNG initilaization error!\n");
		return 1;
	}

	// initialize file i/o
	png_init_io(png_ptr, infile);
	// tell libpng that we already read 8 bytes from the file to check for the signature/header
	png_set_sig_bytes(png_ptr, 8);
	// read it! take the low and broad way, which is good in this case, eh?
	png_read_info(png_ptr, info_ptr);

	// get parameters.
	img_info->height = png_get_image_height(png_ptr, info_ptr);
	img_info->width = png_get_image_width(png_ptr, info_ptr);
	img_info->planes = 2; // 2D image
	// FIXME: Force 32-bit depth and fill alpha accordingly if needed
	img_info->depth = 32; //png_get_bit_depth(png_ptr, info_ptr);

	// get the nearest powers of two
  	// FIXME: SKIP FOR NOW

	int x_2com = img_info->width;
	int y_2com = img_info->height;

	/* FIXME **************
	if(xpower2) {
		x_2com = esm_find2com(img_info->width);
		y_2com = esm_find2com(img_info->height);
	}
	*/

	// add an alpha channel
	bpx = 4; // 4 bytes/pixel
	img_info->type = 4;
	// this will make all the pixels of our image opaque and this ones
	// that are simply the buffer zone be transparent
	png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);


	// allocate memory. ack.
	if((img_info->data_ptr = (char*)calloc(x_2com,y_2com * bpx)) == NULL) {
		zlog_error("decode_from_file: memory allocation error! bail out!\n");
	}
  
	int i;
  
  	// FIXME
	png_bytep row_pointers[3200]; // negligence at work :)

	for (i = 0; i < img_info->height; i++) {
		// we just find the offset of each row. simple, eh?
		row_pointers[i] = (png_byte*)((int)img_info->data_ptr + (x_2com * bpx * i));
	}


	// read the image!
	png_read_image(png_ptr, row_pointers);

	// die!
	png_read_end(png_ptr, end_info);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

	return 0;
}


void png_rowcall(png_structp png_ptr, png_uint_32 row, int pass) {

/*
	dots = (float(row) / float(png_img.height)) * PROG_DOTS;
	if(dots != olddots) {
		printf(".");
	}
	olddots = dots;
*/
	return;
}
 