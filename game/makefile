#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

game: camera.o physics.o

camera.o: camera.cpp
	$(CC) $(CFLAGS) $< -o $@

#files included in camera.cpp
camera.o: \
../math/convexhull.h \
../game/camera.h \
../main/control.h \
../gui/newconsole.h \
../input/keybinds.h

physics.o: physics.cpp
	$(CC) $(CFLAGS) $< -o $@

#files included in physics.cpp
physics.o: \
physics.h \
../main/control.h \
../util/globals.h
#PST: what happens if a header that does not change, includes a header that does?