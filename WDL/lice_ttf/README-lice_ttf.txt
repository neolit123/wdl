lice_ttf:
  multiplatform wrapper library for stb-truetype and freetype.
  uses WDL/lice for rendering.

building:
  CC=compiler (cl/gcc). gcc is the default value
  FT=1 will link with freetype
  FT_CLEAN=1 will clean freetype objects for this compiler
  FT_CLEAN_ALL=1 will clean all freetype objects

  make
  make CC=gcc
  make CC=cl
  make FT=1
  make FT_CLEAN=1
  make FT_CLEAN_ALL=1
  make clean CC=gcc
  make clean CC=cl
  make clean_all
  make CC=cl FT=1 FT_CLEAN_ALL=1

  tested with GNU make 3.81.

object code:
  will be generated in ./obj
  cl:   lice_tff.lib
  gcc:  liblice_tff.a

dependencies:
  ../freetype/src/include/    <- freetype sources
  ../freetype/obj/            <- will link to (lib)freetype.(lib/a)
  ../stb/                     <- stb_truetype.h
  ../lice/                    <- lice headers

contact:
  lubomir i. ivanov
  neolit123 [at] gmail
