/*
  lice_stbtt.cpp
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

#define STB_TRUETYPE_IMPLEMENTATION
#include "lice_ttf_stbtt.h"
#include "lice_ttf_debug.h"

/* definitions */
#define STBTT_CONST_R64       0.015625F
#define STBTT_CONST_R256      0.003921568627450980392156862745098F  /* 1/255 */
#define STBTT_ADVANCE         1
#define STBTT_NO_ADVANCE      0

#define STBTT_DEF_SIZE        12
#define STBTT_DEF_INDEX       0
#define STBTT_DEF_AA_THRESH   0
#define STBTT_DEF_NOAA_THRESH 160
#define STBTT_DEF_HSPACE      0
#define STBTT_DEF_VSPACE      0

#define stbtt_true            1
#define stbtt_false           0

/* function macros */
#define stbtt_advance(st, adv)  ((st).advance = adv)

/*
  lookup table to convert char height to pixels:
  - a very raw method - not accurate for all font faces
  - tries to match freetype's FT_Set_Char_Size's char_height input
    and CreateFont's nHeight
    for fonts Arial, Vera and similar
  - works up to PT = 300 and if larger there is no conversation
*/

static const short
STBTT_SZ_LOOKUP[] =
  { 0,2,2,4,5,5,6,8,10,12,13,14,15,15,17,17,19,19,
    21,21,22,23,25,26,27,29,29,31,33,33,34,35,36,
    37,39,40,41,42,43,43,46,47,48,49,51,52,53,53,
    55,56,57,57,60,61,61,62,64,65,67,67,68,69,70,
    71,73,74,75,76,77,78,80,81,81,83,84,85,86,88,
    89,90,90,92,94,94,95,97,98,98,100,101,103,103,
    104,105,107,108,109,110,111,112,114,115,116,117,
    118,118,121,122,123,123,125,126,127,128,130,131,
    131,132,133,136,136,137,138,140,140,142,143,144,
    145,146,147,149,150,151,152,153,154,156,156,158,
    159,160,160,163,164,165,165,166,168,169,170,169,
    171,171,172,174,176,176,177,178,179,181,182,183,
    184,185,186,188,189,190,191,191,191,194,195,195,
    196,198,199,200,201,203,204,204,205,208,208,209,
    210,212,212,214,215,217,217,218,220,221,222,224,
    225,225,227,228,229,230,232,233,233,234,236,238,
    238,239,241,242,242,244,246,246,247,248,250,251,
    252,253,255,255,256,258,258,260,261,262,262,265,
    266,267,267,268,269,270,271,273,274,274,275,278,
    279,279,280,282,282,284,285,282,283,285,286,287,
    288,289,290,291,292,293,295,296,297,298,299,300,
    301,302,303,304,306,307,308,309,311,311,312,313,
    314,316,317,318,319,321,321,322,323,324,326,327,
    328,329,330,331 };

STBTT_INLINE
STBTT_STATUS
stbtt_setaa(stbtt_st *st, const int aa, int _unused)
{
  if (aa)
    st->aa_thresh = STBTT_DEF_AA_THRESH;
  else
    st->aa_thresh = STBTT_DEF_NOAA_THRESH;
  _unused = _unused;
  return 0;
}

STBTT_INLINE
STBTT_STATUS
stbtt_setsize(stbtt_st *st, const int pt)
{
  if (pt < 300)
    st->size = (STBTT_SZ_LOOKUP[pt]);
  else
    st->size = pt;
  st->scale = stbtt_ScaleForPixelHeight(&st->info, (float)st->size);
  return 0;
}

STBTT_INLINE
STBTT_STATUS
stbtt_setsizepx(stbtt_st *st, const int h)
{
  st->size = h;
  st->scale = stbtt_ScaleForPixelHeight(&st->info, (float)st->size);
  return 0;
}

static
STBTT_INLINE
void
stbtt_setspacing(stbtt_st *st, const int hspace, const int vspace)
{
  st->hspace = hspace;
  st->vspace = vspace;
}

static
STBTT_INLINE
void
stbtt_resetpos(stbtt_st *st)
{
  st->pos = st->newpos = st->adv = st->lsb = 0;
}

static
STBTT_INLINE
int
stbtt_getheight(stbtt_st *st)
{
  stbtt_GetFontVMetrics(&st->info, &st->ascent, &st->descent, &st->linegap);
  return (int)(st->scale*(float)(st->ascent - st->descent + st->linegap));
}

int stbtt_getcharwidth(stbtt_st *st, const int c)
{
  stbtt_GetCodepointHMetrics(&st->info, c, &st->adv, &st->lsb);
  return (int)(st->scale*(float)st->adv);
}

STBTT_INLINE
STBTT_STATUS
stbtt_init(stbtt_st *st, const int index)
{
  st->offset = stbtt_GetFontOffsetForIndex(st->info.data, index);
  return stbtt_InitFont(&st->info, st->info.data, st->offset);
}

STBTT_INLINE
STBTT_STATUS
stbtt_free(stbtt_st *st)
{
  free(st->info.data);
  memset(st, 0, sizeof(st));
  st->loaded = 0;
  return 0;
}

STBTT_STATUS
stbtt_load(stbtt_st *st, const char *filename)
{
  register char *c;

  c = st->filename;
  memset(st->filename, 0, STBTT_NAME_LEN);
  strcat(c, filename);
  strcat(c, (const char*)"\0");

  st->file = fopen(filename, (const char*)"rb");
  if(st->file)
  {
    fseek(st->file, 0, SEEK_END);
    st->len = ftell(st->file);
    rewind(st->file);

    free(st->info.data);
    st->info.data = (unsigned char*) malloc(st->len*sizeof(char));
    if (st->info.data)
    {
      if (fread(st->info.data, sizeof(char), st->len, st->file))
      {
        if (stbtt_init(st, STBTT_DEF_INDEX))
        {
          st->loaded = 1;
          stbtt_resetpos(st);
          stbtt_setsize(st, STBTT_DEF_SIZE);
          st->hspace = STBTT_DEF_HSPACE;
          st->vspace = STBTT_DEF_VSPACE;
          st->aa_thresh = STBTT_DEF_AA_THRESH;
          st->advance = STBTT_ADVANCE;

          fclose(st->file);
          return 0;
        }
      }
    }
  }
  return 1;
}

STBTT_STATUS
stbtt_drawchar( stbtt_st *st,
                LICE_IBitmap *licebmp,
                const int x,
                const int y,
                const int c,
                const LICE_pixel px)
{
  register int i, j;
  register unsigned char op;
  register float alpha = (float)LICE_GETA(px) *
                          STBTT_CONST_R256 * STBTT_CONST_R256;

  st->x = x;
  st->y = y;
  st->lastchar = c;

  if (!st->advance)
    stbtt_resetpos(st);

  stbtt_GetCodepointHMetrics(&st->info, c, &st->adv, &st->lsb);
  st->bitmap = stbtt_GetCodepointBitmap(&st->info, 0.f, st->scale, c,
                                        &st->w, &st->h, &st->xo, &st->yo);

  st->newpos = st->pos + (int)(st->lsb * st->scale);

  j = 0;
  while (j < st->h)
  {
    i = 0;
    while (i < st->w)
    {
      op = st->bitmap[j*st->w + i];
      if (op && op >= st->aa_thresh)
      {
        LICE_PutPixel(licebmp, i + x + st->xo + st->newpos, j + y + st->yo,
                      px, (float)op * alpha, 0);
      }
      i++;
    }
    j++;
  }

  st->pos += (int)(st->adv * st->scale) + st->hspace;
  free(st->bitmap);
  st->advance = 0;

  return 0;
}

STBTT_STATUS
stbtt_drawtext( stbtt_st *st,
                LICE_IBitmap *licebmp,
                const int x,
                const int y,
                const char *text,
                const LICE_pixel px )
{
  register unsigned int cpos;
  const unsigned int len = strlen(text);

  stbtt_resetpos(st);

  cpos = 0;
  while (cpos < len)
  {
    st->advance = 1;
    stbtt_drawchar(st, licebmp, x, y, text[cpos], px);
    cpos++;
  }

  return 0;
}
