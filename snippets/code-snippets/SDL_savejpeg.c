/**
 * Copyright (C) 2004 Pluto, Inc., a Florida Corporation
 *
 * www.plutohome.com
 *
 * Phone: +1 (877) 758-8648
 * This program is distributed according to the terms of the Pluto Public 
 * License, available at: 
 * http://plutohome.com/index.php?section=public_license
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the Pluto 
 * Public License for more details.
 */

/**
 * Adapted from JpegWrapper.cpp by Tyler Montbriand, 2005
 * http://svn.plutohome.com/pluto/trunk/src/Orbiter/SDL/JpegWrapper.cpp
 */

#include <stdio.h>
#include "SDL_byteorder.h"
#include "SDL_image.h"
#include "jpeglib.h" 
#include "SDL_savejpeg.h"

#define RGB_PIXELSIZE 4

int SDL_SaveJPG(SDL_Surface *pSurface, const char* sFileName, int iQuality)
{
    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;
    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */
    int bDeleteBuffer=0;
    unsigned char *image_buffer=NULL;

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
    * stdio stream.  You can also write your own code to do something else.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to write binary files.
    */
    if ((outfile = fopen(sFileName, "wb")) == NULL) {
      SDL_SetError("Can't open %s",sFileName);
      return(-1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    
    cinfo.image_width = pSurface->w; 	/* image width and height, in pixels */
    cinfo.image_height = pSurface->h;
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

    /* Test if image is in the format we want -- if not, convert */
//    if(pSurface->format->BytesPerPixel != 3)
    if(1) // Make more specific later
    {
      SDL_Surface *nsurface;
      Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
      amask = 0x000000ff;
#elif SDL_BYTEORDER == SDL_LIL_ENDIAN
      rmask = 0x000000ff;
      gmask = 0x0000ff00;
      bmask = 0x00ff0000;
      amask = 0xff000000;
#else
#     error "No endian defined!"
#endif

      nsurface=SDL_CreateRGBSurface(
        SDL_SWSURFACE,pSurface->w,pSurface->h,24,
        rmask,gmask,bmask,amask);

      if(nsurface == NULL)
      {
        SDL_SetError("Couldn't flatten surface to 24bpp");
        return(-1);
      }

      /* jpeg doesn't support transparency, so blit checkerboard
       * to show where transparency was */
/*       if(pSurface->flags & (SDL_SRCCOLORKEY|SDL_SRCALPHA))
        checker_fill(nsurface,NULL,-1,dgrey,lgrey);*/

      SDL_BlitSurface(pSurface,NULL,nsurface,NULL);
      pSurface=nsurface;
      bDeleteBuffer=1;
    }

    if(SDL_MUSTLOCK(pSurface))
      SDL_LockSurface(pSurface);

    image_buffer=pSurface->pixels;

    /* Now use the library's routine to set default compression parameters.
    * (You must set at least cinfo.in_color_space before calling this,
    * since the defaults depend on the source color space.)
    */
    jpeg_set_defaults(&cinfo);

    /* -1 quality means 'use default' */
    if(iQuality < 0)
      iQuality=JPEG_QUALITY_DEFAULT;

    /* Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    */
    jpeg_set_quality(&cinfo, iQuality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
    * Pass TRUE unless you are very sure of what you're doing.
    */
    
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.
    */
//    row_stride = cinfo.image_width * 3;	/* JSAMPLEs per row in image_buffer */
    row_stride=pSurface->pitch;

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    /* And we're done! */ 

    if(SDL_MUSTLOCK(pSurface))
      SDL_UnlockSurface(pSurface);

    if(bDeleteBuffer)
      SDL_FreeSurface(pSurface);

    return 0;
}
