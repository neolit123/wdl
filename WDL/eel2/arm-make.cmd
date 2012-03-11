@echo off

set CC=arm-elf-gcc
set CFLAGS=-c -Wall -W -Wno-missing-field-initializers -Wno-unused ^
-Wno-sign-compare
set CFILES=nseel-caltab.c nseel-cfunc.c nseel-compiler.c nseel-eval.c ^
nseel-lextab.c nseel-ram.c nseel-yylex.c test.cpp

set OFILES=nseel-caltab.o nseel-cfunc.o nseel-compiler.o nseel-eval.o ^
nseel-lextab.o nseel-ram.o nseel-yylex.o test.o

set LD=arm-elf-gcc
set LDFLAGS=-lm

set OUT=-o test.arm 

%CC% %CFILES% %CFLAGS%
%LD% %OFILES% %LDFLAGS% %OUT%
