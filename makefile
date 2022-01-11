
#only use the rules I specify
MAKEFLAGS += --no-builtin-rules

# compiler command
CC = ..\..\..\bin\g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1
# additional libraries libraries
CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype

all: $(ALL_OBJECTS)

#how da fuck do I clean
clean:
	del /q $(ALL_OBJECTS)

ALL_OBJECTS = $(O_display) $(O_experimental) $(O_game) $(O_gui) $(O_input) \
$(O_main) $(O_math) $(O_resource) $(O_util)

#1: run every sub-make
all: display experimental game gui input main math resource util $(ALL_OBJECTS)
	$(CC) $(CFLAGS) $(ALL_OBJECTS) -o a.exe $(CLIBS)
	
#2: if object files change, re-link

O_display = display/paint.o display/renderable.o
display:
	cd display && $(MAKE)
	
O_experimental = experimental/testfuncs.o
experimental:
	cd experimental && $(MAKE)
	
O_game = game/camera.o game/physics.o
game:
	cd game && $(MAKE)
	
O_gui = gui/console.o gui/Gui.o gui/Gui2.o gui/Gui3.o gui/newconsole.o gui/toolbox.o gui/valscreen.o
gui:
	cd gui && $(MAKE)
	
O_input = input/codetostring.o input/input.o input/keybinds.o 
input:
	cd input && $(MAKE)
	
O_main = main/control.o main/main.o
main:
	cd main && $(MAKE)
	
O_math = math/convexhull.o math/quaternions.o math/vectors.o
math:
	cd math && $(MAKE)
	
O_resource = resource/fonts.o resource/models.o resource/textureloader.o
resource:
	cd resource && $(MAKE)
	
O_util = util/debug.o util/globals.o util/hook.o util/messenger.o
util:
	cd util && $(MAKE)
		
	

	
.PHONY: all clean display experimental game gui input main math resource util