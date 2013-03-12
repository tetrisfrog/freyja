#####################################################################
#
# Bluefire/Freyja Rendering System - OGL Renderer
# 
# OpenGL Plugin Makefile
# Copyright (c) 2013 J. Hipps
#
#####################################################################

## Program names
CC=gcc
TARGET=render_ogl.so

## Object list
OBJS = render_main.o display.o
LIBS = -ldl -lX11 -lGL -lGLU

## Compilation & Programming parameters
CFLAGS = -O1 -I../ -DFRE_RENDER_LIB
LINKFLAGS = -shared -O1 $(LIBS)


.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LINKFLAGS) -o $@ $(OBJS) $(ARS)

clean :
	rm -f *.o

all : $(TARGET)

