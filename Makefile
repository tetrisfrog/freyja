#####################################################################
#
# Bluefire/Freyja Rendering System
# 
# Global Makefile
# Copyright (c) 2013 J. Hipps
#
#####################################################################

## Program names
CC=gcc
TARGET=freyja

## Build information
GIT_REVISION = `git rev-list HEAD --count`
BUILD_DCODE  = `date +%Y%m%d`
BUILD_TCODE  = `date +%H%M%S`

## Object list
OBJS = asset_mgmt.o config_mgmt.o core_control.o debug.o loadlib.o script_control.o os_linux/os_control.o loaders/register.o
LIBS = -ldl

## Compilation & Programming parameters
CFLAGS = -O1 -I. -DFREYJA_CORE -DFRE_REV=$(GIT_REVISION) -DBUILD_DCODE=$(BUILD_DCODE) -DBUILD_TCODE=$(BUILD_TCODE)
LINKFLAGS = -O1 $(LIBS)


.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LINKFLAGS) -o $@ $(OBJS) $(ARS)

clean :
	rm -f *.o
	rm -f loaders/*.o
	rm -f os_win/*.o
	rm -f os_linux/*.o
	rm -f os_mac/*.o

all : $(TARGET)

