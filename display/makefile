#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

display: paint.o renderable.o

paint.o: paint.cpp
	$(CC) $(CFLAGS) $< -o $@

#files included in paint.cpp
paint.o: \
paint.h \
../util/globals.h

renderable.o: renderable.cpp
	$(CC) $(CFLAGS) $< -o $@

#files included in renderable.cpp
renderable.o: \
renderable.h \
../util/globals.h \
../math/vectors.h \
../game/camera.h \
../resource/models.h \
../gui/valscreen.h \
../resource/fonts.h

#files included in renderable.h
renderable.o: ../resource/models.h