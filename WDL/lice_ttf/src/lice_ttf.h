/*
  lice_ttf.h
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
  generic c api wrapper for truetype font libaries using wdl/lice.

  define one of the following before including this header:
    LTTF_FREETYPE
    LTTF_STBTT

  usage:
    LTTF_st f;
    LTTF_load(&f, filename_ttf);
    LTTF_setsize(&f, size_in_pt);
    LTTF_setaa(&f, LTTF_false, LTTF_true);
    LTTF_drawtext(&f, &some_LICE_SysBitmap, x, y, text, some_LICE_pixel);
    LTTF_free(&f);

  to convert to logical units use:
    LTTF_logicalh(size_in_points, pixels_per_inch)

  notes:
    - kerning and may sizes will differ between libarires.
      for more matching sizes use _sizepx.
    - some of the features of _setaa and _size are disabled with _stbtt
    - library specific data is still available via the _st struct
*/

#ifndef _LICE_TTF_H_
#define _LICE_TTF_H_

/*
  disable libc warnings for msvc
*/
#ifdef _MSC_VER
  #pragma warning(disable : 4996)
#endif

/*
  'inline' could be handled here, but the font libraries are probably
  not ansi c compatible. the wrappers will depend on this definition though.
*/
#define LTTF_INLINE           inline

#define LTTF_true             1
#define LTTF_false            0
#define LTTF_status           int

/*
  calculate logical height:
  sizep - size in points
  dpi - number of pixels per inch (e.g. LOGPIXELSY on WIN32)
  72 - 72 dots per inch
*/
#define LTTF_logicalh(sizep, dpi) \
  (((sizep)*(dpi))/72)

/* library choice */
#ifdef LTTF_FREETYPE /* freetype */

  #define LTTF_st            freetype_st
  #define LTTF_drawchar      freetype_drawchar
  #define LTTF_drawtext      freetype_drawtext
  #define LTTF_setaa         freetype_setaa
  #define LTTF_setsize       freetype_setsize
  #define LTTF_setsizepx     freetype_setsizepx
  #define LTTF_load          freetype_load
  #define LTTF_free          freetype_free

  #include "lice_ttf_freetype.h"

#elif defined LTTF_STBTT /* stbtt */

  #define LTTF_st            stbtt_st
  #define LTTF_drawchar      stbtt_drawchar
  #define LTTF_drawtext      stbtt_drawtext
  #define LTTF_setaa         stbtt_setaa
  #define LTTF_setsize       stbtt_setsize
  #define LTTF_setsizepx     stbtt_setsizepx
  #define LTTF_load          stbtt_load
  #define LTTF_free          stbtt_free

  #include "lice_ttf_stbtt.h"

#else /* unknown, produce error */
  #error Unknown font library. \
Define LTTF_<NAME> before including lice_ttf.h
#endif

#endif /* _LICE_TTF_H_ */
