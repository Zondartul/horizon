cmake_minimum_required(VERSION 3.20)
#include(../horizon/CMake_utils.txt)
project(horizonUtil)

# no SDL for UTIL
#target_compile_definitions(USE_SDL)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/source/" *.cpp)
#SUBDIRLIST(HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/code/include/")
#include_directories(HEADER_DIRS)


add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE DLL_UTIL)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/Ext/"
)

#install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin")
