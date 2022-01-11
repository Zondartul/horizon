#makefile for the /display files.

# compiler command
CC = ..\..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1 -c
# additional libraries (not used cause not linking)
#CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

gui: console.o Gui.o Gui2.o Gui3.o newconsole.o toolbox.o valscreen.o

console.o: console.cpp
	$(CC) $(CFLAGS) $< -o $@

console.o: \
console.h \
../gui/Gui2.h \
../main/control.h \
../resource/textureloader.h \
../game/physics.h 

Gui.o: Gui.cpp
	$(CC) $(CFLAGS) $< -o $@
	
Gui.o: \
Gui.h \
../util/globals.h \
../display/paint.h \
../resource/fonts.h 

Gui2.o: Gui2.cpp
	$(CC) $(CFLAGS) $< -o $@
	
Gui2.o: \
Gui2.h \
../util/globals.h \
../util/messenger.h \
../display/paint.h \
../resource/fonts.h \
../resource/textureloader.h \
../main/control.h 

Gui3.o: Gui3.cpp
	$(CC) $(CFLAGS) $< -o $@
	
Gui3.o: \
Gui3.h \
../util/globals.h \
../util/messenger.h \
../display/paint.h \
../main/control.h

newconsole.o: newconsole.cpp
	$(CC) $(CFLAGS) $< -o $@
	
newconsole.o: \
newconsole.h \
../gui/Gui2.h

toolbox.o: toolbox.cpp
	$(CC) $(CFLAGS) $< -o $@
	
toolbox.o: \
toolbox.h \
../gui/Gui2.h \
../main/control.h \
../gui/console.h \
../resource/textureloader.h \
../resource/models.h \
../game/physics.h 

valscreen.o: valscreen.cpp
	$(CC) $(CFLAGS) $< -o $@
	
valscreen.o: \
valscreen.h \
../gui/Gui2.h