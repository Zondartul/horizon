#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

util: debug.o globals.o hook.o messenger.o

debug.o: debug.cpp
	$(CC) $(CFLAGS) $< -o $@
	
debug.o: \
debug.h \
../util/globals.h

globals.o: globals.cpp
	$(CC) $(CFLAGS) $< -o $@
	
globals.o: \
globals.h

hook.o: hook.cpp
	$(CC) $(CFLAGS) $< -o $@
	
hook.o: \
hook.h \
../util/globals.h

messenger.o: messenger.cpp
	$(CC) $(CFLAGS) $< -o $@
	
messenger.o: \
messenger.h \
../util/globals.h