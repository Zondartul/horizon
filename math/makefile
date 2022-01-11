#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

math: convexhull.o quaternions.o vectors.o

convexhull.o: convexhull.cpp
	$(CC) $(CFLAGS) $< -o $@
	
convexhull.o: \
convexhull.h \
../util/globals.h \
../math/vectors.h \
../resource/models.h \
../display/renderable.h \
../main/control.h \
../util/hook.h \
../game/camera.h

quaternions.o: quaternions.cpp
	$(CC) $(CFLAGS) $< -o $@
	
quaternions.o: \
quaternions.h \
../math/vectors.h

vectors.o: vectors.cpp
	$(CC) $(CFLAGS) $< -o $@
	
vectors.o: \
vectors.h \
../util/globals.h