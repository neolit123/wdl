@echo off

set CC=arm-elf-gcc
set CFLAGS=-c -Wall -W -Wno-missing-field-initializers -Wno-unused ^
-Wno-sign-compare
set TFILES=nseel-caltab.c nseel-cfunc.c nseel-compiler.c nseel-eval.c ^
nseel-lextab.c nseel-ram.c nseel-yylex.c

%CC% %TFILES% %CFLAGS%
