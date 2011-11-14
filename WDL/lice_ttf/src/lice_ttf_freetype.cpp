/*
  lice_ttf_freetype.cpp
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

#include "lice_ttf_freetype.h"
#include "lice_ttf_debug.h"

/* definitions */
#define FT_CONST_R255     0.003921568627450980392156862745098F  /* 1/255 */
#define FT_DEF_SIZE_PT    12
#define FT_DEF_LOADED     0x1

#define freetype_true           1
#define freetype_false          0

/* function macros */
#define freetype_advx(st)               ((st).slot->advance.x >> 6)
#define freetype_advy(st)               ((st).slot->advance.y >> 6)
#define freetype_load_mode(st, mode)    ((st).load_mode   = (mode))
#define freetype_render_mode(st, mode)  ((st).render_mode = (mode))

#define FT_FLOOR(X)	((X & -64) / 64)
#define FT_CEIL(X)	(((X + 63) & -64) / 64)

FT_INLINE
FT_Error
freetype_setaa(freetype_st *st, const FT_Int aa_mode, register FT_Int hinting)
{
  if (hinting)
    hinting = 0;
  else
    hinting = FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;

  if (aa_mode)
  {
    freetype_load_mode(*st, FT_LOAD_DEFAULT | hinting);
    freetype_render_mode(*st, FT_RENDER_MODE_NORMAL);
  }
  else
  {
    freetype_load_mode(*st, FT_LOAD_TARGET_MONO | FT_LOAD_MONOCHROME | hinting);
    freetype_render_mode(*st, FT_RENDER_MODE_MONO);
  }
  return 0;
}

FT_INLINE
FT_Error
freetype_setsize(freetype_st *st, const FT_Int size)
{
  st->error = FT_Set_Char_Size(st->face, 0, (size << 6), FT_DEF_DPI, 0);
  return st->error;
}

FT_INLINE
FT_Error
freetype_setsizepx(freetype_st *st, const FT_UInt h)
{
  st->error = FT_Set_Pixel_Sizes(st->face, 0, h);
  return st->error;
}

FT_INLINE
FT_Error
freetype_free(freetype_st *st)
{
  if (st->loaded == FT_DEF_LOADED)
  {
    st->error = FT_Done_Face(st->face);
    st->error = FT_Done_FreeType(st->library);
    memset(st, 0, sizeof(st));
    st->loaded = 0;
    return st->error;
  }
  return 1;
}

FT_INLINE
FT_Error
freetype_load(freetype_st *st, const char* filename)
{
  register char *c;

  c = st->filename;
  memset(st->filename, 0, FT_NAME_LEN);
  strcat(c, filename);
  strcat(c, (const char*)"\0");

  if (st->loaded == FT_DEF_LOADED)
  {
    st->error = FT_Done_Face(st->face);
    st->error = FT_Done_FreeType(st->library);
  }

  st->error = FT_Init_FreeType(&st->library);
  st->error = FT_New_Face(st->library, filename, 0, &st->face);
  st->error = freetype_setsize(st, FT_DEF_SIZE_PT);
  st->slot = st->face->glyph;
  st->load_mode = FT_LOAD_DEFAULT;
  st->render_mode = FT_RENDER_MODE_NORMAL;

  if (!st->error)
    st->loaded = FT_DEF_LOADED;
  return st->error;
}

#define FT_DRAWCHAR_INIT_SET                            \
  x_init  = x + st->slot->bitmap_left;                  \
  y_init  = y - st->slot->bitmap_top;                   \
  x_max   = x_init + ptr->width;                        \
  y_max   = y_init + ptr->rows

#define FT_DRAWCHAR_LOOP_SET_A(value_of_a)              \
  i = x_init;  p = 0;                                   \
  while (i < x_max)                                     \
  {                                                     \
    j = y_init;  q = 0;                                 \
    while (j < y_max)                                   \
    {                                                   \
      b_val = ptr->buffer[q*ptr->width + p];            \
      if (b_val)                                        \
        LICE_PutPixel(bm, i, j, px, (value_of_a), 0);   \
      j++;  q++;                                        \
    }                                                   \
    i++;  p++;                                          \
  }

FT_Error
freetype_drawchar(  freetype_st             *st,
                    LICE_SysBitmap    *bm,
                    const FT_Int      x,
                    const FT_Int      y,
                    const char        c,
                    const LICE_pixel  px)
{
  register    FT_Int  i, j, p, q, b_val;
  FT_Int      x_init, y_init, x_max, y_max;
  FT_Bitmap   *ptr, tb;

  const FT_UInt   glyph_index = FT_Get_Char_Index(st->face, c);
  const float     a_scale     = (float)LICE_GETA(px) * FT_CONST_R255;
  const float     a_bscale    = a_scale * FT_CONST_R255;

  st->error = FT_Load_Glyph(st->face, glyph_index, st->load_mode);
  st->error = FT_Render_Glyph(st->face->glyph, st->render_mode);
  if (st->error)
    return st->error;

  if (st->render_mode == FT_RENDER_MODE_MONO)
  {
    FT_Bitmap_New(&tb);
    st->error = FT_Bitmap_Convert(st->library, &st->slot->bitmap, &tb, 1);
    ptr = &tb;
    if (st->error)
    {
      st->error = FT_Bitmap_Done(st->library, ptr);
      return st->error;
    }
    FT_DRAWCHAR_INIT_SET;
    FT_DRAWCHAR_LOOP_SET_A(a_scale);
    st->error = FT_Bitmap_Done(st->library, ptr);
  }
  else
  {
    ptr = &st->slot->bitmap;
    FT_DRAWCHAR_INIT_SET;
    FT_DRAWCHAR_LOOP_SET_A((float)b_val*a_bscale);
  }

  return st->error;
}

FT_Error
freetype_drawtext(  freetype_st       *st,
                    LICE_SysBitmap    *bm,
                    register FT_Int   x,
                    register FT_Int   y,
                    const char        *text,
                    const LICE_pixel  px)
{
  register  FT_Int    i;
  const     FT_Int    num_chars = strlen(text);

  int scale = st->face->size->metrics.y_scale;
	int ascent  = FT_CEIL(FT_MulFix(st->face->bbox.yMax, scale));
	int descent = FT_CEIL(FT_MulFix(st->face->bbox.yMin, scale));
  int height  = ascent - descent + /* baseline */ 1;
	int lineskip = FT_CEIL(FT_MulFix(st->face->height, st->face->size->metrics.y_scale));

  // (FT_MulFix(st->face->bbox.yMax, scale) - FT_MulFix(st->face->bbox.yMin, scale) + 1) >> 6

  LTTF_trace(LTTF_TL, "FT_IS_SCALABLE = %d", FT_IS_SCALABLE(st->face));
  LTTF_trace(LTTF_TL, "lineskip = %d", lineskip);
  // y += height >> 6;

  i = 0;
  while (i < num_chars)
  {
    st->error = freetype_drawchar(st, bm, x, y, text[i], px);
    if (st->error)
      return st->error;

    x += freetype_advx(*st);
    y += freetype_advy(*st);
    i++;
  }

  return st->error;
}
