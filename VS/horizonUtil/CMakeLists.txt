cmake_minimum_required(VERSION 3.10)
include(../horizon/CMake_utils.txt)
project(horizonUtil)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/source/" *.cpp)
#SUBDIRLIST(HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/code/include/")
#include_directories(HEADER_DIRS)
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/Ext/"
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
