# CMakeLists.txt is for building Horizon
# using VSCode + cmake + gcc
# OR cmake notepad + cmake + make + gcc
# on either Linux or Windows

cmake_minimum_required(VERSION 3.20)
project(horizon)
# Horizon consists of several modules, such that
# later modules depend on earlier modules:
# PS: check order!
# --------- engine 
# * horizonUtil - generic C++ helper functions
# * horizonResource - finds and loads resource files
# * horizonProgram - OS-related stuff
# * horizonRender - graphics engine
# * horizonGUI - gui widget toolkit
# --------- application
# * horizonApp - final executable that uses the engine

# Cmake file logic:
# we make an "app" that uses the "engine"
# and set engine modules as its dependencies.
if(WIN32)
    add_compile_definitions(WIN32) 
endif(WIN32)
if(UNIX)
    add_compile_definitions(LINUX) 
endif(UNIX)

add_compile_definitions(GCC)

add_subdirectory(VS/SDL2 buildSDL2)
add_subdirectory(VS/horizonUtil buildUtil)
add_subdirectory(VS/horizonProgram buildProgram)
add_subdirectory(VS/horizonResource buildResource)
add_subdirectory(VS/horizonRender buildRender)
add_subdirectory(VS/horizonGUI buildGUI)
add_subdirectory(VS/horizonApp buildApp)
