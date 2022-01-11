#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

main: control.o main.o

control.o: control.cpp
	$(CC) $(CFLAGS) $< -o $@

control.o: \
../util/globals.h \
../util/messenger.h \
../resource/textureloader.h \
../resource/fonts.h \
../util/debug.h \
../input/input.h \
../display/paint.h \
../gui/Gui2.h \
../gui/Gui3.h \
../math/vectors.h \
../resource/models.h \
../math/quaternions.h \
../util/hook.h \
../gui/newconsole.h \
../gui/valscreen.h \
../input/keybinds.h \
../game/camera.h \
../display/renderable.h \
../experimental/testfuncs.h

main.o: main.cpp
	$(CC) $(CFLAGS) $< -o $@