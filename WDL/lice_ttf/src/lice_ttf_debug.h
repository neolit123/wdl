/*
  lice_ttf_debug.h
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
  compile with:
    /DLTT_DEBUG or -DLTTF_DEBUG

  debug with:
    LTTF_trace(LTTF_TL, printf_like_format, arguments...);
    LTTF_trace(LTTF_TL, "%s, %d", some_string, some_int);

    LTTF_TL will expand to trace location

  note:
    OutputDebugString is used on WIN32
*/

#define LTTF_DEBUG_BUF        1024
#define LTTF_TL               __FILE__, __LINE__

void LTTF_trace(const char *file, int line, const char *format, ...);
