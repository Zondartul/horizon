#first, "make clean" is called to ensure that the intermediary files do not exist.
#then, "make" is called and starts checking "all".
#since sources exist, we have a list of required object files.
#these object files do not exist. Therefore, they need to be rebuilt.
#a single rule exists to go from .cpp to .o files, so it is executed.
#this creates object files and dependency files.
#next time make is called, it includes these dependency files and adds them to
#the dependency check. Automation complete.



#==========================================================================
#options:
#the directory, relative to the makefile, where the .cpp files are held
SOURCEDIR = ../source

#the directory, relative to the makefile, where the .h files are held
HEADERDIR = ../include

#the directory, relative to the makefile, in which the intermediate files
#will be placed.
TEMPDIR = ../build

PROGNAME = a.exe

# variables:
CC = g++

# compiler flags
# add -mwindows to not couple to the calling console / not create a console
CFLAGS = -std=c++0x -g -fmax-errors=1
# additional libraries libraries
# opengl32 - used basically everywhere
# freetype - used by fonts.cpp to generate fancy fonts.
# SDL2 - used for platform independent window, input, etc etc
# SDL2main and mingw32 - needed because of "WinMain16" shenanigans.
# libraries that used to be here:
# glu32 - was used by textureloader - LoadTextureRAW/BMP/PNG for mipmaps
# user32, gdi32 - *hisssssssssssssssss*
CLIBS = -lopengl32 -lfreetype -lmingw32 -lSDL2main -lSDL2 -LC:/MinGW32/horizon/horizonSVN/horizon/lib
# command to compile an object file
COMPILEOBJECT = $(CC) -c $(CFLAGS) $< -o $(TEMPDIR)/$*.o -I$(HEADERDIR)
# command to compile a dependency file
COMPILEDEPENDENCY = $(CC) -MM $(CFLAGS) $< -o $(TEMPDIR)/$*.d -I$(HEADERDIR) && depfmt $(TEMPDIR)/$*.d
# command to link everything together
COMPILEEXE = $(CC) $(CFLAGS) $(OBJECTFILES) -o $(PROGNAME) $(CLIBS)
#===========================================================================
#utils:
#recursive wildcard function to match things in lower directories
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
#change slash direction so windows can understand it
flipslashes=$(subst /,\,$1)
#remove trailing backslashes
removetrail=$(patsubst %\,%,$1)
#==========================================================================
#setup:
# only use the rules I specify
MAKEFLAGS += --no-builtin-rules
#also disable built-in suffix rules
.SUFFIXES:
#first, enumerate the source files.
SOURCEFILES = $(call rwildcard,$(SOURCEDIR),*.cpp)
#$(info SOURCEFILES is $(SOURCEFILES))
#no wait we have to change dir...
#define the object files as just the source files with a different extension.
OBJECTFILES = $(subst $(SOURCEDIR),$(TEMPDIR),$(SOURCEFILES:.cpp=.o))
#$(info OBJECTFILES is $(OBJECTFILES))
#same for dependency files
DEPFILES = $(subst $(SOURCEDIR),$(TEMPDIR),$(SOURCEFILES:.cpp=.d))
#$(info DEPFILES is $(DEPFILES))
#and for fixed dependency files
FIXEDDEPFILES = $(DEPFILES:.d=.d.fixed)
#list of directories in the temp folder:
DIRS = $(call removetrail,$(call flipslashes,$(sort $(dir $(DEPFILES)))))
#$(info DIRS is $(DIRS))
#generate a "delete all" string

ALLDELETE = $(call flipslashes,$(OBJECTFILES:%="%") $(DEPFILES:%="%") $(FIXEDDEPFILES:%="%"))
#ALLDELETE = $(subst /,\,$(OBJECTFILES:%="%") $(DEPFILES:%="%") $(FIXEDDEPFILES:%="%"))
#$(info oh shit here we go)

#This is the main rule
$(PROGNAME): $(DIRS) $(OBJECTFILES)
	$(info linking the .exe)
	@$(COMPILEEXE) && echo done

#the rule to clean everything
clean:
	@del /f /q $(ALLDELETE) > nul 2> nul

$(DIRS):
	mkdir $@
#generic rule to start the process
#$(TEMPDIR)/%.o:
#	echo oh shit son
$(TEMPDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(info building $@)
	@$(COMPILEOBJECT) && $(COMPILEDEPENDENCY)
	
#include any dependency files, if they exist. They are actually in makefile format.
#$(info include is $(call rwildcard,,*.d.fixed))
FIXEDDEPSACTUAL = $(call rwildcard,$(TEMPDIR),*.d.fixed)
#$(info FIXEDDEPSACTUAL is $(FIXEDDEPSACTUAL))
-include $(FIXEDDEPSACTUAL)