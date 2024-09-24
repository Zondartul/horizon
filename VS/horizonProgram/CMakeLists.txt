cmake_minimum_required(VERSION 3.10)
include(../horizon/CMake_utils.txt)
project(horizonProgram)

find_package(SDL2 REQUIRED)

# depend only on the previous layer of the engine
add_subdirectory(../horizonUtil buildUtil)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/source/" *.cpp)
#SUBDIRLIST(HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/code/include/")
#include_directories(HEADER_DIRS)


add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_SDL USE_DIRENT)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonGUI/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonRender/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonResource/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonProgram/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonUtil/code/include/"
    )
target_link_libraries(${PROJECT_NAME} PRIVATE 
    horizonResource
    horizonUtil
    ${SDL2_LIBRARIES}
    )

#install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin")
