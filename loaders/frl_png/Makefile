#####################################################################
#
# Bluefire/Freyja Rendering System - Core Loaders
# 
# PNG Loader - Makefile
# Copyright (c) 2013 J. Hipps
#
#####################################################################

## Program names
CC=gcc
TARGET=frl_png.so

## Object list
PLATFORM_OBJS =
OBJS = frl_png.o
LIBS = -lpng -lz

## Compilation & Programming parameters
CFLAGS = -O1 -I../../ -DFRE_LOADER_LIB
LINKFLAGS = -shared -O1 $(LIBS)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LINKFLAGS) -o $@ $(OBJS) $(ARS)

clean :
	rm -f *.o

all : $(TARGET)

