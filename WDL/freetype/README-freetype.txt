freetype:
  place freetype library source in ./src
  e.g. ./src/docs, ./src/include, ./src/src etc...

building:
  make
  make CC=gcc
  make CC=cl
  make clean CC=gcc
  make clean CC=cl
  make clean_all

  tested with GNU make 3.81.

object code:
  will be generated in ./obj
  cl:   freetype.lib
  gcc:  libfreetype.a

contact:
  lubomir i. ivanov
  neolit123 [at] gmail
