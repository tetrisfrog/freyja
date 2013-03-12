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

## Object list
OBJS = asset_mgmt.o config_mgmt.o core_control.o debug.o loadlib.o script_control.o os_linux/os_control.o
LIBS = -ldl

## Compilation & Programming parameters
CFLAGS = -O1 -I/home/jacob/source/freyja -DFREYJA_CORE
LINKFLAGS = -O1 $(LIBS)


.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LINKFLAGS) -o $@ $(OBJS) $(ARS)

clean :
	rm -f *.o
	rm -f os_win/*.o
	rm -f os_linux/*.o
	rm -f os_mac/*.o

all : $(TARGET)

