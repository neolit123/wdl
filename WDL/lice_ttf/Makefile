# makefile for lice_ttf

# project name
PROJECT = lice_ttf

# set default compiler if none
ifndef $(CC)
  CC = gcc
endif

# settings
INC_LICE = ../lice/
FT_DIR = ../freetype/
INC_FT = $(FT_DIR)src/include/
LINK_FT_OBJ =
INC_STB = ../stb/
OUT = ./obj/
SRC = ./src/
LIB_PREFIX =
LIB_SUFFIX =

VPATH = %.cpp %.c . $(SRC) $(OUT)
SRCS=lice_ttf_freetype.cpp lice_ttf_stbtt.cpp lice_ttf_debug.cpp

# per-compiler settings
ifeq ($(CC), cl)
  CFLAGS = /nologo /c /W3 /I$(INC_LICE) /I$(INC_FT) /I$(INC_STB)
ifeq ($(DEBUG), 1)
  CFLAGS += /DLTTF_DEBUG
endif
  POUT = /Fo
  OEXT = .obj
  LIB_PREFIX =
  LIB_SUFFIX = .lib
  PROJECT_LIB = $(OUT)$(LIB_PREFIX)$(PROJECT)$(LIB_SUFFIX)
  LIB_MAKER = lib
  # /nologo does not work
  LIB_FLAGS = /OUT:$(PROJECT_LIB)
  CLEAN_FLAGS = $(OUT)*.obj $(OUT)*.lib
else ifeq ($(CC), gcc)
  CFLAGS = -c -Wall -Wno-write-strings -Wno-unused -I$(INC_LICE) \
-I$(INC_FT) -I$(INC_STB)
ifeq ($(DEBUG), 1)
  CFLAGS += -DLTTF_DEBUG
endif
  POUT = -o
  OEXT = .o
  LIB_PREFIX = lib
  LIB_SUFFIX = .a
  PROJECT_LIB = $(OUT)$(LIB_PREFIX)$(PROJECT)$(LIB_SUFFIX)
  LIB_MAKER = ar
  LIB_FLAGS = rcs $(PROJECT_LIB)
  CLEAN_FLAGS = $(OUT)*.o $(OUT)*.a
else
  $(error ERROR: unsupported compiler: $(CC))
endif

# link with freetype
ifeq ($(FT), 1)
  $(info --- will link with $(FT_DIR)obj/$(LIB_PREFIX)freetype$(LIB_SUFFIX));
  $(info );
  $(shell rm -f $(PROJECT_LIB));
  LINK_FT_OBJ += $(FT_DIR)obj/$(LIB_PREFIX)freetype$(LIB_SUFFIX)
endif

# subst
OBJS:=$(subst .cpp,$(OEXT),$(SRCS))
OBJS:=$(subst .c,$(OEXT),$(OBJS))
OBJS:=$(addprefix $(OUT),$(OBJS))

# rules
default: $(PROJECT_LIB)

$(PROJECT_LIB): $(OBJS)
ifeq ($(FT_CLEAN), 1)
	$(MAKE) -C $(FT_DIR) CC=$(CC) clean
endif
ifeq ($(FT_CLEAN_ALL), 1)
	$(MAKE) -C $(FT_DIR) CC=$(CC) clean_all
endif
ifeq ($(FT), 1)
	$(MAKE) -C $(FT_DIR) CC=$(CC)
endif
	$(LIB_MAKER) $(LIB_FLAGS) $^ $(LINK_FT_OBJ)
	@echo --- done with $(PROJECT_LIB)

$(OUT)%$(OEXT): %.cpp
	$(CC) $(CFLAGS) $< $(POUT)$@

$(OUT)%$(OEXT): %.c
	$(CC) $(CFLAGS) $< $(POUT)$@

clean:
	rm -f $(CLEAN_FLAGS)

clean_all:
	rm -f $(OUT)*.o $(OUT)*.a $(OUT)*.obj $(OUT)*.lib
