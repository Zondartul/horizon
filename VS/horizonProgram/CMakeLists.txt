cmake_minimum_required(VERSION 3.20)
#include(../horizon/CMake_utils.txt)
project(horizonProgram)

#find_package(SDL2 REQUIRED)

# depend only on the previous layer of the engine
#add_subdirectory(../horizonUtil buildUtil)
# also depend on external library SDL2 (also compiled here)
#add_subdirectory(./ext/SDL2 buildSDL2)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/source/" *.cpp)
#SUBDIRLIST(HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/code/include/")
#include_directories(HEADER_DIRS)


add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE DLL_PROGRAM USE_SDL USE_DIRENT)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonGUI/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonRender/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonResource/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonProgram/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonUtil/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../SDL2/include/"
    )
target_link_libraries(${PROJECT_NAME} PRIVATE 
    horizonUtil
    SDL2
    )

#install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin")
