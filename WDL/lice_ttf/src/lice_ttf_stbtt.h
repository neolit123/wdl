/*
  lice_ttf_stbtt.h
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
  user-level, c api wrapper for the stbtt_truetype library by sean barrett
  using wdl/lice.

  usage:
    stbtt_st f;
    stbtt_load(&f, filename_ttf);
    stbtt_setsize(&f, size_in_pt);
    stbtt_setaa(&f, ft_false, ft_true);
    stbtt_drawtext(&f, &some_LICE_SysBitmap, x, y, text, some_LICE_pixel);
    stbtt_free(&f);
*/

#ifndef _LICE_TTF_STBTT_H_
#define _LICE_TTF_STBTT_H_

#include "stdlib.h"
#include "stdio.h"
#include "lice.h"

#include "../stb/stb_truetype.h"

/*
  disable libc warnings for msvc
*/
#ifdef _MSC_VER
  #pragma warning(disable : 4996)
#endif

/* inline */
#ifdef LTTF_INLINE
  #define STBTT_INLINE        LTTF_INLINE
#else
  #define STBTT_INLINE
#endif

/* handle DPI */
#ifdef LTTF_DPI
  #define STBTT_DEF_DPI       LTTF_DPI
#else
  #define STBTT_DEF_DPI       72
#endif

/* definitions */
#define STBTT_NAME_LEN        512
#define STBTT_STATUS          int

/*
  main struct
  a lot of variables are present to handle all parameter and drawing.
*/
typedef struct
{
  char            filename[STBTT_NAME_LEN];
  stbtt_fontinfo  info;

  unsigned char   *bitmap;
  FILE            *file;
  int             len;
  int             loaded;

  float           scale;
  int             offset;
  int             size;
  int             lastchar;

  int             aa_thresh;
  int             advance;

  int             w;
  int             h;
  int             x;
  int             y;
  int             xo;
  int             yo;
  int             adv;
  int             lsb;
  int             ascent;
  int             descent;
  int             linegap;
  int             pos;
  int             newpos;
  int             hspace;
  int             vspace;

} stbtt_st;

/*
  set antialiasing threshold
  "_unused" since there is no hinting in stbtt
*/
STBTT_STATUS
stbtt_setaa(stbtt_st *st, const int aa, int _unused);

/* set size in pt */
STBTT_STATUS
stbtt_setsize(stbtt_st *st, const int pt);

/* set size in pixels */
STBTT_STATUS
stbtt_setsizepx(stbtt_st *st, const int h);

/* vertical and horizontal spacing */
static
void
stbtt_setspacing(stbtt_st *st, const int hspace, const int vspace);

/* reset horizontal position */
static
void
stbtt_resetpos(stbtt_st *st);

/* get font height */
static
int
stbtt_getheight(stbtt_st *st);

/* char width */
static
int stbtt_getcharwidth(stbtt_st *st, const int c);

/* init */
static
STBTT_STATUS
stbtt_init(stbtt_st *st, const int index);

/* load ttf font */
STBTT_STATUS
stbtt_load(stbtt_st *st, const char *filename);

/* release font */
STBTT_STATUS
stbtt_free(stbtt_st *st);

/* draw char on a bitmap */
STBTT_STATUS
stbtt_drawchar( stbtt_st *st,
                LICE_IBitmap *licebmp,
                const int x,
                const int y,
                const int c,
                const LICE_pixel px);

/* draw string of text on a bitmap */
STBTT_STATUS
stbtt_drawtext( stbtt_st *st,
                LICE_IBitmap *licebmp,
                const int x,
                const int y,
                const char *text,
                const LICE_pixel px );

#endif /* _LICE_TTF_STBTT_H_ */
