#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

experimental: testfuncs.o

testfuncs.o: testfuncs.cpp
	$(CC) $(CFLAGS) $< -o $@

#files included in testfuncs.cpp
testfuncs.o: \
testfuncs.h \
../math/convexhull.h \
../game/camera.h \
../main/control.h \
../gui/newconsole.h \
../input/keybinds.h

#PST: what happens if a header that does not change, includes a header that does?