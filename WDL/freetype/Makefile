# makefile for lice_ttf

# project name
PROJECT = freetype

# set default compiler if none
ifndef $(CC)
  CC = gcc
endif

# settings
OUT = ./obj/
SRC = ./src/src/
INC_FT = ./src/include
FTBLIB = FT2_BUILD_LIBRARY

FT_SRC_PATHS = base bdf cff cid pcf pfr sfnt truetype type1 type42 winfonts \
cache gzip lzw bzip2 autofit raster smooth pshinter psnames psaux type1

VPATH=%.cpp %.c . $(OUT) $(addprefix $(SRC), $(FT_SRC_PATHS))

SRCS=ftinit.c ftdebug.c ftbase.c ftbbox.c ftglyph.c bdf.c cff.c type1cid.c \
pcf.c pfr.c sfnt.c truetype.c type42.c winfnt.c raster.c smooth.c ftbitmap.c \
ftsystem.c pshinter.c psnames.c ftlzw.c ftgzip.c autofit.c psaux.c type1.c

# per-compiler settings
ifeq ($(CC), cl)
  CFLAGS = /nologo /c /W3 /I$(INC_FT) /D$(FTBLIB)
  POUT = /Fo
  OEXT = .obj
  PROJECT_LIB = $(OUT)$(PROJECT).lib
  LIB_MAKER = lib
   # /nologo does not work
  LIB_FLAGS = /OUT:$(PROJECT_LIB)
  CLEAN_FLAGS = $(OUT)*.obj $(OUT)*.lib
else ifeq ($(CC), gcc)
  CFLAGS = -c -Wall -I$(INC_FT) -D$(FTBLIB)
  POUT = -o
  OEXT = .o
  PROJECT_LIB = $(OUT)lib$(PROJECT).a
  LIB_MAKER = ar
  LIB_FLAGS = rcs $(PROJECT_LIB)
  CLEAN_FLAGS = $(OUT)*.o $(OUT)*.a
else
  $(error ERROR: unsupported compiler: $(CC))
endif

# subst
OBJS:=$(subst .cpp,$(OEXT),$(SRCS))
OBJS:=$(subst .c,$(OEXT),$(OBJS))
OBJS:=$(addprefix $(OUT),$(OBJS))

# rules
default: $(PROJECT_LIB)

$(PROJECT_LIB): $(OBJS)
	$(LIB_MAKER) $(LIB_FLAGS) $^
	@echo --- done with $(PROJECT_LIB)

$(OUT)%$(OEXT): %.cpp
	$(CC) $(CFLAGS) $< $(POUT)$@

$(OUT)%$(OEXT): %.c
	$(CC) $(CFLAGS) $< $(POUT)$@

clean:
	rm -f $(CLEAN_FLAGS)

clean_all:
	rm -f $(OUT)*.o $(OUT)*.a $(OUT)*.obj $(OUT)*.lib
