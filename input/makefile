#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

input: codetostring.o input.o keybinds.o

codetostring.o: codetostring.cpp
	$(CC) $(CFLAGS) $< -o $@
	
codetostring.o: \
codetostring.h \
../util/globals.h

input.o: input.cpp
	$(CC) $(CFLAGS) $< -o $@
	
input.o: \
input.h \
../util/messenger.h \
../input/codetostring.h \

keybinds.o: keybinds.cpp
	$(CC) $(CFLAGS) $< -o $@

keybinds.o: \
keybinds.h \
../util/globals.h \
../util/messenger.h \
../gui/newconsole.h