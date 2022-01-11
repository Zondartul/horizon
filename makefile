# makefile for the entire horizon project

# options:
# only use the rules I specify
MAKEFLAGS += --no-builtin-rules
PROGNAME = a.exe

# variables:
CC = g++
# compiler flags
CFLAGS = -std=c++0x -g -fmax-errors=1
# additional libraries libraries
CLIBS = -luser32 -lopengl32 -lglu32 -lgdi32 -lfreetype
# command to compile an object file
COMPILEOBJECT = $(CC) -c $(CFLAGS) $< -o $*.o
# command to compile a dependency file
COMPILEDEPENDENCY = $(CC) -MM $(CFLAGS) $< -o $*.d && depfmt $*.d

#recursive wildcard function to match things in lower directories
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
#this prints the value of a variable
#$(info AOBJECTS is $(ALLOBJECTS))

#first, search for all .cpp files.
ALLSOURCES = $(call rwildcard,,*.cpp)
#define the object files as just the source files with a different extension.
ALLOBJECTS = $(ALLSOURCES:.cpp=.o)
#define other object types the same way
ALLOTHER = $(ALLSOURCES:.cpp=.d) $(ALLSOURCES:.cpp=.d.fixed)
#now we can write the command to link all those object files into an exe.
COMPILEEXE = $(CC) $(CFLAGS) $(ALLOBJECTS) -o $(PROGNAME) $(CLIBS)

#the rule to compile everything
$(PROGNAME): $(ALLOBJECTS)
	@$(COMPILEEXE)

#the rule to delete all temporary files
ALLDELETE = $(subst /,\,$(ALLOBJECTS:%="%") $(ALLOTHER:%="%"))
clean:
	@del /f /q $(ALLDELETE) > nul 2> nul

#generic rule to start the process
%.o: %.cpp
	@$(COMPILEOBJECT) && $(COMPILEDEPENDENCY)
	
#include any dependency files, if they exist. They are actually in makefile format.
#$(info include is $(call rwildcard,,*.d.fixed))
-include $(call rwildcard,,*.d.fixed)
	
	
#first, "make clean" is called to ensure that the intermediary files do not exist.
#then, "make" is called and starts checking "all".
#since sources exist, we have a list of required object files.
#these object files do not exist. Therefore, they need to be rebuilt.
#a single rule exists to go from .cpp to .o files, so it is executed.
#this creates object files and dependency files.
#next time make is called, it includes these dependency files and adds them to
#the dependency check. Automation complete.












