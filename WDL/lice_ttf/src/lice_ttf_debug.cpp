/*
  lice_ttf_debug.cpp
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

#include "lice_ttf_debug.h"

#ifdef LTTF_DEBUG

  #include "stdarg.h"
  #include "stdio.h"
  #include "stdlib.h"

  #ifdef _WIN32
    #include "windows.h"
  #endif

  void LTTF_trace(const char *file, int line, const char *format, ...)
  {
    char buffer[LTTF_DEBUG_BUF];
    char vabuffer[LTTF_DEBUG_BUF-256];

    va_list args;
    va_start(args, format);
      vsprintf(vabuffer, format, args);
    va_end(args);

    //sprintf(buffer, "%s[%d]:%s", file, line, vabuffer);
    sprintf(buffer, "%s", vabuffer);
    #ifdef _WIN32
      OutputDebugString(buffer);
    #else
      fprintf(stderr, "%s\n", buffer);
    #endif
  }

#else  /* !LICE_TTF_DEBUG */

  void LTTF_trace(const char *file, int line, const char *format, ...)
  { return; }

#endif
