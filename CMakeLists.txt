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
    option(DIRECTX "For SDL2, disable DirectX, use OpenGL" OFF)
    option(FT_DISABLE_PNG "For FreeType, disable PNG" ON)
    option(FT_DISABLE_HARFBUZZ "For FreeType, disable HarfBuzz" ON)
    #option(FT_REQUIRE_PNG "For FreeType, disable PNG" OFF)
    set(MY_MAKE_CMD "C:/ProgramData/chocolatey/bin/make.exe")
endif(WIN32)
if(UNIX)
    add_compile_definitions(LINUX)
    set(MY_MAKE_CMD "make") 
endif(UNIX)

add_compile_definitions(GCC)

# download the dependencies of GLEW
execute_process(COMMAND ${MY_MAKE_CMD} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/VS/GLEW/auto" COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)
#message("rv='${rv}'") #RESULT_VARIABLE rv

#add_custom_target("MyGLEWSetup" COMMAND make WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/VS/GLEW/auto")

add_subdirectory(VS/SDL2 buildSDL2)
add_subdirectory(VS/GLEW/build/cmake buildGLEW)
add_subdirectory(VS/FreeType buildFreeType)
add_subdirectory(VS/horizonUtil buildUtil)
add_subdirectory(VS/horizonProgram buildProgram)
add_subdirectory(VS/horizonResource buildResource)
add_subdirectory(VS/horizonRender buildRender)
add_subdirectory(VS/horizonGUI buildGUI)
add_subdirectory(VS/horizonApp buildApp)
