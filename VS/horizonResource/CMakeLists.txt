cmake_minimum_required(VERSION 3.10)
include(../horizon/CMake_utils.txt)
project(horizonResource)

# depend only on the previous layer of the engine
add_subdirectory(../horizonUtil buildUtil)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/source/" *.cpp)
#SUBDIRLIST(HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/code/include/")
#include_directories(HEADER_DIRS)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/code/include/")

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    horizonUtil
    )

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
