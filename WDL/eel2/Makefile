# makefile for eel2-arm

CC = arm-elf-gcc
CXX = $(CC)
RUN = arm-elf-run
DUMP = arm-elf-objdump
OUT = test.arm
BE = -mbig-endian

CFLAGS = -Wall -Wno-missing-field-initializers -Wno-unused \
-Wno-sign-compare -Wno-cast-qual -g -save-temps $(BE)
LFLAGS = -lm $(BE)

vpath %.c %.cpp %.h

HEADERS = ns-eel-addfuncs.h ns-eel-int.h ns-eel.h
OBJS = nseel-caltab.o nseel-cfunc.o nseel-compiler.o nseel-eval.o \
nseel-lextab.o nseel-ram.o nseel-yylex.o test.o

default: $(OUT)

nseel-caltab.o: nseel-caltab.c $(HEADERS)
	$(CC) -c nseel-caltab.c $(CFLAGS)

nseel-cfunc.o: nseel-cfunc.c asm-nseel-arm-gcc.c $(HEADERS)
	$(CC) -c nseel-cfunc.c $(CFLAGS)

nseel-compiler.o: nseel-compiler.c $(HEADERS)
	$(CC) -c nseel-compiler.c $(CFLAGS)

nseel-eval.o: nseel-eval.c $(HEADERS)
	$(CC) -c nseel-eval.c $(CFLAGS)

nseel-lextab.o: nseel-lextab.c $(HEADERS)
	$(CC) -c nseel-lextab.c $(CFLAGS)

nseel-ram.o: nseel-ram.c $(HEADERS)
	$(CC) -c nseel-ram.c $(CFLAGS)

nseel-yylex.o:nseel-yylex.c $(HEADERS)
	$(CC) -c nseel-yylex.c $(CFLAGS)

test.o:test.cpp $(HEADERS)
	$(CC) -c test.cpp $(CFLAGS)

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) $(LFLAGS)
	@echo [*] all done

run:
	$(RUN) $(OUT)

dump:
	$(DUMP) -d $(OUT) > objdump.txt
	@echo [*] saved to objdump.txt

clean:
	rm -f $(OBJS) $(OUT)

%.o: %.c %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)
