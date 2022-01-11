#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

resource: fonts.o models.o textureloader.o

fonts.o: fonts.cpp
	$(CC) $(CFLAGS) $< -o $@
	
fonts.o: \
fonts.h \
../util/globals.h \
../resource/textureloader.h

models.o: models.cpp
	$(CC) $(CFLAGS) $< -o $@
	
models.o: \
models.h \
../math/vectors.h

textureloader.o: textureloader.cpp
	$(CC) $(CFLAGS) $< -o $@

textureloader.o: \
textureloader.h \
../util/globals.h