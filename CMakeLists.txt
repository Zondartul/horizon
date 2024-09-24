# CMakeLists.txt is for building Horizon
# using VSCode + cmake + gcc
# OR cmake notepad + cmake + make + gcc
# on either Linux or Windows

cmake_minimum_required(VERSION 3.10)
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
add_subdirectory(VS/horizonApp buildApp)
