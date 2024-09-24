#Zon's generic makefile
#target all:
#	finds every .cpp file in the source directory tree.
#	if corresponding dependency files exist in the build dir tree, includes them.
#	if rebuild required:
#		compiles .o files and puts them in corresponding dirs in build dir tree
#		compiles dependency files and puts them in corresponding dirs in build dir tree
#		links the .exe
#
#target clean:
#	removes all the files created during building.
#	Does not delete directories.
#
#target veryclean
#	removes the entire build directory
#
#target test:
#	prints random debug information.

# build options:
#	completed binary goes here.
    BINARYDIR = bin/
#	make searches for the sources here.
    SOURCEDIR = code/source/
    SOURCEDIR_EXT = codeExt/sourceExt/
#	temporary build files go here.
    BUILDDIR = build/
    BUILDDIR_EXT = buildExt/
#	header files are found here.
    INCLUDEDIR = \
	codeExt/includeExt/ 			\
	codeExt/includeExt/tinyXML2/	\
	code/include/ 					\
	code/include/math 				\
	code/include/program 			\
	code/include/render 			\
	code/include/GUI				\
	code/include/util 				\
	code/include/resource			\
	code/include/input 				\
	code/include/physics 			\
	code/include/physics/collision	\
	code/include/loaders 			\
	code/include/generators 		\
	code/include/experimental		\
	code/include/gadgets			\
	code/include/entity				\
	code/include/game				\
	code/include/main
#	libraries are found here.
  #  
# putting everything in the same directory is also acceptable.


#-lglew32 -lopengl32 -lmingw32

#poopcode follows:	
obj_as_cpp     =$(patsubst $(BUILDDIR)%.o,$(SOURCEDIR)%.cpp,$1)
obj_as_cpp_ext =$(patsubst $(BUILDDIR_EXT)%.o,$(SOURCEDIR_EXT)%.cpp,$1)
#kinda wish we didn't go back and forth with this
cpp_as_obj     =$(patsubst $(SOURCEDIR)%.cpp,$(BUILDDIR)%.o,$1)
cpp_as_obj_ext =$(patsubst $(SOURCEDIR_EXT)%.cpp,$(BUILDDIR_EXT)%.o,$1)
#for preprocessor files
cpp_as_prep=$(patsubst $(SOURCEDIR)%.cpp,$(BUILDDIR)%.prep.cpp,$1)
prep_as_obj=$(patsubst $(BUILDDIR)%.prep.cpp,$(BUILDDIR)%.o,$1)
prep_as_cpp=$(patsubst $(BUILDDIR)%.prep.cpp,$(SOURCEDIR)%.cpp,$1)
#recursive wildcard:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SOURCE_FILES     := $(sort $(strip $(call rwildcard,$(SOURCEDIR),*.cpp)))
SOURCE_FILES_EXT := $(sort $(strip $(call rwildcard,$(SOURCEDIR_EXT),*.cpp)))

DIRS := $(sort $(dir $(SOURCE_FILES)))
DIRS_EXT := $(sort $(dir $(SOURCE_FILES_EXT)))

BUILD_DIRS := $(patsubst $(SOURCEDIR)%,$(BUILDDIR)%,$(DIRS))
BUILD_DIRS_EXT := $(patsubst $(SOURCEDIR_EXT)%, $(BUILDDIR_EXT)%,$(DIRS_EXT))

DIRTARGETS     := $(addsuffix .dirstamp, $(BUILD_DIRS))
DIRTARGETS_EXT := $(addsuffix .dirstamp, $(BUILD_DIRS_EXT))

OBJ_FILES := $(call cpp_as_obj,$(SOURCE_FILES))
OBJ_FILES_EXT := $(call cpp_as_obj_ext,$(SOURCE_FILES_EXT))

PREP_FILES := $(call cpp_as_prep,$(SOURCE_FILES))

DEP_FILES := $(OBJ_FILES:.o=.d)
DEP_FILES_EXT := $(OBJ_FILES_EXT:.o=.d)

DEP_FILES_ACTUAL := $(call rwildcard,$(BUILDDIR),*.d)
DEP_FILES_ACTUAL_EXT := $(call rwildcard,$(BUILDDIR_EXT),*.d)

#detecting Windows / Linux
ifneq (,$(findstring Windows,$(OS)))
   STFU=1> NUL 2>&1
   NOERROR=2> NULL
   makedir=mkdir $(patsubst %/,%,$(subst /,\,$1))
   delfile=del $(subst /,\,$1)
   deldir=rmdir /s $(subst /,\,$1)
   stamp=echo directory stamp: this tells "make" that "$1" exists. > $1.dirstamp
# compile options:
   PROGNAME = a.exe
   CFLAGS = -g -fmax-errors=1 -std=gnu++17 -Wno-narrowing -fno-tree-vectorize -Werror=return-type -DDEF_OS=\"Windows\"
   LDFLAGS = 
   #-U__STRICT_ANSI__ is so that vsnprintf works... or just use -std=gnu++11 / 14
   CC = g++ $(CFLAGS) $(addprefix -I,$(INCLUDEDIR))
   LINK = g++ $(LDFLAGS)
   LIBDIR = libs/ libs/win/ bin/
   LIBS = $(addprefix -L,$(LIBDIR)) -lglew32 -lopengl32 -lmingw32 -lSDL2 -lSDL2main -lfreetype
   #-lBulletDynamics -lBulletCollision -lBulletSoftBody -lLinearMath
   
else
  OS := $(shell uname)
ifneq (,$(findstring Linux,$(OS)))
   STFU=1> /dev/null 2>&1
   #STFU=
   NOERROR=2> /dev/null
   makedir=mkdir -p $1
   delfile=rm $1 -f
   deldir=rm $1 -f -r
   stamp=echo directory stamp: this tells "make" that "$1" exists. > $1.dirstamp
# compile options:
   PROGNAME = a
   #CFLAGS = -g -fmax-errors=1 -std=gnu++17 -Wno-narrowing -fpermissive
   CFLAGS = -g -fmax-errors=1 -std=gnu++17 -Wno-narrowing -fno-tree-vectorize -Werror=return-type -m32 -DGLEW_NO_GLU -DDEF_OS=\"Linux\"
   CC = g++ $(CFLAGS) $(addprefix -I,$(INCLUDEDIR))
   #LINK = g++
   #LIBS = -lSDL2 -lSDL2main -lGL -lGLEW -lfreetype 
   LINK = g++ -m32
   LIBDIR = libs/ bin/
   LIBS = $(addprefix -L,$(LIBDIR)) -lGLEW -lOpenGL -lSDL2 -lSDL2main -lfreetype
else
  $(error $(OS) is not supported yet...)
endif
endif

MODE = NORMAL

.PHONY: all test clean cleansilent veryclean verycleansilent

ifeq ($(MODE),NORMAL)
all: $(DIRTARGETS) $(DIRTARGETS_EXT) $(BINARYDIR)$(PROGNAME)
endif
ifeq ($(MODE),DIRS)
all: $(DIRTARGETS) $(DIRTARGETS_EXT)
endif

test:
	@echo OS:
	@echo $(OS)
	@echo source files:
	@echo $(SOURCE_FILES)
	@echo source files ext:
	@echo $(SOURCE_FILES_EXT)
	@echo object files:
	@echo $(OBJ_FILES)
	@echo object files ext:
	@echo $(OBJ_FILES_EXT)
	@echo dependency files:
	@echo $(DEP_FILES)
	@echo dependency files ext:
	@echo $(DEP_FILES_EXT)
	@echo dirs:
	@echo $(DIRS)
	@echo build_dirs:
	@echo $(BUILD_DIRS)
	@echo dirtargets:
	@echo $(DIRTARGETS)
	@echo dirtargets_ext:
	@echo $(DIRTARGETS_EXT)
	@echo dirmode:
	@echo $(DIRMODE)
	@echo makedir:
	@echo $(makedir)

#this is probably a really bad way to do it
#but those mkdir and "error ignored" messages
#can go eat a bag of dicks
ifeq ($(MODE),NORMAL)
$(DIRTARGETS):
	@echo Rebuilding directory structure in $(BUILDDIR)
	@$(MAKE) MODE=DIRS $(STFU)
$(DIRTARGETS_EXT):
	@echo Rebuilding directory structure in $(BUILDDIR_EXT)
	@$(MAKE) MODE=DIRS_EXT $(STFU)
else
$(DIRTARGETS):
	@-$(call makedir,$(dir $@))
	@-$(call stamp,$(dir $@))
$(DIRTARGETS_EXT):
	@-$(call makedir,$(dir $@))
	@-$(call stamp,$(dir $@))
endif

clean:
	@$(MAKE) cleansilent $(STFU)

cleansilent:
	@-$(call delfile,$(OBJ_FILES) $(DEP_FILES) $(PREP_FILES))

veryclean:
	@echo Really delete $(BUILDDIR) ? y/n
	@$(MAKE) verycleansilent1 $(STFU)
	@echo also delete $(BUILDDIR_EXT) ? y/n
	@$(MAKE) verycleansilent2 $(STFU)

verycleansilent:
	@-$(call deldir,$(BUILDDIR))
	@-$(call deldir,$(BUILDDIR_EXT))
	
verycleansilent1:
	@-$(call deldir,$(BUILDDIR))
	
verycleansilent2:
	@-$(call deldir,$(BUILDDIR_EXT))

	
$(BINARYDIR)$(PROGNAME): $(OBJ_FILES_EXT) $(OBJ_FILES) $(DEP_FILES_EXT) $(DEP_FILES) $(PREP_FILES)
	@echo linking $@
	@$(LINK) $(OBJ_FILES) $(OBJ_FILES_EXT) $(LIBS) -o $(BINARYDIR)$(PROGNAME)
	@echo done

#	$(LINK) $(OBJ_FILES) $(LIBS) -o $(BINARYDIR)$(PROGNAME)

	
.SECONDEXPANSION:$(OBJ_FILES) $(OBJ_FILES_EXT)
#$$(patsubst build/%.o,source/%.cpp,$$(patsubst build/%.d,build/%.o,$$@))
dep_as_obj=$(1:.d=.o)
dep_as_obj_ext=$(1:.d=.o)

$(DEP_FILES):
	@echo building $@ (dep)
	@$(CC) $(call obj_as_cpp,$(call dep_as_obj,$@)) -o $(call dep_as_obj,$@) -c -MMD

$(DEP_FILES_EXT):
	@echo building $@ (dep ext)
	@$(CC) $(call obj_as_cpp_ext,$(call dep_as_obj_ext,$@)) -o $(call dep_as_obj_ext,$@) -c -MMD

$(OBJ_FILES): $$(call obj_as_cpp,$$@)
	@echo building $@
	@$(CC) $< -o $@ -c -MMD
	
$(OBJ_FILES_EXT): $$(call obj_as_cpp_ext,$$@)
	@echo building $@ ext
	@$(CC) $< -o $@ -c -MMD
	
$(PREP_FILES): $$(call prep_as_cpp,$$@)
	@echo preprocessing $@
	@$(CC) $< -o $@ -c -E
	
include $(DEP_FILES_ACTUAL)


