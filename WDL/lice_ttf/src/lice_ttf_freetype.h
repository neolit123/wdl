/*
  lice_ttf_freetype.h
  Copyright (C) 2011 and later Lubomir I. Ivanov (neolit123 [at] gmail)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/*
  end-user level, c api wrapper for the freetype library using wdl/lice.

  usage:
    freetype_st f;
    freetype_load(&f, filename_ttf);
    freetype_setsize(&f, size_in_pt);
    freetype_setaa(&f, freetype_false, freetype_true);
    freetype_drawtext(&f, &some_LICE_SysBitmap, x, y, text, some_LICE_pixel);
    freetype_free(&f);

  todo:
    - optimize monochrome rendering
    - expose more api
*/

#ifndef _LICE_TTF_FREETYPE_H_
#define _LICE_TTF_FREETYPE_H_

#include "stdlib.h"
#include "lice.h"

#include "freetype/ftbitmap.h"
#include FT_FREETYPE_H

/*
  disable libc warnings for msvc
*/
#ifdef _MSC_VER
  #pragma warning(disable : 4996)
#endif

/* handle DPI */
#ifdef LTTF_DPI
  #define FT_DEF_DPI       LTTF_DPI
#else
  #define FT_DEF_DPI       72
#endif

/* inline */
#ifdef LTTF_INLINE
  #define FT_INLINE       LTTF_INLINE
#else
  #define FT_INLINE
#endif

/* definitions */
#define FT_NAME_LEN       512

/* main structure */
typedef struct
{
  char                filename[FT_NAME_LEN];

  FT_Library          library;
  FT_Face             face;
  FT_GlyphSlot        slot;
  FT_Error            error;

  FT_Int              load_mode;
  FT_Render_Mode      render_mode;

  int                 loaded;

} freetype_st;

/* set aa (0-1) and hinting (0-1) */
FT_Error
freetype_setaa(freetype_st *st, const FT_Int aa_mode, register FT_Int hinting);

/* set size in pt */
FT_Error
freetype_setsize(freetype_st *st, const FT_Int size);

/* set size in pixels */
FT_Error
freetype_setsizepx(freetype_st *st, const FT_UInt h);

/* load ttf (alloc) */
FT_Error
freetype_load(freetype_st *st, const char* filename);

/* release font (free) */
FT_Error
freetype_free(freetype_st *st);

/*
  draw char pixels on a bitmap directly. optimized for speed
  (more size overhead), apart from using the bitmap conversion
  for monochrome buffers (todo: replace / optimize).
*/
FT_Error
freetype_drawchar(  freetype_st       *st,
                    LICE_SysBitmap    *bm,
                    const FT_Int      x,
                    const FT_Int      y,
                    const char        c,
                    const LICE_pixel  px);

/* draw a string of text on a bitmap */
FT_Error
freetype_drawtext(  freetype_st       *st,
                    LICE_SysBitmap    *bm,
                    register FT_Int   x,
                    register FT_Int   y,
                    const char        *text,
                    const LICE_pixel  px);

#endif /* _LICE_TTF_FREETYPE_H_ */
